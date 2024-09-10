#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

// type holding a tri-state value: `TRISTATE_TRUE`, `TRISTATE_UNDEF`, or `TRISTATE_TRUE`
typedef int8_t tristate_t;

#define TRISTATE_FALSE -1
#define TRISTATE_UNDEF 0
#define TRISTATE_TRUE 1

// describe a pin's exact location
typedef struct {
  GPIO_TypeDef *port;
  uint16_t pin;
} pin_t;

// clamp the first argument between [min, max]
#define CLAMP(value, min, max) \
  if ((value) < (min)) {       \
    (value) = (min);           \
  } else if ((value) > (max)) {\
    (value) = (max);           \
  }

// set (state=true) or reset (state=false) the given pin
inline void write_pin(const pin_t *pin, bool state) {
  HAL_GPIO_WritePin(pin->port, pin->pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

// set a pin high
inline void set_pin(const pin_t *pin) {
  HAL_GPIO_WritePin(pin->port, pin->pin, GPIO_PIN_SET);
}

// set a pin low
inline void reset_pin(const pin_t *pin) {
  HAL_GPIO_WritePin(pin->port, pin->pin, GPIO_PIN_RESET);
}

// toggle the state of a pin
inline void toggle_pin(const pin_t *pin) {
  HAL_GPIO_TogglePin(pin->port, pin->pin);
}

// read the state of the given pin (true=set, false=reset)
inline bool read_pin(const pin_t *pin) {
  return HAL_GPIO_ReadPin(pin->port, pin->pin) == GPIO_PIN_SET;
}

// toggle a reset pin, with given final delay (other delays are 1ms)
void toggle_reset(const pin_t *pin, uint32_t final_delay);

// read a tri-state switch, or other simlar structure
// returns `TRISTATE_FALSE` if the false pin is set, same for the true pin, otherwise `TRISTATE_UNDEF`
tristate_t read_tristate_pins(const pin_t *false_pin, const pin_t *true_pin);

// return if `value` is within `tolerance` of `target`
inline bool is_within_tolerance(int target, int value, int tolerance) {
  int delta = target < value ? value - target : target - value;
  return delta <= tolerance;
}

// map value from range to range
double map_range_decimal(uint32_t value, uint32_t old_min, uint32_t old_max, double new_min, double new_max);

int32_t map_range_int(int32_t value, int32_t old_min, int32_t old_max, int32_t new_min, int32_t new_max);

#endif
