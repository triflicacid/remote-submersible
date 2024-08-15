#include "mcp23S17.h"

static inline uint8_t get_control_byte(bool write, uint8_t hw_addr) {
  return 0x40 | ((hw_addr & 0x7) << 1) | (write ? 0 : 1);
}

// start communication -- select device
static inline void select(mcp23s17_t *mcp) {
	write_pin(mcp->port, mcp->pin, false);
}

// stop communication -- deselect device
static inline void deselect(mcp23s17_t *mcp) {
	HAL_Delay(1);
	write_pin(mcp->port, mcp->pin, true);
	HAL_Delay(1);
}

void mcp23s17_write(mcp23s17_t *mcp, uint8_t address, uint8_t data) {
  select(mcp);

  uint8_t payload[] = {
    get_control_byte(true, mcp->hw_addr),
    address,
    data
  };

  HAL_SPI_Transmit(mcp->spi, (uint8_t *) payload, sizeof(payload), MCP_TIMEOUT);

  deselect(mcp);
}

uint8_t mcp23s17_read(mcp23s17_t *mcp, uint8_t address) {
  select(mcp);

  uint8_t payload[] = {
    get_control_byte(false, mcp->hw_addr),
    address
  };

  HAL_SPI_Transmit(mcp->spi, (uint8_t *) &payload, sizeof(payload), MCP_TIMEOUT);

  // read data sent back
  uint8_t data;
  HAL_SPI_Receive(mcp->spi, &data, sizeof(data), MCP_TIMEOUT);

  deselect(mcp);
  return data;
}

void mcp23s17_init(mcp23s17_t *mcp, SPI_HandleTypeDef *spi, port_t *port, pin_t pin, uint8_t hw_addr) {
  mcp->port = port;
  mcp->pin = pin;
  mcp->hw_addr = hw_addr;
  mcp->spi = spi;

  deselect(mcp);
}
