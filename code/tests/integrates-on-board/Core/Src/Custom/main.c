#include "constants.h"
#include "actions.h"
#include "globals.h"
#include "pins.h"
#include "../Lib/action-mgr.h"
#include "../Lib/dc-motor.h"

lora_t g_lora;
dc_motor_t g_primary_motor;
dc_motor_t g_secondary_motor;
stepper_motor_t ballast_motor;
stepper_event_t g_ballast;

// INTERRUPT: override timer callback
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *h) {
  if (h == &TIMER_ELECTROMAGNET_HANDLE) {
    // times up! reactivate magnet and cancel timer
    reset_pin(&pin_electromagnet);
    HAL_TIM_Base_Stop_IT(&TIMER_ELECTROMAGNET_HANDLE);
    return;
  }

  if (h == &TIMER_STEPPER_HANDLE) {
    // step ballast motor, stopping the timer if done
    if (stepper_event_microstep(&ballast_motor, &g_ballast)) {
      HAL_TIM_Base_Stop_IT(h);
    }

    return;
  }
}

void interrupt_radio(void) {
	lora_dio_interrupt(&g_lora);
	lora_receive(&g_lora, 0);
}

void on_radio_receive(int size) {
	on_recv(&g_lora, size);
}

// INTERRUPT: override GPIO external interrupt callback
void HAL_GPIO_EXTI_Callback(uint16_t pin) {
  switch (pin) {
    case GPIO_PIN_1: // radio DIO
      create_action(interrupt_radio);
      break;
    default:
    	break;
  }
}

void setup(void) {
	//global reset
	set_pin(&pin_reset);
	HAL_Delay(1);
	reset_pin(&pin_reset);
	HAL_Delay(1);
	set_pin(&pin_reset);
	HAL_Delay(5);

  // initialise LoRa device with +20dBm
  lora_init(&g_lora, &SPI_HANDLE, &pin_cs_radio, &pin_reset);
  g_lora.on_receive = on_radio_receive;
  lora_set_tx_power(&g_lora, 20);

  // finally, set LoRa to receive mode
    lora_receive(&g_lora, 0);


  // register payload receive handlers
  register_propeller_callback(recv_propeller);
  register_ballast_callback(recv_ballast);
  register_send_code_callback(recv_send_code);
  register_request_code_callback(recv_request_code);
  register_release_pod_callback(recv_release_pod);

  return;

  // initialise DC motors and respective PWM channels
  dc_motor_init(&g_primary_motor, &TIMER_PWM_HANDLE, TIM_CHANNEL_1, &pins_dc_in[0], &pins_dc_in[1]);
  HAL_TIM_PWM_Start(&TIMER_PWM_HANDLE, TIM_CHANNEL_1);
  dc_motor_init(&g_secondary_motor, &TIMER_PWM_HANDLE, TIM_CHANNEL_3, &pins_dc_in[2], &pins_dc_in[3]);
  HAL_TIM_PWM_Start(&TIMER_PWM_HANDLE, TIM_CHANNEL_3);

  // initialise stepper motor and the ballast event operating it
  stepper_motor_init(&ballast_motor, (const pin_t *[4]) { &pin_stepper_black, &pin_stepper_blue, &pin_stepper_green, &pin_stepper_red }, STEPPER_MOTOR_FULL_DRIVE);
  uint8_t count = stepper_motor_microstep_count(&ballast_motor);
  stepper_event_init(&g_ballast, BALLAST_ASCEND_POSITION * count, BALLAST_DESCEND_POSITION * count);


}

void loop(void) {
  execute_pending_actions();

}
