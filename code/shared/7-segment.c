#include "7-segment.h"

#include <stdlib.h>

struct display_t {
  GPIO_TypeDef *segment_port; // port for a-g segments
  uint16_t segment_pins[SEGMENT_COUNT]; // a-g segment pins
  uint16_t decimal_pin; // decimal point pin
  
  GPIO_PinState state_on;
  GPIO_PinState state_off;
  
  GPIO_TypeDef *digit_port; // port for digits
  uint8_t digit_count; // number of supporting digits
  uint16_t *digit_pins; // array of `digit_count` digit pins
};

// segment data dor digits 0-9 on 7-segment display
static uint8_t segment_data[] = {
  0x3F, // 0
  0x06, // 1
  0x5B, // 2
  0x4F, // 3
  0x66, // 4
  0x6D, // 5
  0x7D, // 6
  0x07, // 7
  0x7F, // 8
  0x6F  // 9
};

// set all digit pins to the given state
static void set_digit_pins(display_t *display, GPIO_PinState state) {
  // exit if no display port (case when digits==1)
  if (display->digit_port == NULL) {
    return;
  }
  
  for (uint8_t i = 0; i < display->digit_count; i++) {
    HAL_GPIO_WritePin(display->digit_port, display->digit_pins[i], state);
  }
}

// set digit pin to the given state
static void set_digit_pin(display_t *display, uint8_t segment, GPIO_PinState state) {
  // exit if no display port (case when digits==1)
  if (display->digit_port == NULL) {
    return;
  }
  
  HAL_GPIO_WritePin(display->digit_port, display->digit_pins[segment], state);
}

// turn all segments on/off for all digits
static void set_segment_pins(display_t *display, GPIO_PinState state) {
  // disable all digit pins
  set_digit_pins(display, display->state_off);
  
  // disable all segments
  for (uint8_t i = 0; i < SEGMENT_COUNT; i++) {
    HAL_GPIO_WritePin(display->segment_port, display->segment_pins[i], state);
  }
  
  // disable decimal point
  HAL_GPIO_WritePin(display->segment_port, display->decimal_pin, state);
  
  // enable all digit pins
  set_digit_pins(display, display->state_on);
}

// display given value (0-9) on selected segment(s), and enable/disable decimal point
static void write_segment(display_t *display, uint8_t value, bool dp) {
  if (value > 9) {
    return;
  }
  
  // get segment representation
  uint8_t data = segment_data[value];
  
  // set pins if required by segment data
  for (uint8_t i = 0, k = 0x1; i < SEGMENT_COUNT; i++, k <<= 1) {
    HAL_GPIO_WritePin(display->segment_port, display->segment_pins[i], (data & k) ? display->state_on : display->state_off);
  }
  
  // write decimal point pin
  HAL_GPIO_WritePin(display, display->decimal_pin, dp ? display->state_on : display->state_off);
}

void display_init_single(display_t *display, GPIO_TypeDef *segment_port, uint16_t segment_pins[SEGMENT_COUNT], uint16_t decimal_pin, bool is_anode) {
  display_init(display, segment_port, segment_pins, decimal_pin, NULL, 1, NULL, is_anode);				
}

void display_init(display_t *display, GPIO_TypeDef *segment_port, uint16_t segment_pins[SEGMENT_COUNT], uint16_t decimal_pin, GPIO_TypeDef *digit_port, uint8_t digit_count, uint16_t *digit_pins, bool is_anode) {
  if (digit_count == 0) {
    return;
  }
  
  // configure segment info
  display->segment_port = segment_port;
  display->decimal_pin = decimal_pin;
  
  for (uint8_t i = 0; i < SEGMENT_COUNT; i++) {
    display->segment_pins[i] = segment_pins[i];
  }

  // configure digital ports
  display->digit_port = digit_port;
  display->digit_pins = malloc(sizeof(uint16_t) * digit_count);

  for (uint16_t i = 0; i < digit_count; i++) {
    display->digit_pins[i] = digit_pins[i];
  }
  
  // configure on/off pin states
  // 'on' and 'off' states depend on if display is common anode or cathode
  display->state_on  = is_anode ? GPIO_PIN_RESET : GPIO_PIN_SET;
  display->state_off = is_anode ? GPIO_PIN_SET   : GPIO_PIN_RESET;
}

void display_write_digit(display_t *display, uint8_t segment, uint8_t value, bool decimal_point) {
  // disable all digits
  set_digit_pins(display, display->state_off);
  
  // write segment LEDs and decimal point
  write_segment(display, value, decimal_point);
  
  // enable desired digit
  set_digit_pin(display, segment, display->state_on);
}

void display_write(display_t *display, uint64_t value, uint32_t decimal_points) {
  // bit mask for determining if decimal point is enabled
  uint32_t k = 0x1;
  
  // iterate, extracting digits and writing to corresponding segment
  for (uint8_t i = 0; i < display->digit_count; i++) {
    display_write_digit(display, i, value % 10, decimal_points & k);
    k <<= 1;
    value /= 10;
  }
}

void display_clear(display_t *display) {
  set_segment_pins(display, display->state_off);
}

void display_illuminate(display_t *display) {
  set_segment_pins(display, display->state_on);
}
