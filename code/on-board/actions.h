#ifndef _ACTIONS_H_
#define _ACTIONS_H_

#include "shared/communication.h"

// trigger: receive OP_SEND_CODE
void recv_send_code(code_data *data);

// trigger: receive OP_REQUEST_CODE
void recv_request_code(void);

// trigger: receive OP_RELEASE_POD
void recv_release_pod(void);

// trigger: RxDone signal from LoRa SPI
void action_rx_done(void);

#endif
