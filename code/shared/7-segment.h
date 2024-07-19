#ifndef _7_SEGMENT_H_
#define _7_SEGMENT_H_

#include <stdint.h>
#include <stdbool.h>

// number of segments
// constant SHOULD NOT and SHOULD NEVER BE changed
#define SEGMENT_COUNT 7

typedef struct display_t display_t;

// initialise 7-segment display structure with one digit.
// provide port for segment pins, segment pins a-g, decimal pin and if the display is common anode.
// NOTE decimal point pins are from least (ones) to most value.
void display_init_single(display_t *display, GPIO_TypeDef *segment_port, uint16_t segment_pins[SEGMENT_COUNT], uint16_t decimal_pin, bool is_anode);

// initialise 7-segment display structure with arbitrary (> 1) digits.
// provide port for segment pins, segment pins a-g, decimal pin, digit port, number of digits, digit pins,
// and if the display is common anode.
// NOTE digit and decimal point pins are from least (ones) to most value.
void display_init(display_t *display, GPIO_TypeDef *segment_port, uint16_t segment_pins[SEGMENT_COUNT], uint16_t decimal_pin, GPIO_TypeDef *digit_port, uint8_t digit_count, uint16_t *digit_pins, bool is_anode);

// destroy display (does not free pointer)
void display_destroy(display_t *display);

// display given number to display, provide decimal point bit pattern (0x1 = dp1, 0x2 = dp2, 0x4 = dp3, ...)
void display_write(display_t *display, uint64_t value, uint32_t decimal_points);

// display digit (0-9) at given segment position and state if decimal point is on or off
void display_write_digit(display_t *display, uint8_t segment, uint8_t value, bool decimal_point);

// clear the display
void display_clear(display_t *display);

// illuminate the display (all on)
void display_illuminate(display_t *display);

#endif
