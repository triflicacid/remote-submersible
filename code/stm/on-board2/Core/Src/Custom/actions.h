#ifndef _ACTIONS_H_
#define _ACTIONS_H_

#include "../Lib/communication.h"

// trigger: receive OP_PROPELLER
void recv_propeller(propeller_data *data);

// trigger: receive OP_BALLAST
void recv_ballast(ballast_data *data);

// trigger: receive OP_SEND_CODE
void recv_send_code(code_data *data);

// trigger: receive OP_REQUEST_CODE
void recv_request_code(void);

// trigger: receive OP_RELEASE_POD
void recv_release_pod(void);

// trigger: RxDone signal from LoRa SPI -- received OPCODE, receive remaining payload data as required
void action_rx_opcode(void);

#endif
