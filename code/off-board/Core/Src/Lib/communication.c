#include "communication.h"

// call `f` if not NULL, with the given arg
#define CALL_FN1(F, ARG) \
  if (F)                 \
    (F)(ARG);

// call `f` if not NULL, with the given args
#define CALL_FN2(F, ARG1, ARG2) \
  if (F)                        \
    (F)(ARG1, ARG2);

// buffer for packet storage
static uint8_t _buffer[255];

// given header fields, write using lora_write
static void write_header(lora_t *lora, uint8_t opcode, uint8_t sender, uint8_t recipient) {
  uint8_t hdr = ((opcode & 0xf) << 4) | ((sender & 0x3) << 2) | (recipient & 0x3);
  lora_write(lora, hdr);
}

// given buffer to start of header, return payload header (increments buffer pointer)
static payload_header read_header(const uint8_t *buffer) {
  payload_header hdr;
  hdr.receiver = *buffer & 0x3;
  hdr.sender = (*buffer >> 2) & 0x3;
  hdr.opcode = (*buffer >> 4) & 0xf;

  return hdr;
}

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

uint64_t get_payload_size(uint8_t opcode) {
  switch (opcode) {
    case OP_PROPELLER:
      return sizeof(propeller_data);
    case OP_BALLAST:
      return sizeof(ballast_data);
    case OP_SEND_CODE:
      return sizeof(code_data);
    default:
      return 0;
  }
}

void on_recv(lora_t *lora, uint8_t recv_size) {
  // read bytes
  for (uint16_t i = 0; i < recv_size; i++) {
    _buffer[i] = lora_read(lora);
  }

  // invoke handler
  payload_header hdr = read_header(_buffer);
  on_recv_payload(&hdr, _buffer + PAYLOAD_SIZE);
}

void on_recv_payload(const payload_header *header, const void *buffer) {
  // ensure packet is addressed to us
  if (header->receiver != RADIO_IDENTIFIER) {
	  return;
  }

  // action depends on opcode...
  switch (header->opcode) {
    case OP_PROPELLER:
      CALL_FN2(propeller_cb, header, buffer)
      break;
    case OP_BALLAST:
      CALL_FN2(ballast_cb, header, buffer)
      break;
    case OP_SEND_CODE:
      CALL_FN2(send_code_cb, header, buffer)
      break;
    case OP_REQUEST_CODE:
      CALL_FN1(request_code_cb, header)
      break;
    case OP_RELEASE_POD:
      CALL_FN1(release_pod_cb, header)
      break;
  }
}

uint32_t queued_transmits = 0;
uint32_t transmits = 0;

void transmit_opcode(lora_t *lora, uint8_t opcode, uint8_t recipient) {
	queued_transmits++;
  lora_begin_packet(lora, RADIO_IMPLICIT_HEADER);
  write_header(lora, opcode, RADIO_IDENTIFIER, recipient);
  lora_end_packet(lora, false);
  transmits++;
}

void transmit(lora_t *lora, uint8_t opcode, uint8_t recipient, const void *data, uint8_t data_size) {
	queued_transmits++;
  lora_begin_packet(lora, RADIO_IMPLICIT_HEADER);
  write_header(lora, opcode, RADIO_IDENTIFIER, recipient);
  lora_write_bytes(lora, data, data_size);
  lora_end_packet(lora, false);
  transmits++;
}
