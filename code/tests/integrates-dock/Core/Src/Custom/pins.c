#include "pins.h"

const pin_t
	pin_cs_radio = { GPIOA, GPIO_PIN_2 },
	pin_radio_dio = { GPIOA, GPIO_PIN_8 },
	pin_reset = { GPIOA, GPIO_PIN_9 },
	pin_request_code = { GPIOC, GPIO_PIN_13 },
	pin_tm1637_clk = { GPIOA, GPIO_PIN_1 },
	pin_tm1637_data = { GPIOA, GPIO_PIN_0 },
	pin_led = { GPIOA, GPIO_PIN_5 };
