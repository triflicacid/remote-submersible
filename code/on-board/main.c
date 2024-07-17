#include "main.h"

#include "constants.h"

#include "shared/action-mgr.h"
#include "shared/timed-events.h"

lora_t g_lora;

void setup(void) {
  // initialise LoRa device
  lora_setup(&g_lora, &LORA_SPI_HANDLER, LORA_NSS_PORT, LORA_NSS_PIN);
}

void loop(void) {
  execute_pending_actions();
  timed_events_main_all();
}
