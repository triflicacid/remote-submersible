#include "main.h"

#include "constants.h"

#include "actions.h"
#include "shared/action-mgr.h"
#include "shared/stored-code.h"

display_t g_display;
lora_t g_lora;
volatile timed_event_t *g_joystick_event;
volatile uint32_t g_joystick_data[2];

// store old g_joystick_data
volatile uint32_t g_old_joystick_data[2] = {0};

// INTERRUPT: override GPIO external interrupt callback
void HAL_GPIO_EXTI_Callback(uint16_t pin) {
  switch (pin) {
    case SEND_CODE_PIN:
      create_action(action_send_code);
      break;
    case REQUEST_CODE_PORT:
      create_action(action_request_code);
      break;
    case RELEASE_POD_PIN:
      create_action(action_release_pod);
      break;
  }
}

// INTERRUPT: override timer callback
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *h) {
  if (h == &TIMER_100ms_HANDLE) {
    timed_events_tick(TIMER_100ms_LIST, 100);
  } else if (h == &TIMER_1us_HANDLE) {
    timed_events_tick(TIMER_1us_LIST, 1);
  }
}

// INTERRUPT: override ADC completion callback
void HAL_ADC_ConvCompltCallback(ADC_HandleTypeDef *h) {
  if (h == &DMA_JOYSTICK_HANDLE) { // ADC done, so trigger joystick event
    timed_event_prime(g_joystick_event);
  }
}

// timed event callback for joystick DMA
void joystick_event_callback(timed_event_t *event) {
  // call action if either entries are different
  if (g_joystick_data[0] != g_old_joystick_data[0] || g_joystick_data[1] != g_old_joystick_data[1]) {
    // update old values
    g_old_joystick_data[0] = g_joystick_data[0];
    g_old_joystick_data[1] = g_joystick_data[1];

    // trigger appropriate action
    action_propeller(g_joystick_data[0], g_joystick_data[1]);
  }
}

void setup(void) {
  // initialise 7-segment display
  display_init(
    &g_display,
    DISPLAY_SEGMENT_PORT,
    (uint16_t[SEGMENT_COUNT]) { DISPLAY_SEGMENT_A, DISPLAY_SEGMENT_B, DISPLAY_SEGMENT_C, DISPLAY_SEGMENT_D, DISPLAY_SEGMENT_E, DISPLAY_SEGMENT_F, DISPLAY_SEGMENT_G },
    DISPLAY_SEGMENT_DP,
    DISPLAY_DIGIT_PORT,
    4,
    (uint16_t[4]) { DISPLAY_DIGIT_1, DISPLAY_DIGIT_2, DISPLAY_DIGIT_3, DISPLAY_DIGIT_4 },
    true
  );

  // initialise LoRa device
  lora_setup(&g_lora, &LORA_SPI_HANDLER, LORA_NSS_PORT, LORA_NSS_PIN);

  // setup joystick timeout and DMA
  g_joystick_event = timed_event_create(TIMER_100ms_LIST, joystick_event_callback, 1);
  HAL_ADC_Start_DMA(&DMA_JOYSTICK_HANDLE, g_joystick_data, sizeof(g_joystick_data));

#ifdef CODE_INTERNAL_VALUE
  // hardcode internal code
  save_code(CODE_INTERNAL, CODE_INTERNAL_VALUE);
#endif
}

void loop(void) {
  execute_pending_actions();
  timed_events_main_all();
}
