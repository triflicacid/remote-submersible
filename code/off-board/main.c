#include "main.h"

#include "constants.h"
#include "actions.h"
#include "depth.h"
#include "shared/action-mgr.h"
#include "shared/stored-code.h"

display_t g_display;
lora_t g_lora;
volatile timed_event_t *g_joystick_event;
volatile dma_t g_joystick_data[ADC_NCONV];
volatile dma_t g_joystick_data_prev[ADC_NCONV]; // store prevous results for comparison
volatile timed_event_t *g_depth_event;

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
  timed_events_tick(0, TIMER_TICK_PER);
}

// INTERRUPT: override ADC completion callback
void HAL_ADC_ConvCompltCallback(ADC_HandleTypeDef *h) {
  if (h == &DMA_JOYSTICK_HANDLE) { // ADC done, trigger joystick event
    timed_event_prime(g_joystick_event);
  }
}

// timed event callback for joystick DMA
void joystick_event_callback(timed_event_t *event) {
  // only invoke action if data has changed
  if (g_joystick_data[0] != g_joystick_data_prev[0] || g_joystick_data[1] != g_joystick_data_prev[1]) {
    action_propeller(
      g_joystick_data_prev[0] = g_joystick_data[0],
      g_joystick_data_prev[1] = g_joystick_data[1]
    );
  }
}

// INTERRUPT: SPI device RX complete
void HAL_SPI_RxCompltCallback(SPI_HandleTypeDef *h) {
  if (h == &LORA_SPI_HANDLE) { // LoRa device received data
    create_action(action_rx_done);
  }
}

#ifdef PREDICT_DEPTH
// timed event callback for depth prediction
// only called if state != HOVER
void depth_event_callback(timed_event_t *event) {
  // increase time spent in direction (in seconds)
  inc_time_in_dir(TIMER_TICK_PER / 1000.0);

  // display depth in form `X.XXX`
  display_write(&g_display, estimate_depth() * 1000.0, 0x8);

  // re-start event to emulate an interval
  timed_event_start(event);
}
#endif

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

  // setup joystick timeout and DMA (DMA is continuous, so only call once at start)
  // use timer to prevent create_action spam and overwhelming LoRa
  g_joystick_event = timed_event_create(0, joystick_event_callback, 1);
  timed_event_start(g_joystick_event);
  HAL_ADC_Start_DMA(&ADC_HANDLE, g_joystick_data, ADC_NCONV);

#ifdef PREDICT_DEPTH
  // setup depth prediction timeout
  g_depth_event = timed_event_create(0, depth_event_callback, 1);
#endif

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
  timed_events_main_all();
}
