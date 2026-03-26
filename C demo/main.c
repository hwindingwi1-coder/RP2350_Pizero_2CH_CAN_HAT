#include <hardware/spi.h>
#include "mcp2515.h"
#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>

void setup_can(mcp2515_t *dev) {
  mcp2515_reset(dev);

  // Set bit timing for 8MHz crystal, 125kbps
  mcp2515_write_reg(dev, MCP_CNF1, 0x03); 
  mcp2515_write_reg(dev, MCP_CNF2, 0xAC);
  mcp2515_write_reg(dev, MCP_CNF3, 0x03);

  // Set RXB0 to receive all messages
  mcp2515_write_reg(dev, MCP_RXB0CTRL, 0x60);

  // Set to Normal Mode
  mcp2515_set_mode(dev, MODE_NORMAL);
}

int main() {
  stdio_init_all();
  sleep_ms(2000);
  printf("RP2350-PiZero 2-CH CAN HAT: Bidirectional Cross-Channel Test\n");

  spi_init(CAN_SPI, 10 * 1000 * 1000);
  gpio_set_function(CAN_SCK, GPIO_FUNC_SPI);
  gpio_set_function(CAN_MOSI, GPIO_FUNC_SPI);
  gpio_set_function(CAN_MISO, GPIO_FUNC_SPI);

  mcp2515_t can0, can1;
  mcp2515_init(&can0, CAN_SPI, CAN0_CS);
  mcp2515_init(&can1, CAN_SPI, CAN1_CS);

  setup_can(&can0);
  setup_can(&can1);

  printf("Setup complete.\n");
  printf("Ensure H-H and L-L are jumpered between CAN channels.\n");

  uint32_t count = 0;
  while (true) {
    can_msg_t tx_msg, rx_msg;
    bool received = false;

    // Phase 1: CAN0 (CH1) -> CAN1 (CH2)
    tx_msg.id = 0x101;
    tx_msg.dlc = 8;
    sprintf((char *)tx_msg.data, "C0:%04d", count);
    printf("CAN0 Sending: [%s]...", tx_msg.data);
    if (mcp2515_transmit(&can0, &tx_msg)) {
      printf("Done.\n");
    } else {
      printf("Failed (Busy).\n");
    }

    received = false;
    for (int i = 0; i < 50; i++) {
      if (mcp2515_receive(&can1, &rx_msg)) {
        received = true;
        break;
      }
      sleep_ms(10);
    }
    if (received) {
      printf("CAN1 Received ID 0x%X: [%s]\n", rx_msg.id, rx_msg.data);
    } else {
      printf("CAN1: Timeout - No message from CAN0.\n");
    }

    sleep_ms(500);

    // Phase 2: CAN1 (CH2) -> CAN0 (CH1)
    tx_msg.id = 0x202;
    tx_msg.dlc = 8;
    sprintf((char *)tx_msg.data, "C1:%04d", count);
    printf("CAN1 Sending: [%s]...", tx_msg.data);
    if (mcp2515_transmit(&can1, &tx_msg)) {
      printf("Done.\n");
    } else {
      printf("Failed (Busy).\n");
    }

    received = false;
    for (int i = 0; i < 50; i++) {
      if (mcp2515_receive(&can0, &rx_msg)) {
        received = true;
        break;
      }
      sleep_ms(10);
    }
    if (received) {
      printf("CAN0 Received ID 0x%X: [%s]\n", rx_msg.id, rx_msg.data);
    } else {
      printf("CAN0: Timeout - No message from CAN1.\n");
    }

    count++;
    printf("----------------------------------\n");
    
    // Heartbeat LED
    gpio_init(CAN1_INT);
    gpio_set_dir(CAN1_INT, GPIO_OUT);
    gpio_put(CAN1_INT, 1);
    sleep_ms(500);
    gpio_put(CAN1_INT, 0);
    sleep_ms(500);
  }

  return 0;
}
