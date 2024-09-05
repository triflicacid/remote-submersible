#include "util.h"

void write_pin(port_t *port, pin_t pin, bool state) {
  HAL_GPIO_WritePin(port, pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

bool read_pin(port_t *port, pin_t pin) {
  return HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET;
}

tristate_t read_tristate_pins(port_t *false_port, pin_t false_pin, port_t *true_port, pin_t true_pin) {
  if (read_pin(false_port, false_pin)) {
    return TRISTATE_FALSE;
  } else if (read_pin(true_port, true_pin)) {
    return TRISTATE_TRUE;
  } else {
    return TRISTATE_UNDEF;
  }
}

tristate_t tristate_transition(tristate_t old_state, tristate_t transition_state) {
	switch (old_state) {
    case TRISTATE_FALSE:
      if (transition_state == TRISTATE_TRUE) {
        return TRISTATE_UNDEF;
      }
      break;
    case TRISTATE_UNDEF:
      if (transition_state != TRISTATE_UNDEF) {
        return transition_state;
      }
      break;
    case TRISTATE_TRUE:
      if (transition_state == TRISTATE_FALSE) {
        return TRISTATE_UNDEF;
      }
      break;
    default: ;
  }

  // no change
  return old_state;
}

double map_range(uint32_t value, uint32_t old_min, uint32_t old_max, double new_min, double new_max) {
  double percentage = (double) (value - old_min) / (old_max - old_min);
  return percentage * (new_max - new_min) + new_min;
}
