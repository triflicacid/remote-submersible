#include "actions.h"
#include "constants.h"
#include "globals.h"
#include "pins.h"
#include "../Lib/timer.h"
#include "../Lib/stored-code.h"

void recv_propeller(const payload_header *hdr, const propeller_data *data) {
  // update the velocity of both propeller motors
  dc_motor_set_velocity(&g_primary_motor, data->y);
  dc_motor_set_velocity(&g_secondary_motor, data->x);
}

void recv_ballast(const payload_header *hdr, const ballast_data *data) {
  // update stepper motor event
  stepper_event_update(&g_ballast, data->mode);

  // if action is to be taken, start clock
  if (!stepper_event_done(&g_ballast)) {
	TIMER_STEPPER_HANDLE.Instance->CNT=0;
	__HAL_TIM_CLEAR_FLAG(&TIMER_STEPPER_HANDLE, TIM_SR_UIF);
    HAL_TIM_Base_Start_IT(&TIMER_STEPPER_HANDLE);
  }
}

void recv_send_code(const payload_header *hdr, const code_data *data) {
  // cache code, we need to send it later
  save_code(data->code);
}

void recv_request_code(const payload_header *hdr) {
  // reply with our cached code
  code_data data = { fetch_code() };
  //transmit(&g_lora, OP_SEND_CODE, hdr->sender, &data, sizeof(data));
}

void recv_release_pod(const payload_header *hdr) {
  // de-activate electromagnet
  set_pin(&pin_electromagnet);

  // start electromagnet timer
  //HAL_TIM_Base_Stop_IT(&TIMER_ELECTROMAGNET_HANDLE);
  //TIM_ResetCounter(TIMER_ELECTROMAGNET_HANDLE.Instance);
  TIMER_ELECTROMAGNET_HANDLE.Instance->CNT=0;
  __HAL_TIM_CLEAR_FLAG(&TIMER_ELECTROMAGNET_HANDLE, TIM_SR_UIF);
  HAL_TIM_Base_Start_IT(&TIMER_ELECTROMAGNET_HANDLE);

  //HAL_Delay(2000);
  //reset_pin(&pin_electromagnet);
  //HAL_Delay(2000);
}
