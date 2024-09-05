#include "mcp23S17.h"

static inline uint8_t get_control_byte(bool write, uint8_t hw_addr) {
  return 0x40 | ((hw_addr & 0x7) << 1) | (write ? 0 : 1);
}

void mcp_init(mcp_t *mcp, SPI_HandleTypeDef *spi, const pin_t *cs, uint8_t hw_addr, const pin_t *rst) {
  mcp->spi = spi;
  mcp->cs = cs;
  mcp->hw_addr = hw_addr;
  mcp->rst = rst;

  // toggle reset & chip select
  set_pin(cs);
  set_pin(rst);
  HAL_Delay(1);

  reset_pin(cs);
  reset_pin(rst);
  HAL_Delay(1);

  set_pin(cs);
  set_pin(rst);
  HAL_Delay(5);
}

void mcp_write(mcp_t *mcp, uint8_t address, uint8_t data) {
  reset_pin(mcp->cs);

  uint8_t payload[] = {
    0x40, // get_control_byte(true, mcp->hw_addr),
    address,
    data
  };

  HAL_SPI_Transmit(mcp->spi, payload, 3, MCP_TIMEOUT);
  HAL_Delay(1);

  set_pin(mcp->cs);
  HAL_Delay(1);
}

uint8_t mcp_read(mcp_t *mcp, uint8_t address) {
  reset_pin(mcp->cs);

  uint8_t payload[] = {
    0x41, // get_control_byte(false, mcp->hw_addr),
    address
  };

  HAL_SPI_Transmit(mcp->spi, payload, 2, MCP_TIMEOUT);

  // read data sent back
  uint8_t data;
  HAL_SPI_Receive(mcp->spi, &data, 1, MCP_TIMEOUT);
  HAL_Delay(1);

  set_pin(mcp->cs);
  HAL_Delay(1);

  return data;
}

void mcp_reset(mcp_t *mcp) {
  reset_pin(mcp->rst);
  HAL_Delay(1);

  set_pin(mcp->rst);
  HAL_Delay(5);
}
