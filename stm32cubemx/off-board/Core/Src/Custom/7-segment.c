#include "7-segment.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// a = bit 0, g = bit 6
static uint8_t segment_mask[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };
static uint8_t dp_mask = 1 << 7;

void display_init(display_t *display, mcp23s17_t *units, uint8_t digit_count) {
  uint8_t unit_count = (digit_count + 1) / 2;

  display->units = malloc(sizeof(*units) * unit_count);
  memcpy(display->units, units, sizeof(*units) * unit_count);
}

void display_write_manual(display_t *display, uint8_t segment, uint8_t value) {
  if (segment >= display->digit_count) {
    return;
  }

  // get unit location and determine port (for register address)
  mcp23s17_t *unit = display->units + (segment / 2);
  uint8_t address = segment % 2 ? MCP_GPIOB : MCP_GPIOA;

  mcp23s17_write(unit, address, value);
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

  display_write_manual(display, segment, ~data);
}

void display_clear(display_t *display) {
  for (uint16_t i = 0; i < display->digit_count; i++) {
    display_write_manual(display, i, 0xff);
  }
}
