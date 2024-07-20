#include "main.h"

#include "constants.h"
#include "actions.h"
#include "shared/action-mgr.h"

lora_t g_lora;

// INTERRUPT: override timer callback
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *h) {
  if (h == &TIMER_ELECTROMAGNET_HANDLE) {
    // times up! reactivate magnet amd cancel timer
    HAL_GPIO_WritePin(ELECTROMAGNET_PORT, ELECTROMAGNET_PIN, GPIO_PIN_RESET);
    HAL_TIM_Base_Stop_IT(&TIMER_ELECTROMAGNET_HANDLE);
    return;
  }
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

  // TODO COMPLETE: register payload receive handlers
  register_send_code_callback(recv_send_code);
  register_request_code_callback(recv_request_code);
  register_release_pod_callback(recv_release_pod);

  // finally, set LoRa to receive mode
  lora_mode_rx(&g_lora, false);
}

void loop(void) {
  execute_pending_actions();
}
