#include "actions.h"
#include "constants.h"
#include "depth.h"
#include "globals.h"
#include "shared/counter.h"
#include "shared/stored-code.h"
#include "shared/util.h"

// record balast state (NOT tri-state switch state)
tristate_t ballast_state = TRISTATE_UNDEF;

void action_propeller(void) {
  // convert raw ADC values into range
  propeller_data data;
  data.x = map_range(g_joystick_data[0], JOYSTICK_X_MIN, JOYSTICK_X_MAX, 0, 1);
  data.y = map_range(g_joystick_data[1], JOYSTICK_Y_MIN, JOYSTICK_Y_MAX, 0, 1);

  transmit(&g_lora, OP_PROPELLER, &data, sizeof(data));
}

void action_ballast(void) {
  // read which pin is active (position of switch)
  tristate_t position = read_tristate_pins(BALLAST_PORT, BALLAST_DOWN_PIN, BALLAST_PORT, BALLAST_UP_PIN);

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

#ifdef PREDICT_DEPTH
  // update vertical direction for depth estimation
  set_vert_dir(data.mode);
#endif

  // start or stop depth timer if hovering
  if (data.mode == TRISTATE_UNDEF) {
    HAL_TIM_Base_Stop_IT(&TIMER_DEPTH_HANDLE);
  } else {
    HAL_TIM_Base_Start_IT(&TIMER_DEPTH_HANDLE);
  }

  transmit(&g_lora, OP_BALLAST, &data, sizeof(data));
}

void action_send_code(void) {
  // create payload to contain our stored code
  code_data data = { fetch_code(CODE_INTERNAL) };
  transmit(&g_lora, OP_SEND_CODE, &data, sizeof(data));
}

void action_request_code(void) {
  transmit_opcode(&g_lora, OP_REQUEST_CODE);
}

void action_release_pod(void) {
  transmit_opcode(&g_lora, OP_RELEASE_POD);
}

void action_rx_opcode(void) {
  lora_rx_point_next_packet(&g_lora);

  // get opcode from buffer
  opcode_t opcode = g_lora_buffer[0];

  // get expected payload size; receive if necessary
  uint64_t size = get_payload_size(opcode);

  if (size > 0) {
    lora_mode_rx(&g_lora, false);
    lora_receive(&g_lora, g_lora_buffer, size);
  }

  // clear IRQ flags
  lora_irq_clear(&g_lora);

  // deliver payload
  on_recv_payload(opcode, g_lora_buffer);

  // reset FIFO
  lora_rx_reset_buffer(&g_lora);

  // go back to listening on RX
  lora_mode_rx(&g_lora, false);
  lora_receive_async(&g_lora, g_lora_buffer, sizeof(opcode_t));
}

void recv_send_code(code_data *data) {
  // save downloaded code and display to operator
  save_code(CODE_DOWNLOADED, data->code);
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
  counter_tick(&movement_counter);
}
