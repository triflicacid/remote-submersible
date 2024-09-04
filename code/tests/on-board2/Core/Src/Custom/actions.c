#include "actions.h"
#include "constants.h"
#include "globals.h"
#include "../Lib/stored-code.h"

void recv_propeller(propeller_data *data) {
  // update the velocity of both propeller motors
  dc_motor_set_velocity(&g_primary_motor, data->y);
  dc_motor_set_velocity(&g_secondary_motor, data->x);
}

void recv_ballast(ballast_data *data) {
  // update stepper motor event
  stepper_event_update(&g_ballast, data->mode);

  // if action is to be taken, start clock
  if (!stepper_event_done(&g_ballast)) {
    HAL_TIM_Base_Start_IT(&TIMER_STEPPER_HANDLE);
  }
}

void recv_send_code(code_data *data) {
  // cache code, we need to send it later
  save_code(data->code);
}

void recv_request_code(void) {
  // respond with our cached code
  code_data data = { fetch_code() };
  transmit(&g_lora, OP_SEND_CODE, &data, sizeof(data));
}

void recv_release_pod(void) {
  // de-activate electromagnet
  write_pin(ELECTROMAGNET_PORT, ELECTROMAGNET_PIN, true);

  // start electromagnet timer
  HAL_TIM_Base_Start_IT(&TIMER_ELECTROMAGNET_HANDLE);
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
