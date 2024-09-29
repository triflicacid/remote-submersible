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
	// stepper motor pins
	pin_stepper_black, pin_stepper_blue, pin_stepper_green, pin_stepper_red,
	// dc motor in pins
	pins_dc_in[4],
	// dc motor enable pins
	pins_dc_enable[2],
	// electromagnet
	pin_electromagnet;

#endif
