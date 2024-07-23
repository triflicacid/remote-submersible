#include "constants.h"
#include "actions.h"
#include "globals.h"
#include "shared/action-mgr.h"
#include "shared/stored-code.h"

display_t g_display;
lora_t g_lora;
volatile bool is_adc_complete = false; // indicate if the ADC has completed conversion
volatile dma_t g_joystick_data[ADC_NCONV];
volatile dma_t prev_joystick_data[ADC_NCONV]; // store prevous results for comparison

// INTERRUPT: override GPIO external interrupt callback
void HAL_GPIO_EXTI_Callback(uint16_t pin) {
  switch (pin) {
    case SEND_CODE_PIN: // press 'send code' button
      create_action(action_send_code);
      break;
    case REQUEST_CODE_PORT: // press 'request code' button
      create_action(action_request_code);
      break;
    case RELEASE_POD_PIN: // press 'release escape pod' button
      create_action(action_release_pod);
      break;
    case BALLAST_UP_PIN:
    case BALLAST_DOWN_PIN: // toggle tri-state switch
      create_action(action_ballast);
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

#ifdef PREDICT_DEPTH
    if (h == &TIMER_DEPTH_HANDLE) {
      create_ation(action_predict_depth_tick);
      return;
    }
#endif

    if (h == &TIMER_7SEG_HANDLE) {
      display_cycle(&g_display);
    }
  }
}

// INTERRUPT: override ADC completion callback
void HAL_ADC_ConvCompltCallback(ADC_HandleTypeDef *h) {
  is_adc_complete = true;
}

// INTERRUPT: SPI device RX complete
void HAL_SPI_RxCompltCallback(SPI_HandleTypeDef *h) {
  if (h == &LORA_SPI_HANDLE) { // LoRa device received data
    create_action(action_rx_done);
  }
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

  // finally, set LoRa to receive mode
  lora_mode_rx(&g_lora, false);
}

void loop(void) {
  execute_pending_actions();
}
