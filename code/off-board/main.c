#include "constants.h"
#include "actions.h"
#include "globals.h"
#include "depth.h"
#include "shared/action-mgr.h"
#include "shared/stored-code.h"
#include "shared/timed-lock.h"

display_t g_display;
lora_t g_lora;
volatile bool is_adc_complete = false; // indicate if the ADC has completed conversion
volatile dma_t g_joystick_data[ADC_NCONV];
volatile dma_t prev_joystick_data[ADC_NCONV]; // store prevous results for comparison
counter_t movement_counter;

timed_lock_t lock_send_code, lock_req_code, lock_release_pod, lock_tristate_down, lock_tristate_up;

// segment data for movement on 7-segment display
// none, top, middle, bottom
uint32_t movement_segment_data[] = {
  0b0000000,
  0b1000000,
  0b0000001,
  0b0001000
};

// interrupt handler various actions, but not as an actual interrupt
void interrupt_send_code(void) {
  timed_lock_call(&lock_send_code, HAL_GetTick());
}

void interrupt_request_code(void) {
  timed_lock_call(&lock_req_code, HAL_GetTick());
}

void interrupt_release_pod(void) {
  timed_lock_call(&lock_release_pod, HAL_GetTick());
}

void interrupt_tristate_up(void) {
  timed_lock_call(&lock_tristate_up, HAL_GetTick());
}

void interrupt_tristate_down(void) {
  timed_lock_call(&lock_tristate_down, HAL_GetTick());
}

// INTERRUPT: override GPIO external interrupt callback
void HAL_GPIO_EXTI_Callback(uint16_t pin) {
  switch (pin) {
    case SEND_CODE_PIN: // press 'send code' button
      create_action(interrupt_send_code);
      break;
    case REQUEST_CODE_PIN: // press 'request code' button
      create_action(interrupt_request_code);
      break;
    case RELEASE_POD_PIN: // press 'release escape pod' button
      create_action(interrupt_release_pod);
      break;
    case BALLAST_UP_PIN: // move tri-state switch into up position
      create_action(interrupt_tristate_up);
      break;
    case BALLAST_DOWN_PIN: // move tri-state switch into down position
      create_action(interrupt_tristate_down);
      break;
  }
}

// INTERRUPT: override timer callback
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *h) {
  if (h == &TIMER_HANDLE) {
    // if ADC is done, check if data is different from previous
    if (is_adc_complete && (g_joystick_data[0] != prev_joystick_data[0] || g_joystick_data[1] != prev_joystick_data[1])) {
      is_adc_complete = false;

      // update previous values
      prev_joystick_data[0] = g_joystick_data[0];
      prev_joystick_data[1] = g_joystick_data[1];

      // queue action handler
      create_action(action_propeller);

      return;
    }

    if (h == &TIMER_DEPTH_HANDLE) {
      create_action(action_display_movement_tick);
      
#ifdef PREDICT_DEPTH
      create_ation(action_predict_depth_tick);
#endif
      return;
    }
  }
}

// INTERRUPT: override ADC completion callback
void HAL_ADC_ConvCompltCallback(ADC_HandleTypeDef *h) {
  is_adc_complete = true;
}

// INTERRUPT: SPI device RX complete
void HAL_SPI_RxCompltCallback(SPI_HandleTypeDef *h) {
  if (h == &SPI_HANDLE) { // LoRa device received data
    create_action(action_rx_opcode);
  }
}

// on-tick callback for movement counter
void movement_counter_on_tick(uint32_t tick) {
  uint8_t i;

  if (tick == 0) {
    i = 0;
  } else {
    i = get_vert_dir() == TRISTATE_TRUE ? tick : 4 - tick;
  }

  display_write_manual(&g_display, 4, movement_segment_data[i]);
}

void setup(void) {
  // initialise LoRa device with max TX power
  lora_setup(&g_lora, &SPI_HANDLE, LORA_NSS_PORT, LORA_NSS_PIN);
  lora_maximise_tx_power(&g_lora);

  // set payload receive handlers
  register_send_code_callback(recv_send_code);

  // initialise 7-segment display
  display_init(&g_display, &SPI_HANDLE, (ploc_t[2]) {
    { DISPLAY_SELECT1_2_PORT, DISPLAY_SELECT1_2_PIN },
    { DISPLAY_SELECT3_4_PORT, DISPLAY_SELECT3_4_PIN }
  }, 4);

  // start joystick ADC in DMA mode
  HAL_ADC_Start_DMA(&ADC_HANDLE, g_joystick_data, ADC_NCONV);

#ifdef CODE_INTERNAL_VALUE
  // hardcode internal code
  save_code(CODE_INTERNAL, CODE_INTERNAL_VALUE);
#endif

  // start timers
  HAL_TIM_Base_Start_IT(&TIMER_HANDLE);

  // initialise debouncing locks
  timed_lock_init(&lock_send_code, 10, action_send_code);
  timed_lock_init(&lock_req_code, 10, action_request_code);
  timed_lock_init(&lock_release_pod, 10, action_release_pod);
  timed_lock_init(&lock_tristate_up, 10, action_ballast);
  timed_lock_init(&lock_tristate_down, 10, action_ballast);

  // setup movement counter
  counter_init(&movement_counter, 4);
  counter_on_tick(&movement_counter, movement_counter_on_tick);

  // finally, set LoRa to receive mode, and receive OPCODE in async mode
  lora_mode_rx(&g_lora, true);
  lora_receive_async(&g_lora, g_lora_buffer, sizeof(opcode_t));
}

void loop(void) {
  execute_pending_actions();
}
