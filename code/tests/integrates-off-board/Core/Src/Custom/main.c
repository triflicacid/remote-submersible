#include "constants.h"
#include "actions.h"
#include "globals.h"
#include "depth.h"
#include "pins.h"
#include "../Lib/action-mgr.h"
#include "../Lib/mcp23s17.h"
#include "../Lib/stored-code.h"
#include "../Lib/timed-lock.h"


lora_t g_lora;

static mcp_t mcp1_2, mcp3_4;
display_t g_display;

static volatile bool read_joystick = false; // prevent transmitting on first update
volatile uint16_t g_joystick_data[ADC_NCONV];
static volatile uint16_t prev_joystick_data[ADC_NCONV]; // store previous results for comparison

counter_t g_movement_counter;

static timed_lock_t lock_send_code, lock_req_code, lock_release_pod, lock_tristate_down, lock_tristate_up;


// segment data for movement on 7-segment display
// { none, top, middle, bottom }
static uint32_t movement_segment_data[] = {
  0b0000000,
  0b1000000,
  0b0000001,
  0b0001000
};

// interrupt handler various actions, but not as an actual interrupt
void interrupt_send_code(void) {
  timed_lock_call(&lock_send_code, HAL_GetTick());
}

void interrupt_request_code(void) {
  timed_lock_call(&lock_req_code, HAL_GetTick());
}

void interrupt_release_pod(void) {
  timed_lock_call(&lock_release_pod, HAL_GetTick());
}

void interrupt_tristate_up(void) {
  timed_lock_call(&lock_tristate_up, HAL_GetTick());
}

void interrupt_tristate_down(void) {
  timed_lock_call(&lock_tristate_down, HAL_GetTick());
}

// INTERRUPT: override GPIO external interrupt callback
void HAL_GPIO_EXTI_Callback(uint16_t pin) {
  switch (pin) {
    case GPIO_PIN_8: // move tri-state switch into down position
      create_action(interrupt_tristate_down);
      break;
    case GPIO_PIN_9: // move tri-state switch into up position
      create_action(interrupt_tristate_up);
      break;
    case GPIO_PIN_10: // press 'send code' button
      create_action(interrupt_send_code);
      break;
    case GPIO_PIN_11: // press 'request code' button
      create_action(interrupt_request_code);
      break;
    case GPIO_PIN_12: // press 'release escape pod' button
      create_action(interrupt_release_pod);
      break;
  }
}

// INTERRUPT: override timer callback
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *h) {
  if (h == &TIMER_HANDLE) {
    // poll joystick; start ADC
    HAL_ADC_Start_DMA(&ADC_HANDLE, (uint32_t *) g_joystick_data, ADC_NCONV);

    // DMA writes ADC results to buffer
    // compare new results, only update if differ to old
    if (read_joystick) {
      if (!is_within_tolerance(g_joystick_data[0], prev_joystick_data[0], JOYSTICK_TOLERANCE) || !is_within_tolerance(g_joystick_data[1], prev_joystick_data[1], JOYSTICK_TOLERANCE)) {
    	//create_action(action_propeller);

    	// update previous values
    	prev_joystick_data[0] = g_joystick_data[0];
    	prev_joystick_data[1] = g_joystick_data[1];
      }
    } else {
      // update values and mark as read
      prev_joystick_data[0] = g_joystick_data[0];
      prev_joystick_data[1] = g_joystick_data[1];
      read_joystick = true;
    }

    return;
  }

  if (h == &TIMER_DEPTH_HANDLE) {
    create_action(action_display_movement_tick);
//#ifdef PREDICT_DEPTH
    //create_action(action_predict_depth_tick);
//#endif
    return;
  }
}

// on-tick callback for movement counter
void movement_counter_on_tick(uint32_t tick) {
  uint8_t i;

  if (tick == 0) {
    i = 0;
  } else {
    i = get_vert_dir() == TRISTATE_TRUE ? tick : 4 - tick;
  }

  display_write_manual(&g_display, 4, movement_segment_data[i]);
}

void setup(void) {
	//reset all io
	set_pin(&pin_reset);
	HAL_Delay(1);
	reset_pin(&pin_reset);
	HAL_Delay(1);
	set_pin(&pin_reset);
	HAL_Delay(5);

  // initialise LoRa device with +20dBm
  lora_init(&g_lora, &SPI_HANDLE, &pin_cs_radio, &pin_reset);
  lora_set_tx_power(&g_lora, 20);

  timed_lock_init(&lock_send_code, 10, action_send_code);
    timed_lock_init(&lock_req_code, 10, action_request_code);
    timed_lock_init(&lock_release_pod, 10, action_release_pod);
    timed_lock_init(&lock_tristate_up, 10, action_ballast);
    timed_lock_init(&lock_tristate_down, 10, action_ballast);

  // set payload receive handlers
  register_send_code_callback(recv_send_code);



  // initialise 7-segment display
  mcp_init(&mcp1_2, &SPI_HANDLE, &pin_cs1_2, 0x00, &pin_reset);

  mcp_init(&mcp3_4, &SPI_HANDLE, &pin_cs3_4, 0x00, &pin_reset);

  display_init(&g_display, (mcp_t *[2]) { &mcp1_2, &mcp3_4 }, 4, true);

  // hardcode internal code
  save_code(CODE_INITIAL_VALUE);

  // start timers
  TIMER_HANDLE.Instance->CNT=0;
  __HAL_TIM_CLEAR_FLAG(&TIMER_HANDLE, TIM_SR_UIF);
  HAL_TIM_Base_Start_IT(&TIMER_HANDLE);

  // initialise debouncing locks
  timed_lock_init(&lock_send_code, 10, action_send_code);
  timed_lock_init(&lock_req_code, 10, action_request_code);
  timed_lock_init(&lock_release_pod, 10, action_release_pod);
  timed_lock_init(&lock_tristate_up, 10, action_ballast);
  timed_lock_init(&lock_tristate_down, 10, action_ballast);

  // setup movement counter
  counter_init(&g_movement_counter, 4);
  counter_on_tick(&g_movement_counter, movement_counter_on_tick);

  // finally, set LoRa to receive mode
  //lora_receive(&g_lora, 0);
}

void loop(void) {
  execute_pending_actions();
	HAL_Delay(1);
}
