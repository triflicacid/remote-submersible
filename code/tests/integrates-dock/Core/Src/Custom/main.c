#include "constants.h"
#include "actions.h"
#include "globals.h"
#include "pins.h"
#include "../Lib/action-mgr.h"
#include "../Lib/stored-code.h"
#include "../Lib/tm1637.h"

lora_t g_lora;
tm1637_t g_display;

// DEBUG variables
volatile bool has_recv_code = false;
const payload_header recv_hdr = { OP_SEND_CODE, RADIO_ON_BOARD_IDENTIFIER, RADIO_IDENTIFIER };
code_data recv_data;

void interrupt_radio(void) {
  lora_dio_interrupt(&g_lora);
  lora_receive(&g_lora, 0);
}

void on_radio_receive(int size) {
  on_recv(&g_lora, size);
}

// INTERRUPT: override GPIO external interrupt callback
void HAL_GPIO_EXTI_Callback(uint16_t pin) {
  switch (pin) {
    case GPIO_PIN_8: // radio DIO
      create_action(interrupt_radio);
      break;
    case GPIO_PIN_13: // button
      create_action(action_request_code);
      break;
    default:
    	break;
  }
}

void setup(void) {
  // reset all devices
  toggle_reset(&pin_reset, 5);

  // initialise LoRa device with +20dBm
  lora_init(&g_lora, &SPI_HANDLE, &pin_cs_radio, &pin_reset);
  lora_set_tx_power(&g_lora, 20);
  lora_receive(&g_lora, 0);

  // set payload receive handlers
  register_send_code_callback(recv_send_code);

  // initialise 7-segment display
  tm1637_init(&g_display, &pin_tm1637_clk, &pin_tm1637_data);
}

void loop(void) {
  execute_pending_actions();

  // DEBUG code for faking RF receive
  if (has_recv_code) {
	  has_recv_code = false;
	  recv_send_code(&recv_hdr, &recv_data);
  }
}
