from machine import Pin, SPI
import time

# MCP2515 OPCODES
RESET = 0xC0
READ = 0x03
WRITE = 0x02
BITMOD = 0x05
READ_STATUS = 0xA0

# Registers
CANSTAT = 0x0E
CANCTRL = 0x0F
CNF1 = 0x2A
CNF2 = 0x29
CNF3 = 0x28
CANINTF = 0x2C
TXB0CTRL = 0x30
TXB0SIDH = 0x31
TXB0SIDL = 0x32
TXB0DLC = 0x35
TXB0D0 = 0x36
RXB0CTRL = 0x60
RXB0SIDH = 0x61
RXB0SIDL = 0x62
RXB0DLC = 0x65
RXB0D0 = 0x66

# Modes
MODE_NORMAL = 0x00
MODE_LOOPBACK = 0x40
MODE_CONFIG = 0x80

class MCP2515:
    def __init__(self, spi, cs_pin):
        self.spi = spi
        self.cs = Pin(cs_pin, Pin.OUT, value=1)
        self.reset()
        
    def _select(self):
        self.cs.value(0)
        
    def _deselect(self):
        self.cs.value(1)
        
    def reset(self):
        self._select()
        self.spi.write(bytearray([RESET]))
        self._deselect()
        time.sleep_ms(10)
        
    def read_reg(self, reg):
        self._select()
        self.spi.write(bytearray([READ, reg]))
        val = self.spi.read(1)[0]
        self._deselect()
        return val
        
    def write_reg(self, reg, val):
        self._select()
        self.spi.write(bytearray([WRITE, reg, val]))
        self._deselect()
        
    def bit_modify(self, reg, mask, val):
        self._select()
        self.spi.write(bytearray([BITMOD, reg, mask, val]))
        self._deselect()
        
    def set_mode(self, mode):
        # Clear mode bits (7-5) and set new mode
        self.bit_modify(CANCTRL, 0xE0, mode)
        # Wait for mode change
        while (self.read_reg(CANSTAT) & 0xE0) != mode:
            time.sleep_us(10)
            
    def configure(self):
        # Set 125kbps for 8MHz crystal
        self.write_reg(CNF1, 0x03)
        self.write_reg(CNF2, 0xAC)
        self.write_reg(CNF3, 0x03)
        # Receive all messages on RXB0
        self.write_reg(RXB0CTRL, 0x60)
        self.set_mode(MODE_NORMAL)
        
    def transmit(self, id, data):
        # Check if TXB0 is busy
        if self.read_reg(TXB0CTRL) & 0x08:
            return False
        
        # Standard ID (11-bit)
        self.write_reg(TXB0SIDH, (id >> 3) & 0xFF)
        self.write_reg(TXB0SIDL, (id << 5) & 0xE0)
        
        dlc = len(data) & 0x0F
        self.write_reg(TXB0DLC, dlc)
        
        for i in range(dlc):
            self.write_reg(TXB0D0 + i, data[i])
            
        # Request to send TXB0
        self.bit_modify(TXB0CTRL, 0x08, 0x08)
        return True
        
    def receive(self):
        intf = self.read_reg(CANINTF)
        if not (intf & 0x01): # RXB0IF
            return None
            
        # Read ID
        sidh = self.read_reg(RXB0SIDH)
        sidl = self.read_reg(RXB0SIDL)
        msg_id = (sidh << 3) | (sidl >> 5)
        
        dlc = self.read_reg(RXB0DLC) & 0x0F
        data = bytearray(dlc)
        for i in range(dlc):
            data[i] = self.read_reg(RXB0D0 + i)
            
        # Clear RXB0IF
        self.bit_modify(CANINTF, 0x01, 0x00)
        return {"id": msg_id, "data": data}
