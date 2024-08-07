#ifndef _ACTIONS_H_
#define _ACTIONS_H_

#include "shared/communication.h"

#include <stdint.h>

// trigger: push request-code button
void action_request_code(void);

// trigger: RxDone signal from LoRa SPI -- received OPCODE, receive remaining payload data as required
void action_rx_opcode(void);

// trigger: receive OP_SEND_CODE
void recv_send_code(code_data *data);

#endif
