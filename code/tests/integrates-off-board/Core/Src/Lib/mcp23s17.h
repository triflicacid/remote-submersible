#ifndef _MCP23S17_H_
#define _MCP23S17_H_

#include "util.h"

#define MCP_IODIRA 0x00
#define MCP_IODIRB 0x01
#define MCP_IOCONA 0x0A
#define MCP_GPPUA  0x0C
#define MCP_GPPUB  0x0D
#define MCP_GPIOA  0x12
#define MCP_GPIOB  0x13

#define MCP_PIN_0 0x01
#define MCP_PIN_1 0x02
#define MCP_PIN_2 0x04
#define MCP_PIN_3 0x08
#define MCP_PIN_4 0x10
#define MCP_PIN_5 0x20
#define MCP_PIN_6 0x40
#define MCP_PIN_7 0x80

#define MCP_TIMEOUT 1000

typedef struct {
  const pin_t *cs;
  uint8_t hw_addr; // hardware address
  SPI_HandleTypeDef *spi;
  const pin_t *rst;
} mcp_t;

// setup MCP struct
void mcp_init(mcp_t *mcp, SPI_HandleTypeDef *spi, const pin_t *cs, uint8_t hw_addr, const pin_t *rst);

// write data to address
// sets pins in mask, resets others
void mcp_write(mcp_t *mcp, uint8_t address, uint8_t data);

// read data in address
uint8_t mcp_read(mcp_t *mcp, uint8_t address);

// toggle reset pin on device
void mcp_reset(mcp_t *mcp);

#endif
