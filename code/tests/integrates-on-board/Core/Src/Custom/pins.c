#include "pins.h"

const pin_t
	pin_cs_radio = { GPIOB, GPIO_PIN_12 },
	pin_radio_dio = { GPIOB, GPIO_PIN_1 },
	pin_reset = { GPIOB, GPIO_PIN_0 },
	pin_stepper_black = { GPIOC, GPIO_PIN_0 },
	pin_stepper_blue = { GPIOC, GPIO_PIN_1 },
	pin_stepper_green = { GPIOC, GPIO_PIN_2 },
	pin_stepper_red = { GPIOC, GPIO_PIN_3 },
	pins_dc_in[4] = {
			{ GPIOA, GPIO_PIN_0 },
			{ GPIOA, GPIO_PIN_1 },
			{ GPIOA, GPIO_PIN_2 },
			{ GPIOA, GPIO_PIN_3 }
	},
	pins_dc_enable[2] = {
			{ GPIOA, GPIO_PIN_5 },
			{ GPIOA, GPIO_PIN_10 }
	},
	pin_electromagnet = { GPIOB, GPIO_PIN_3 };
