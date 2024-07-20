#include "tri-state-switch.h"

int8_t tri_state_switch_read(GPIO_TypeDef *down_port, uint16_t down_pin, GPIO_TypeDef *up_port, uint16_t up_pin) {
  if (HAL_GPIO_ReadPin(down_port, down_pin) == GPIO_PIN_SET) {
    return TRISTATE_DOWN;
  }

  if (HAL_GPIO_ReadPin(up_port, up_pin) == GPIO_PIN_SET) {
    return TRISTATE_UP;
  }

  return TRISTATE_MID;
}
