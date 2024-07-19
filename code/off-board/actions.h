#ifndef _ACTIONS_H_
#define _ACTIONS_H_

#include "shared/communication.h"

#include <stdint.h>

// trigger: joystick change
// receive raw ADC values
void action_propeller(uint32_t x, uint32_t y);

// trigger: tri-state switch change
void action_ballast(void);

// trigger: push send-code button
void action_send_code(void);

// trigger: push request-code button
void action_request_code(void);

// trigger: push release-pod button
void action_release_pod(void);

// trigger: RxDone signal from LoRa SPI
void action_rx_done(void);

// trigger: receive OP_SEND_CODE
void recv_send_code(code_data *data);

#endif
