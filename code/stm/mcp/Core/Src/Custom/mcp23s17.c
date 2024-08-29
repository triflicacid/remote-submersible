#include "mcp23S17.h"

static inline uint8_t get_control_byte(bool write, uint8_t hw_addr) {
  return 0x40 | ((hw_addr & 0x7) << 1) | (write ? 0 : 1);
}

// start communication -- select device
static void select(mcp23s17_t *mcp) {
	HAL_Delay(10);
	write_pin(mcp->port, mcp->pin, false);
	HAL_Delay(10);
}

// stop communication -- deselect device
static void deselect(mcp23s17_t *mcp) {
	HAL_Delay(10);
	write_pin(mcp->port, mcp->pin, true);
	HAL_Delay(10);
}

void mcp23s17_write(mcp23s17_t *mcp, uint8_t address, uint8_t data) {
  select(mcp);

  uint8_t payload[] = {
    0x40, // get_control_byte(true, mcp->hw_addr),
    address,
    data
  };

  HAL_SPI_Transmit(mcp->spi, payload, 3, MCP_TIMEOUT);

  deselect(mcp);
}

uint8_t mcp23s17_read(mcp23s17_t *mcp, uint8_t address) {
  select(mcp);

  uint8_t payload[] = {
    0x41, // get_control_byte(false, mcp->hw_addr),
    address
  };

  HAL_SPI_Transmit(mcp->spi, payload, 2, MCP_TIMEOUT);

  // read data sent back
  uint8_t data;
  HAL_SPI_Receive(mcp->spi, &data, 1, MCP_TIMEOUT);

  deselect(mcp);
  return data;
}

void mcp23s17_init(mcp23s17_t *mcp, SPI_HandleTypeDef *spi, port_t *port, pin_t pin, uint8_t hw_addr, port_t *rst_port, pin_t rst_pin) {
  mcp->spi = spi;
  mcp->port = port;
  mcp->pin = pin;
  mcp->hw_addr = hw_addr;
  mcp->rst_port = rst_port;
  mcp->rst_pin = rst_pin;

  // toggle reset
  write_pin(rst_port, rst_pin, true);
  HAL_Delay(1);
  mcp23s17_reset(mcp);

  // toggle chip select
  deselect(mcp);
  select(mcp);
  deselect(mcp);
}

void mcp23s17_reset(mcp23s17_t *mcp) {
  write_pin(mcp->rst_port, mcp->rst_pin, false);
  HAL_Delay(1);
  write_pin(mcp->rst_port, mcp->rst_pin, true);
  HAL_Delay(5);
}
