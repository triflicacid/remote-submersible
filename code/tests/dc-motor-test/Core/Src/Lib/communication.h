#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

// ensure global LoRa buffer is defined
#define LORA_PROVIDE_GLOBAL_BUFFER

#include "stored-code.h"
#include "lora.h"
#include "util.h"

typedef uint8_t opcode_t;

#define OP_PROPELLER 0x1
#define OP_BALLAST 0x5
#define OP_SEND_CODE 0x10
#define OP_REQUEST_CODE 0x11
#define OP_RELEASE_POD 0x20

// top-level structure of a command packet
typedef struct {
  opcode_t opcode;  // opcode to determine payload reason
  void *data;  // capture trailing data, may be NULL for certain opcodes
} payload;

// payload data for OP_PROPELLER
typedef struct {
  double x;  // secondary propeller, in [-1, 1]
  double y;  // primary propeller, in [-1, 1]
} propeller_data;

// payload data for OP_BALLAST
typedef struct {
  tristate_t mode;  // -1 descend; 0 hover; 1 ascend
} ballast_data;

// payload data for OP_SEND_CODE
typedef struct {
  code_t code;
} code_data;

typedef void (*propeller_callback_t)(propeller_data *);
typedef void (*ballast_callback_t)(ballast_data *);
typedef void (*send_code_callback_t)(code_data *);
typedef void (*request_code_callback_t)(void);
typedef void (*release_pod_callback_t)(void);

// register callback for OP_PROPELLER
void register_propeller_callback(propeller_callback_t cb);

// register callback for OP_BALLAST
void register_ballast_callback(ballast_callback_t cb);

// register callback for OP_SEND_CODE
void register_send_code_callback(send_code_callback_t cb);

// register callback for OP_REQUEST_CODE
void register_request_code_callback(request_code_callback_t cb);

// register rcallback for OP_RELEASE_POD
void register_release_pod_callback(release_pod_callback_t cb);

// return payload size, in bytes, for the given opcode
uint64_t get_payload_size(uint8_t opcode);

// recieve payload; given buffer of received data, invoke callback with data
void on_recv_payload(opcode_t opcode, void *buffer);

// transmit a lone opcode over LoRa
void transmit_opcode(lora_t *lora, opcode_t opcode);

// transmit an opcode along with a data payload over LoRa
// combined opcode and data size must not exceed LORA_MAX_PAYLOAD_SIZE
void transmit(lora_t *lora, opcode_t opcode, void *data, uint8_t data_size);

#endif
