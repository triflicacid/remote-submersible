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
