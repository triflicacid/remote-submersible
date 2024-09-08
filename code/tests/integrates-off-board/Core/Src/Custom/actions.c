#include "actions.h"
#include "constants.h"
#include "depth.h"
#include "globals.h"
#include "pins.h"
#include "../Lib/counter.h"
#include "../Lib/stored-code.h"
#include "../Lib/util.h"

// record ballast state (NOT tri-state switch state)
tristate_t ballast_state = TRISTATE_UNDEF;

void action_propeller(void) {
  // convert raw ADC values into range
  propeller_data data;
  data.x = map_range(g_joystick_data[0], JOYSTICK_X_MIN, JOYSTICK_X_MAX, -1, 1);
  data.y = map_range(g_joystick_data[1], JOYSTICK_Y_MIN, JOYSTICK_Y_MAX, -1, 1);

  transmit(&g_lora, OP_PROPELLER, RADIO_ON_BOARD_IDENTIFIER, &data, sizeof(data));
}

void action_ballast(void) {
  // read which pin is active (position of switch)
  tristate_t position = read_tristate_pins(&pin_tristate_down, &pin_tristate_up);

  // calculate new ballast position
  tristate_t new_state = ballast_state;

  switch (ballast_state) {
    case TRISTATE_FALSE:
      if (position == TRISTATE_TRUE) {
        new_state = TRISTATE_UNDEF;
      }
      break;
    case TRISTATE_UNDEF:
      if (position != TRISTATE_UNDEF) {
        new_state = position;
      }
      break;
    case TRISTATE_TRUE:
      if (position == TRISTATE_FALSE) {
        new_state = TRISTATE_UNDEF;
      }
      break;
  }

  // exit if there is no change
  if (new_state == ballast_state) {
    return;
  }

  // update state
  ballast_state = new_state;

  // create payload
  ballast_data data = { ballast_state };

  // record direction
  set_vert_dir(data.mode);

  // start or stop depth timer if hovering
  if (data.mode == TRISTATE_UNDEF) {
    HAL_TIM_Base_Stop_IT(&TIMER_DEPTH_HANDLE);
  } else {
    HAL_TIM_Base_Start_IT(&TIMER_DEPTH_HANDLE);
  }

  transmit(&g_lora, OP_BALLAST, RADIO_ON_BOARD_IDENTIFIER, &data, sizeof(data));
}

void action_send_code(void) {
  // create payload to contain our stored code
  code_data data = { fetch_code() };
  transmit(&g_lora, OP_SEND_CODE, RADIO_ON_BOARD_IDENTIFIER, &data, sizeof(data));

  display_write(&g_display, data.code, 0x0);
}

void action_request_code(void) {
  transmit_opcode(&g_lora, OP_REQUEST_CODE, RADIO_ON_BOARD_IDENTIFIER);
}

void action_release_pod(void) {
  transmit_opcode(&g_lora, OP_RELEASE_POD, RADIO_ON_BOARD_IDENTIFIER);
}

void recv_send_code(const payload_header *hdr, const code_data *data) {
  display_write(&g_display, data->code, 0x0);
}

#ifdef PREDICT_DEPTH
void action_predict_depth_tick(void) {
  // increase time spent in direction
  inc_time_in_dir(TIMER_DEPTH_INTERVAL);

  // display depth in form `X.XX`
  display_write(&g_display, estimate_depth() * 100, 0x4);
}
#endif

void action_display_movement_tick(void) {
  // tick movement counter
  counter_tick(&g_movement_counter);
}
