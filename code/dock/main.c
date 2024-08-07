#include "constants.h"
#include "globals.h"
#include "actions.h"
#include "shared/action-mgr.h"

tm1637_t g_display;
lora_t g_lora;

// INTERRUPT: override GPIO external interrupt callback
void HAL_GPIO_EXTI_Callback(uint16_t pin) {
  switch (pin) {
    case REQUEST_CODE_PIN: // press 'request code' button
      create_action(action_request_code);
      break;
  }
}

// INTERRUPT: SPI device RX complete
void HAL_SPI_RxCompltCallback(SPI_HandleTypeDef *h) {
  if (h == &SPI_HANDLE) { // LoRa device received data
    create_action(action_rx_opcode);
  }
}

void setup(void) {
  // initialise LoRa device with max TX power
  lora_setup(&g_lora, &SPI_HANDLE, LORA_NSS_PORT, LORA_NSS_PIN);
  lora_maximise_tx_power(&g_lora);

  // initialise 7-segment display
  tm1637_init(&g_display, DISPLAY_PORT, DISPLAY_CLK, DISPLAY_DIO);

  // finally, set LoRa to receive mode, and receive OPCODE in async mode
  lora_mode_rx(&g_lora, true);
  lora_receive_async(&g_lora, g_lora_buffer, sizeof(opcode_t));
}

void loop(void) {
  execute_pending_actions();
}
