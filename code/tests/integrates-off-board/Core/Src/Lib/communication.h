#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "stored-code.h"
#include "lora.h"
#include "util.h"

// different modules' identification codes
#define RADIO_OFF_BOARD_IDENTIFIER 1
#define RADIO_ON_BOARD_IDENTIFIER 2
#define RADIO_DOCK_IDENTIFIER 3

// [!] this is our identification code!
#define RADIO_IDENTIFIER RADIO_OFF_BOARD_IDENTIFIER

// send with implicit header?
#define RADIO_IMPLICIT_HEADER false

#define OP_PROPELLER 0x1
#define OP_BALLAST 0x5
#define OP_SEND_CODE 0x10
#define OP_REQUEST_CODE 0x11
#define OP_RELEASE_POD 0x20

// header information
typedef struct {
  uint8_t opcode;
  uint8_t sender;
  uint8_t receiver;
} payload_header;

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

typedef void (*propeller_callback_t)(const payload_header *, const propeller_data *);
typedef void (*ballast_callback_t)(const payload_header *, const ballast_data *);
typedef void (*send_code_callback_t)(const payload_header *, const code_data *);
typedef void (*request_code_callback_t)(const payload_header *);
typedef void (*release_pod_callback_t)(const payload_header *);

// register callback for OP_PROPELLER
void register_propeller_callback(propeller_callback_t cb);

// register callback for OP_BALLAST
void register_ballast_callback(ballast_callback_t cb);

// register callback for OP_SEND_CODE
void register_send_code_callback(send_code_callback_t cb);

// register callback for OP_REQUEST_CODE
void register_request_code_callback(request_code_callback_t cb);

// register callback for OP_RELEASE_POD
void register_release_pod_callback(release_pod_callback_t cb);

// return payload size, in bytes, for the given opcode
uint64_t get_payload_size(uint8_t opcode);

// read `size` bytes from LoRa, call on_recv_payload
void on_recv(lora_t *lora, uint8_t recv_size);

// recieve payload; pass header and additional data
void on_recv_payload(const payload_header *header, const void *buffer);

// transmit a lone opcode over LoRa
void transmit_opcode(lora_t *lora, uint8_t opcode, uint8_t recipient);

// transmit an opcode along with a data payload over LoRa
// combined opcode and data size must not exceed LORA_MAX_PAYLOAD_SIZE
void transmit(lora_t *lora, uint8_t opcode, uint8_t recipient, const void *data, uint8_t data_size);

#endif
