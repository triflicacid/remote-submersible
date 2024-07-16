#include "communication.h"

#include <stdlib.h>
#include <string.h>

// macro - call `fn` with `arg` if not null
#define FN_GUARD(fn, arg) if (fn) fn(arg);

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

void receive_communication(payload *payload) {
	// action depends on opcode...
	switch (payload->opcode) {
		case OP_PROPELLER:
			FN_GUARD(propeller_cb, (propeller_data *) payload->data)
			break;
		case OP_BALLAST:
			FN_GUARD(ballast_cb, (ballast_data *) payload->data)
			break;
		case OP_SEND_CODE:
			FN_GUARD(send_code_cb, (code_data *) payload->data)
			break;
		case OP_REQUEST_CODE:
			FN_GUARD(request_code_cb,)
			break;
		case OP_RELEASE_POD:
			FN_GUARD(release_pod_cb,)
			break;
	}
}

void transmit_opcode(lora_t *lora, opcode_t opcode) {
	uint8_t buffer = opcode;
	lora_send(lora, &opcode, 1);
}

void transmit(lora_t *lora, opcode_t opcode, void *data, uint16_t data_size) {
	// create buffer to fit opcode + data
	uint8_t buffer_size = sizeof(opcode) + data_size;
	uint8_t *buffer = malloc(buffer_size);

	// copy opcode into buffer
	buffer[0] = opcode;
	// use following approaches if opcode_t changes from 8-bit value
	// *(opcode_t *) buffer = opcode;
	// memcpy(buffer, &opcode, sizeof(opcode))

	// copy data into buffer
	memcpy(buffer + sizeof(opcode), data, data_size);

	// transmit data
	lora_send(lora, buffer, buffer_size);

	// release buffer
	free(buffer);
}
