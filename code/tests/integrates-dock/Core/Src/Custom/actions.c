#include "actions.h"
#include "constants.h"
#include "globals.h"
#include "pins.h"
#include "../Lib/stored-code.h"
#include "../Lib/tm1637.h"

void action_request_code(void) {
  transmit_opcode(&g_lora, OP_REQUEST_CODE, RADIO_ON_BOARD_IDENTIFIER);

  // enable LED (LED indicates waiting for code)
  set_pin(&pin_led);
}

void recv_send_code(const payload_header *hdr, const code_data *data) {
  tm1637_display(&g_display, data->code, false);

  // disable LED
  reset_pin(&pin_led);
}
