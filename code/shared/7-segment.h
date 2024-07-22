#ifndef _7_SEGMENT_H_
#define _7_SEGMENT_H_

#include "util.h"

// define number of segments in a digit
// constant SHOULD NOT and SHOULD NEVER BE changed
#define SEGMENT_COUNT 7

// structure to represent an aritrary-digit 7-segment display
// !DO NOT ACCESS MANUALLY
typedef struct {
  bool enabled;

  port_t *segment_port; // port for a-g segments
  pin_t segment_pins[SEGMENT_COUNT]; // a-g segment pins
  pin_t decimal_pin; // decimal point pin
  bool state_on; // which Boolean is used to turn a segment on?

  port_t *digit_port; // port for digits
  uint8_t digit_count; // number of supporting digits
  pin_t *digit_pins; // array of `digit_count` digit pins

  uint8_t *digits; // array of digits 0-9 to display at respective position (least to most)
  uint32_t dp_mask; // decimal point mask
  uint8_t pos; // display position, index into `digits`
} display_t;

// initialise 7-segment display structure with arbitrary (> 1) digits in DISABLED mode
// provide port for segment pins, pin numbers a-g, decimal pin, digit port, number of digits, digit pins, and if the display is common anode
// NOTE digit and decimal point pins are from least (ones) to most value
void display_init(display_t *display, port_t *segment_port, pin_t segment_pins[SEGMENT_COUNT], pin_t decimal_pin, port_t *digit_port, uint8_t digit_count, pin_t *digit_pins, bool is_anode);

// same as `display_init`, but for a display with only one digit
void display_init_single(display_t *display, port_t *segment_port, pin_t segment_pins[SEGMENT_COUNT], pin_t decimal_pin, bool is_anode);

// destroy display (does not free pointer)
void display_destroy(display_t *display);

// reset the display (same config as if just init)
void display_reset(display_t *display);

// enable/disable display (resets cycle pos)
// immediately disables all segments if enable=false
void display_enable(display_t *display, bool enable);

// display given number to display, provide decimal point bit mask (0x1 = dp1, 0x2 = dp2, 0x4 = dp3, ..., 0x3 = dp1 & dp2, ...)
void display_write(display_t *display, uint64_t value, uint32_t decimal_points);

// display digit (0-9) at given segment position and state if decimal point is on or off
void display_write_digit(display_t *display, uint8_t segment, uint8_t value, bool decimal_point);

// cycle display: display next digit
void display_cycle(display_t *display);

#endif
