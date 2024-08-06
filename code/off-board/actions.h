#ifndef _ACTIONS_H_
#define _ACTIONS_H_

#include "shared/communication.h"

#include <stdint.h>

// trigger: joystick change
void action_propeller(void);

// trigger: tri-state switch change
void action_ballast(void);

// trigger: push send-code button
void action_send_code(void);

// trigger: push request-code button
void action_request_code(void);

// trigger: push release-pod button
void action_release_pod(void);

// trigger: RxDone signal from LoRa SPI -- received OPCODE, receive remaining payload data as required
void action_rx_opcode(void);

// trigger: receive OP_SEND_CODE
void recv_send_code(code_data *data);

// trigger: timer (TIMER_DEPTH_HANDLE)
void action_predict_depth_tick(void);

// trigger: timer (TIMER_DEPTH_HANDLE)
void action_display_movement_tick(void);

#endif
