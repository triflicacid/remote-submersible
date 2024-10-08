#include "constants.h"
#include "actions.h"
#include "globals.h"
#include "../Lib/action-mgr.h"

#include "../Lib/dc-motor.h"
#include "../Lib/lora.h"

lora_t g_lora;
dc_motor_t g_primary_motor;
dc_motor_t g_secondary_motor;
stepper_motor_t ballast_motor;
volatile stepper_event_t g_ballast;

// INTERRUPT: override timer callback
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *h) {
  if (h == &TIMER_ELECTROMAGNET_HANDLE) {
    // times up! reactivate magnet and cancel timer
    write_pin(ELECTROMAGNET_PORT, ELECTROMAGNET_PIN, false);
    HAL_TIM_Base_Stop_IT(&TIMER_ELECTROMAGNET_HANDLE);
    return;
  }

  if (h == &TIMER_STEPPER_HANDLE) {
    // step ballast motor, stopping the timer if done
    //if (stepper_event_step(&ballast_motor, &g_ballast)) {
      HAL_TIM_Base_Stop_IT(h);
    //}

    return;
  }
}

// INTERRUPT: SPI device RX complete
void HAL_SPI_RxCompltCallback(SPI_HandleTypeDef *h) {
  if (h == &SPI_HANDLE) { // LoRa device received data
    create_action(action_rx_opcode);
  }
}

void setup(void) {
  // initialise LoRa device with max TX power
  //lora_setup(&g_lora, &SPI_HANDLE, LORA_NSS_PORT, LORA_NSS_PIN, LORA_RESET_PORT, LORA_RESET_PIN);
  //lora_maximise_tx_power(&g_lora);

  // register payload receive handlers
  register_propeller_callback(recv_propeller);
  register_ballast_callback(recv_ballast);
  register_send_code_callback(recv_send_code);
  register_request_code_callback(recv_request_code);
  register_release_pod_callback(recv_release_pod);

  // initialise DC motors and respective PWM channels
  dc_motor_init(&g_primary_motor, &TIMER_PWM_HANDLE, DC_MOTOR_EN1_2_PWM_CHANNEL, DC_MOTOR_PORT, DC_MOTOR_IN1, DC_MOTOR_IN2);
  HAL_TIM_PWM_Start(&TIMER_PWM_HANDLE, DC_MOTOR_EN1_2_PWM_CHANNEL);
  dc_motor_init(&g_secondary_motor, &TIMER_PWM_HANDLE, DC_MOTOR_EN3_4_PWM_CHANNEL, DC_MOTOR_PORT, DC_MOTOR_IN3, DC_MOTOR_IN4);
  HAL_TIM_PWM_Start(&TIMER_PWM_HANDLE, DC_MOTOR_EN3_4_PWM_CHANNEL);

  // initialise stepper motor and the ballast event operating it
  //stepper_motor_init(&ballast_motor, STEPPER_PORT, (uint16_t[4]) { STEPPER_IN1, STEPPER_IN2, STEPPER_IN3, STEPPER_IN4 }, STEPPER_MOTOR_FULL_DRIVE);
  //stepper_event_init(&g_ballast, BALLAST_ASCEND_POSITION, BALLAST_DESCEND_POSITION);

  // finally, set LoRa to receive mode
  //lora_mode_rx(&g_lora, false);
  //lora_receive_async(&g_lora, g_lora_buffer, sizeof(opcode_t));

  dc_motor_backwards(&g_primary_motor);
  dc_motor_set_speed(&g_primary_motor, 0.0);
}

void loop(void) {
  //execute_pending_actions();
	dc_motor_set_speed(&g_primary_motor, 0.0);
	HAL_Delay(10000);
	dc_motor_set_speed(&g_primary_motor, 0.4);
	HAL_Delay(4000);

}
