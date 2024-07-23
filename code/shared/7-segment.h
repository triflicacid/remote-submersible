#ifndef _7_SEGMENT_H_
#define _7_SEGMENT_H_

#include "util.h"

// ! segment map: a = bit 0, b = bit 1, ..., g = bit 6, dp = bit 7
// ! unit 0, port a = ones digit, unit 0, port b = tens, unit 1, port a = hundreds, ...

// represent a segment display
typedef struct {
  SPI_HandleTypeDef *spi;
  ploc_t *units; // map slave-select pins of designated unit pairs (0&1, 2&3, ...)
  uint8_t digit_count; // number of digits
  uint8_t hw_addr; // hardware address
} display_t;

// initialise a display
void display_init(display_t *display, SPI_HandleTypeDef *spi, ploc_t *units, uint8_t digit_count);

// display given number to display, provide decimal point bit mask (0x1 = dp1, 0x2 = dp2, 0x4 = dp3, ..., 0x3 = dp1 & dp2, ...)
void display_write(display_t *display, uint64_t value, uint32_t decimal_points);

// display digit (0-9) at given segment position and state if decimal point is on or off
void display_write_digit(display_t *display, uint8_t segment, uint8_t value, bool decimal_point);

#endif
