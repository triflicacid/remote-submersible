#include "7-segment.h"

#include <stdlib.h>

// segment data for digits 0-9 on 7-segment display
static uint8_t segment_data[] = { 0x3F,  0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };

// set all digit pins to the given state (dependent on `display->state_on`)
static void set_digit_pins(display_t *display, bool state) {
  // exit if no display port (case when digits==1)
  if (display->digit_port == NULL) {
    return;
  }

  state = state ? display->state_on : !display->state_on;
  
  for (uint8_t i = 0; i < display->digit_count; i++) {
    write_pin(display->digit_port, display->digit_pins[i], state);
  }
}

// set digit pin to the given state (dependent on `display->state_on`)
static void set_digit_pin(display_t *display, uint8_t segment, bool state) {
  // exit if no display port (case when digits==1)
  if (display->digit_port == NULL) {
    return;
  }

  state = state ? display->state_on : !display->state_on;
  write_pin(display->digit_port, display->digit_pins[segment], state);
}

// turn all segments on/off for all digits
static void set_segment_pins(display_t *display, bool state) {
  // disable all digit pins
  set_digit_pins(display, false);
  
  // set all segments
  for (uint8_t i = 0; i < SEGMENT_COUNT; i++) {
    write_pin(display->segment_port, display->segment_pins[i], state);
  }
  
  write_pin(display->segment_port, display->decimal_pin, state);
  
  // enable all digit pins
  set_digit_pins(display, true);
}

// display given value (0-9) on selected segment(s), and enable/disable decimal point
static void write_segment(display_t *display, uint8_t value, bool dp) {
  if (value > 9) {
    return;
  }
  
  // get segment representation
  uint8_t data = segment_data[value];
  
  // set pins if required by segment data
  for (uint16_t i = 0, k = 0x1; i < SEGMENT_COUNT; i++, k <<= 1) {
    write_pin(display->segment_port, display->segment_pins[i], data & k);
  }
  
  // write decimal point pin
  write_pin(display, display->decimal_pin, dp);
}

void display_init_single(display_t *display, port_t *segment_port, pin_t segment_pins[SEGMENT_COUNT], pin_t decimal_pin, bool is_anode) {
  display_init(display, segment_port, segment_pins, decimal_pin, NULL, 1, NULL, is_anode);				
}

void display_init(display_t *display, port_t *segment_port, pin_t segment_pins[SEGMENT_COUNT], pin_t decimal_pin, port_t *digit_port, uint8_t digit_count, pin_t *digit_pins, bool is_anode) {
  if (digit_count == 0) {
    return;
  }
  
  // configure segment info
  display->segment_port = segment_port;
  display->decimal_pin = decimal_pin;
  
  for (uint8_t i = 0; i < SEGMENT_COUNT; i++) {
    display->segment_pins[i] = segment_pins[i];
  }

  // configure digit port and pins, if necessary
  display->digit_count = digit_count;

  // if only one digit, no digit pins as only one digit to select
  if (digit_count > 1) {
    display->digit_pins = malloc(sizeof(pin_t) * digit_count);

    for (uint16_t i = 0; i < digit_count; i++) {
      display->digit_pins[i] = digit_pins[i];
    }
  } else {
    display->digit_port = NULL;
    display->digit_pins = NULL;
  }
  
  // configure on/off pin states
  // 'on' and 'off' states depend on if display is common anode or cathode
  display->state_on = !is_anode;
}

void display_destroy(display_t *display) {
  // free digit pin buffer?
  if (display->digit_count > 1) {
    free(display->digit_pins);
  }
}

void display_write_digit(display_t *display, uint8_t segment, uint8_t value, bool decimal_point) {
  // disable all digits
  set_digit_pins(display, false);
  
  // write segment LEDs and decimal point
  write_segment(display, value, decimal_point);
  
  // enable desired digit
  set_digit_pin(display, segment, true);
}

void display_write(display_t *display, uint64_t value, uint32_t decimal_points) {
  // iterate, extracting digits and writing to corresponding segment
  for (uint16_t i = 0, k = 0x1; i < display->digit_count; i++, k <<= 1) {
    display_write_digit(display, i, value % 10, decimal_points & k);
    value /= 10;
  }
}

void display_clear(display_t *display) {
  set_segment_pins(display, display->state_off);
}

void display_illuminate(display_t *display) {
  set_segment_pins(display, display->state_on);
}
