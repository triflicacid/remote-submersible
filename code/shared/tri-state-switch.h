#ifndef _TRI_STATE_SWITCH_H_
#define _TRI_STATE_SWITCH_H_

#define TRISTATE_DOWN -1
#define TRISTATE_MID 0
#define TRISTATE_UP 1

#include <stdint.h>

// read tri-state switch, returning one of `TRISTATE_*`
int8_t tri_state_switch_read(GPIO_TypeDef *down_port, uint16_t down_pin, GPIO_TypeDef *up_port, uint16_t up_pin);

#endif
