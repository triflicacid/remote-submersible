#include "actions.h"
#include "constants.h"
#include "globals.h"
#include "../Lib/util.h"

void action_request_code(void) {
  transmit_opcode(&g_lora, OP_REQUEST_CODE);
}

void action_rx_opcode(void) {
  lora_rx_point_next_packet(&g_lora);

  // get opcode from buffer
  opcode_t opcode = g_lora_buffer[0];

  // get expected payload size; receive if necessary
  uint64_t size = get_payload_size(opcode);

  if (size > 0) {
    lora_mode_rx(&g_lora, false);
    lora_receive(&g_lora, g_lora_buffer, size);
  }

  // clear IRQ flags
  lora_irq_clear(&g_lora);

  // deliver payload
  on_recv_payload(opcode, g_lora_buffer);

  // reset FIFO
  lora_rx_reset_buffer(&g_lora);

  // go back to listening on RX
  lora_mode_rx(&g_lora, false);
  lora_receive_async(&g_lora, g_lora_buffer, sizeof(opcode_t));
}

void recv_send_code(code_data *data) {
  tm1637_display(&g_display, data->code, false);
}
