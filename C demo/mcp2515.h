#ifndef MCP2515_H
#define MCP2515_H

#include <hardware/spi.h>
#include <pico/stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define MCP_CANSTAT 0x0E
#define MCP_CANCTRL 0x0F
#define MCP_TEC 0x1C
#define MCP_REC 0x1D
#define MCP_CNF1 0x2A
#define MCP_CNF2 0x29
#define MCP_CNF3 0x28
#define MCP_CANINTF 0x2C
#define MCP_CANINTE 0x2B
#define MCP_TXB0CTRL 0x30
#define MCP_TXB0SIDH 0x31
#define MCP_TXB0SIDL 0x32
#define MCP_TXB0DLC 0x35
#define MCP_TXB0D0 0x36
#define MCP_RXB0CTRL 0x60
#define MCP_RXB0SIDH 0x61
#define MCP_RXB0SIDL 0x62
#define MCP_RXB0DLC 0x65
#define MCP_RXB0D0 0x66

#define MCP_WRITE 0x02
#define MCP_READ 0x03
#define MCP_BITMOD 0x05
#define MCP_LOAD_TX0 0x40
#define MCP_RTS_TX0 0x81
#define MCP_READ_STATUS 0xA0
#define MCP_RX_STATUS 0xB0
#define MCP_RESET 0xC0

#define MODE_NORMAL 0x00
#define MODE_SLEEP 0x20
#define MODE_LOOPBACK 0x40
#define MODE_LISTENONLY 0x60
#define MODE_CONFIG 0x80

#define CAN_SPI spi1
#define CAN_SCK 10
#define CAN_MOSI 11
#define CAN_MISO 12
#define CAN0_CS 8
#define CAN1_CS 7
#define CAN0_INT 23
#define CAN1_INT 25

typedef struct {
  spi_inst_t *spi;
  uint32_t cs_pin;
} mcp2515_t;

typedef struct {
  uint32_t id;
  uint8_t dlc;
  uint8_t data[8];
} can_msg_t;

void mcp2515_init(mcp2515_t *dev, spi_inst_t *spi, uint32_t cs_pin);
void mcp2515_reset(mcp2515_t *dev);
uint8_t mcp2515_read_reg(mcp2515_t *dev, uint8_t reg);
void mcp2515_write_reg(mcp2515_t *dev, uint8_t reg, uint8_t val);
void mcp2515_set_mode(mcp2515_t *dev, uint8_t mode);
void mcp2515_bit_modify(mcp2515_t *dev, uint8_t reg, uint8_t mask, uint8_t val);
bool mcp2515_transmit(mcp2515_t *dev, can_msg_t *msg);
bool mcp2515_receive(mcp2515_t *dev, can_msg_t *msg);

#endif
