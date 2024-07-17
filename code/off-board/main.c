#include "main.h"

#include "constants.h"

#include "actions.h"
#include "shared/action-mgr.h"
#include "shared/stored-code.h"

display_t g_display;
lora_t g_lora;

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

#ifdef CODE_INTERNAL_VALUE
  // hardcode internal code
  save_code(CODE_INTERNAL, CODE_INTERNAL_VALUE);
#endif
}

void loop(void) {
  execute_pending_actions();
}

// overide GPIO external interrupt callback
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
