#ifndef _ACTIONS_H_
#define _ACTIONS_H_

#include "../Lib/communication.h"

// trigger: receive OP_PROPELLER
void recv_propeller(const payload_header *hdr, const propeller_data *data);

// trigger: receive OP_BALLAST
void recv_ballast(const payload_header *hdr, const ballast_data *data);

// trigger: receive OP_SEND_CODE
void recv_send_code(const payload_header *hdr, const code_data *data);

// trigger: receive OP_REQUEST_CODE
void recv_request_code(const payload_header *hdr);

// trigger: receive OP_RELEASE_POD
void recv_release_pod(const payload_header *hdr);

#endif
