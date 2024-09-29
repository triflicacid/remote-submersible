#include "pins.h"

const pin_t
	pin_cs1_2 = { GPIOA, GPIO_PIN_0 },
	pin_cs3_4 = { GPIOA, GPIO_PIN_1 },
	pin_cs_radio = { GPIOA, GPIO_PIN_2 },
	pin_radio_dio = { GPIOA, GPIO_PIN_8 },
	pin_reset = { GPIOA, GPIO_PIN_9 },
	pin_send_code = { GPIOC, GPIO_PIN_10 },
	pin_request_code = { GPIOC, GPIO_PIN_11 },
	pin_release_pod = { GPIOC, GPIO_PIN_12 },
	pin_tristate_up = { GPIOC, GPIO_PIN_9 },
	pin_tristate_down = { GPIOC, GPIO_PIN_8 };
