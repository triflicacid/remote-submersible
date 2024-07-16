#include "main.h"

#include "constants.h"

lora_t g_lora;

void setup(void) {
  // initialise LoRa device
  lora_setup(&g_lora, &LORA_SPI_HANDLER, LORA_NSS_PORT, LORA_NSS_PIN);
}

void loop(void) {

}
