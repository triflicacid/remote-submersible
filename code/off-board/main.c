#include "main.h"

#include "constants.h"
#include "actions.h"
#include "shared/action-mgr.h"
#include "shared/stored-code.h"
#include "shared/tri-state-switch.h"

display_t g_display;
lora_t g_lora;
volatile bool g_adc_complete = false; // indicate if the ADC has completed conversion
volatile dma_t g_joystick_data[ADC_NCONV];
volatile dma_t g_joystick_data_prev[ADC_NCONV]; // store prevous results for comparison

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
    if (g_adc_complete && (g_joystick_data[0] != g_joystick_data_prev[0] || g_joystick_data[1] != g_joystick_data_prev[1])) {
      g_adc_complete = false;

      // update previous values
      g_joystick_data_prev[0] = g_joystick_data[0];
      g_joystick_data_prev[1] = g_joystick_data[1];

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

  }
}

// INTERRUPT: override ADC completion callback
void HAL_ADC_ConvCompltCallback(ADC_HandleTypeDef *h) {
  g_adc_complete = true;
}

// INTERRUPT: SPI device RX complete
void HAL_SPI_RxCompltCallback(SPI_HandleTypeDef *h) {
  if (h == &LORA_SPI_HANDLE) { // LoRa device received data
    create_action(action_rx_done);
  }
}

void setup(void) {
  // initialise LoRa device with max TX power
  lora_setup(&g_lora, &LORA_SPI_HANDLE, LORA_NSS_PORT, LORA_NSS_PIN);
  lora_maximise_tx_power(&g_lora);

  // set payload receive handlers
  register_send_code_callback(recv_send_code);

  // initialise 7-segment display
  // note difference between digit number on hardware and representation of 7-segment library
  display_init(
    &g_display,
    DISPLAY_SEGMENT_PORT,
    (uint16_t[SEGMENT_COUNT]) { DISPLAY_SEGMENT_A, DISPLAY_SEGMENT_B, DISPLAY_SEGMENT_C, DISPLAY_SEGMENT_D, DISPLAY_SEGMENT_E, DISPLAY_SEGMENT_F, DISPLAY_SEGMENT_G },
    DISPLAY_SEGMENT_DP,
    DISPLAY_DIGIT_PORT,
    DISPLAY_DIGITS,
    (uint16_t[DISPLAY_DIGITS]) { DISPLAY_DIGIT_4, DISPLAY_DIGIT_3, DISPLAY_DIGIT_2, DISPLAY_DIGIT_1 },
    true
  );

  // start joystick ADC in DMA mode
  HAL_ADC_Start_DMA(&ADC_HANDLE, g_joystick_data, ADC_NCONV);

#ifdef CODE_INTERNAL_VALUE
  // hardcode internal code
  save_code(CODE_INTERNAL, CODE_INTERNAL_VALUE);
#endif

  // start timer
  HAL_TIM_Base_Start_IT(&TIMER_HANDLE);

  // finally, set LoRa to receive mode
  lora_mode_rx(&g_lora, false);
}

void loop(void) {
  execute_pending_actions();
}
