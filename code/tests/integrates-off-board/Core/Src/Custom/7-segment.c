#include "7-segment.h"

#include <stdlib.h>
#include <stdint.h>

// a = bit 0, g = bit 6
static uint8_t segment_mask[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };
static uint8_t segment_mask_flipped[] = { 0x7E, 0x30, /**/ 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };
static uint8_t dp_mask = 1 << 7;

void display_init(display_t *display, const mcp_t *units[], uint8_t digit_count, bool inverted) {
  display->invert = inverted;
  display->digit_count = digit_count;
  uint8_t unit_count = (digit_count + 1) / 2;
  uint8_t initial = inverted ? 0xff : 0x00;


  for (uint8_t i = 0; i < unit_count; ++i) {
    display->units[i] = units[i];

    // set both ports to output mode and reset states
    mcp_write(units[i], MCP_IODIRA, 0x00);
    mcp_write(units[i], MCP_GPIOA, initial);

    mcp_write(units[i], MCP_IODIRB, 0x00);
    mcp_write(units[i], MCP_GPIOB, initial);
  }
}

void display_write_manual(display_t *display, uint8_t segment, uint8_t value) {
  if (segment >= display->digit_count) {
    return;
  }

  // get unit location and determine port (for register address)
  mcp_t *unit = display->units[display->digit_count / 2 - 1 - segment / 2];
  uint8_t address = segment % 2 ? MCP_GPIOA : MCP_GPIOB;

  mcp_write(unit, address, value);
}

void display_write(display_t *display, uint64_t value, uint8_t decimal_points) {
  // iterate, extracting and writing digits
  for (uint16_t i = 0, k = 1; i < display->digit_count; i++, k <<= 1) {
    display_write_digit(display, i, value % 10, decimal_points & k);
    value /= 10;
  }
}

void display_write_digit(display_t *display, uint8_t segment, uint8_t value, bool decimal_point) {
  if (value > 9) {
    return;
  }

  // calculate register data 
  uint8_t data = segment_mask[value];

  if (decimal_point) {
    data |= dp_mask;
  }

  display_write_manual(display, segment, display->invert ? ~data : data);
}

void display_clear(display_t *display) {
  uint8_t data = display->invert ? 0xff : 0x00;

  for (uint16_t i = 0; i < display->digit_count; i++) {
    display_write_manual(display, i, data);
  }
}
