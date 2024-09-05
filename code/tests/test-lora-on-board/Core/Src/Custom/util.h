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

typedef GPIO_TypeDef port_t;
typedef uint16_t pin_t;

// describe a pin's exact location
typedef struct {
  port_t *port;
  pin_t pin;
} ploc_t;

// clamp the first argument between [min, max]
#define CLAMP(value, min, max) \
  if ((value) < (min)) {       \
    (value) = (min);           \
  } else if ((value) > (max)) {\
    (value) = (max);           \
  }

// set (state=true) or reset (state=false) the given pin
void write_pin(port_t *port, pin_t pin, bool state);

// read the state of the given pin (true=set, false=reset)
bool read_pin(port_t *port, pin_t pin);

// toggle a pin's state
void toggle_pin(port_t *port, pin_t pin);

// read a tri-state switch, or other simlar structure
// returns `TRISTATE_FALSE` if the false pin is set, same for the true pin, otherwise `TRISTATE_UNDEF`
tristate_t read_tristate_pins(port_t *false_port, pin_t false_pin, port_t *true_port, pin_t true_pin);

// calculate new tri-state given a transition from an old state
// e.g., "up" maps to "middle" via a "down" transition
tristate_t tristate_transition(tristate_t old_state, tristate_t transition_state);

// map value from range to range
double map_range(uint32_t value, uint32_t old_min, uint32_t old_max, double new_min, double new_max);

#endif
