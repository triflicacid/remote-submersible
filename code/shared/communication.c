#include "communication.h"

#include <string.h>

// call `cb` if not NULL
#define CALL_FN(f) \
	if (f)           \
		f();

// call `f` if not NULL, with `payload->data` interpreted as the given type `t`, providing `t *` as an argument
#define CALL_FN_ARG(f, t) \
	if (f)                  \
		f((t *)((uint8_t *)payload + sizeof(opcode_t)));

// store address of operation callbacks
static propeller_callback_t propeller_cb = NULL;
static ballast_callback_t ballast_cb = NULL;
static send_code_callback_t send_code_cb = NULL;
static request_code_callback_t request_code_cb = NULL;
static release_pod_callback_t release_pod_cb = NULL;

void register_propeller_callback(propeller_callback_t cb) {
	propeller_cb = cb;
}

void register_ballast_callback(ballast_callback_t cb) {
	ballast_cb = cb;
}

void register_send_code_callback(send_code_callback_t cb) {
	send_code_cb = cb;
}

void register_request_code_callback(request_code_callback_t cb) {
	request_code_cb = cb;
}

void register_release_pod_callback(release_pod_callback_t cb) {
	release_pod_cb = cb;
}

void on_recv_payload(payload *payload) {
	// action depends on opcode...
	switch (payload->opcode) {
		case OP_PROPELLER:
			CALL_FN_ARG(propeller_cb, propeller_data)
			break;
		case OP_BALLAST:
			CALL_FN_ARG(ballast_cb, ballast_data)
			break;
		case OP_SEND_CODE:
			CALL_FN_ARG(send_code_cb, code_data)
			break;
		case OP_REQUEST_CODE:
			CALL_FN(request_code_cb)
			break;
		case OP_RELEASE_POD:
			CALL_FN(release_pod_cb)
			break;
	}
}

void transmit_opcode(lora_t *lora, opcode_t opcode) {
	lora_send(lora, &opcode, sizeof(opcode));
}

void transmit(lora_t *lora, opcode_t opcode, void *data, uint8_t data_size) {
	// check if too large
	if (sizeof(opcode) + data_size > LORA_MAX_PAYLOAD_SIZE) {
		return;
	}

	// load opcode into buffer
	g_lora_buffer[0] = opcode;

	// copy data into buffer
	memcpy(g_lora_buffer + sizeof(opcode), data, data_size);

	// transmit data
	lora_send(lora, g_lora_buffer, sizeof(opcode) + data_size);
}
