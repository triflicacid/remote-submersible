#ifndef _ACTIONS_H_
#define _ACTIONS_H_

#include "../Lib/communication.h"

#include <stdint.h>

// trigger: push request-code button
void action_request_code(void);

// trigger: receive OP_SEND_CODE
void recv_send_code(const payload_header *hdr, const code_data *data);

#endif
