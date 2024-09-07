#ifndef _PINS_H_
#define _PINS_H_

#include "../Lib/util.h"

extern const pin_t
	// chip select for radio
	pin_cs_radio,
	// radio DIO pin
	pin_radio_dio,
	// reset for radio
	pin_reset,
	// request code button
	pin_request_code,
	// TIM1637 clock pin
	pin_tm1637_clk,
	// TIM1637 data pin
	pin_tm1637_data,
	// misc LED
	pin_led;

#endif
