from machine import Pin, SPI
import time
from mcp2515 import MCP2515

# RP2350 Pin Definitions
CAN0_CS = 8
CAN1_CS = 7
CAN1_INT_LED = 25

def main():
    print("RP2350-PiZero 2-CH CAN HAT: MicroPython Bidirectional Test")
    
    # Initialize SPI1
    spi = SPI(1, baudrate=10000000, sck=Pin(10), mosi=Pin(11), miso=Pin(12))
    
    # Initialize MCP2515 controllers
    can0 = MCP2515(spi, CAN0_CS)
    can1 = MCP2515(spi, CAN1_CS)
    
    # Configure for 8MHz crystal, 125kbps
    can0.configure()
    can1.configure()
    
    # LED indication (CAN1 INT pin as output)
    led = Pin(CAN1_INT_LED, Pin.OUT, value=0)
    
    print("Setup complete. Jumper H-H and L-L for cross-channel test.")
    
    count = 0
    while True:
        # Phase 1: CAN0 -> CAN1
        msg_str = "C0:{:04d}".format(count)
        data = msg_str.encode('utf-8')

        
        print("CAN0 Sending: [{}]...".format(msg_str))
        if can0.transmit(0x101, data):
            print("Done.")
        else:
            print("Failed (Busy).")
            
        # Wait and receive on CAN1
        received = None
        for _ in range(50):
            received = can1.receive()
            if received:
                break
            time.sleep_ms(10)
            
        if received:
            print("CAN1 Received ID 0x{:X}: [{}]".format(received["id"], received["data"].decode()))
        else:
            print("CAN1: Timeout - No message from CAN0.")
            
        time.sleep_ms(500)
        
        # Phase 2: CAN1 -> CAN0
        msg_str = "C1:{:04d}".format(count)
        data = msg_str.encode('utf-8')

        
        print("CAN1 Sending: [{}]...".format(msg_str))
        if can1.transmit(0x202, data):
            print("Done.")
        else:
            print("Failed (Busy).")
            
        # Wait and receive on CAN0
        received = None
        for _ in range(50):
            received = can0.receive()
            if received:
                break
            time.sleep_ms(10)
            
        if received:
            print("CAN0 Received ID 0x{:X}: [{}]".format(received["id"], received["data"].decode()))
        else:
            print("CAN0: Timeout - No message from CAN1.")
            
        count += 1
        print("-" * 34)
        
        # Blink LED
        led.value(1)
        time.sleep_ms(500)
        led.value(0)
        time.sleep_ms(500)

if __name__ == "__main__":
    main()
