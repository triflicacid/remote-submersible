#ifndef _7_SEGMENT_H_
#define _7_SEGMENT_H_

#include "../Lib/mcp23s17.h"

// ! segment map: a = bit 0, b = bit 1, ..., g = bit 6, dp = bit 7
// ! unit 0, port a = ones digit, unit 0, port b = tens, unit 1, port a = hundreds, ...

// represent a segment display
typedef struct {
  mcp_t *units[8]; // chip extender units for digit pairs (0 = digits 0 & 1, ...)
  uint8_t digit_count; // number of digits
  bool invert; // invert states? i.e., high is off
} display_t;

// initialise a display, maximum 16 digits
// map: unit 0 gpioa is LSB, unit n gpiob is MSB
void display_init(display_t *display, const mcp_t *units[], uint8_t digit_count, bool inverted);

// control segments of a digit exactly
void display_write_manual(display_t *display, uint8_t segment, uint8_t value);

// display given number to display, provide decimal point bit mask (0x1 = dp1, 0x2 = dp2, 0x4 = dp3, ..., 0x3 = dp1 & dp2, ...)
void display_write(display_t *display, uint64_t value, uint8_t decimal_points);

// display digit (0-9) at given segment position and state if decimal point is on or off
void display_write_digit(display_t *display, uint8_t segment, uint8_t value, bool decimal_point);

// clear a display (turn all segments off)
void display_clear(display_t *display);

#endif
