#include "main.h"

#include "shared/action-mgr.h"
#include "constants.h"

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
}

void loop(void) {
  execute_pending_actions();
}
