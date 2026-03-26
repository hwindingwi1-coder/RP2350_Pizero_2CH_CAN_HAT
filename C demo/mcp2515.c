#include "mcp2515.h"
#include <stdio.h>
#include <string.h>

void mcp2515_init(mcp2515_t *dev, spi_inst_t *spi, uint32_t cs_pin) {
  dev->spi = spi;
  dev->cs_pin = cs_pin;

  gpio_init(cs_pin);
  gpio_set_dir(cs_pin, GPIO_OUT);
  gpio_put(cs_pin, 1);

  printf("MCP2515 Initializing on CS %d...\n", cs_pin);
}

static inline void mcp2515_select(mcp2515_t *dev) { gpio_put(dev->cs_pin, 0); }

static inline void mcp2515_deselect(mcp2515_t *dev) {
  gpio_put(dev->cs_pin, 1);
}

void mcp2515_reset(mcp2515_t *dev) {
  mcp2515_select(dev);
  uint8_t cmd = MCP_RESET;
  spi_write_blocking(dev->spi, &cmd, 1);
  mcp2515_deselect(dev);
  sleep_ms(10);
}

uint8_t mcp2515_read_reg(mcp2515_t *dev, uint8_t reg) {
  mcp2515_select(dev);
  uint8_t buf[2] = {MCP_READ, reg};
  spi_write_blocking(dev->spi, buf, 2);

  uint8_t val;
  spi_read_blocking(dev->spi, 0, &val, 1);
  mcp2515_deselect(dev);
  return val;
}

void mcp2515_write_reg(mcp2515_t *dev, uint8_t reg, uint8_t val) {
  mcp2515_select(dev);
  uint8_t buf[3] = {MCP_WRITE, reg, val};
  spi_write_blocking(dev->spi, buf, 3);
  mcp2515_deselect(dev);
}

void mcp2515_bit_modify(mcp2515_t *dev, uint8_t reg, uint8_t mask, uint8_t val) {
  mcp2515_select(dev);
  uint8_t buf[4] = {MCP_BITMOD, reg, mask, val};
  spi_write_blocking(dev->spi, buf, 4);
  mcp2515_deselect(dev);
}

void mcp2515_set_mode(mcp2515_t *dev, uint8_t mode) {
  uint8_t ctrl = mcp2515_read_reg(dev, MCP_CANCTRL);
  ctrl &= 0x1F;
  ctrl |= mode;
  mcp2515_write_reg(dev, MCP_CANCTRL, ctrl);

  while ((mcp2515_read_reg(dev, MCP_CANSTAT) & 0xE0) != mode) {
    sleep_us(10);
  }
}

bool mcp2515_transmit(mcp2515_t *dev, can_msg_t *msg) {
  // Check if TXB0 is busy
  if (mcp2515_read_reg(dev, MCP_TXB0CTRL) & 0x08) {
    return false;
  }

  // Set standard ID only for simplicity
  mcp2515_write_reg(dev, MCP_TXB0SIDH, (uint8_t)(msg->id >> 3));
  mcp2515_write_reg(dev, MCP_TXB0SIDL, (uint8_t)(msg->id << 5));
  mcp2515_write_reg(dev, MCP_TXB0DLC, msg->dlc & 0x0F);

  // Load data
  for (int i = 0; i < msg->dlc; i++) {
    mcp2515_write_reg(dev, MCP_TXB0D0 + i, msg->data[i]);
  }

  // Request to send TXB0
  mcp2515_bit_modify(dev, MCP_TXB0CTRL, 0x08, 0x08);
  return true;
}

bool mcp2515_receive(mcp2515_t *dev, can_msg_t *msg) {
  uint8_t intf = mcp2515_read_reg(dev, MCP_CANINTF);
  if (!(intf & 0x01)) { // Check RXB0IF
    return false;
  }

  // Read standard ID
  uint8_t sidh = mcp2515_read_reg(dev, MCP_RXB0SIDH);
  uint8_t sidl = mcp2515_read_reg(dev, MCP_RXB0SIDL);
  msg->id = ((uint32_t)sidh << 3) | (sidl >> 5);
  msg->dlc = mcp2515_read_reg(dev, MCP_RXB0DLC) & 0x0F;

  // Read data
  for (int i = 0; i < msg->dlc; i++) {
    msg->data[i] = mcp2515_read_reg(dev, MCP_RXB0D0 + i);
  }

  // Clear RXB0IF
  mcp2515_bit_modify(dev, MCP_CANINTF, 0x01, 0x00);
  return true;
}
