#ifndef _PINS_H_
#define _PINS_H_

#include "../Lib/util.h"

extern const pin_t
// chip select for display digits 1, 2
	pin_cs1_2,
	// chip select for display digits 3, 4
	pin_cs3_4,
	// chip select for radio
	pin_cs_radio,
	// radio DIO pin
	pin_radio_dio,
	// reset for display and radio
	pin_reset,
	// send_code button
	pin_send_code,
	// request code button
	pin_request_code,
	// release escape pod button
	pin_release_pod,
	// tristate switch's UP position
	pin_tristate_up,
	// tristate switch's DOWN position
	pin_tristate_down;

#endif
