#ifndef _STEPPER_MOTOR_EVENT_H_
#define _STEPPER_MOTOR_EVENT_H_

// stepper motor: backward position
#define POSITION_BACKWARD -1
// stepper motor: middle position
#define POSITION_MIDDLE 0
// stepper motor: forward position
#define POSITION_FORWARD 1

#include <stdint.h>
#include <stdbool.h>

#include "stepper-motor.h"

// !DO NOT ACCESS MANUALLY
typedef struct {
  int8_t target_pos; // direction of stepper motor (POSITION_* macro)
  bool step_fwd; // true=step, false=step_back
  uint32_t steps; // steps carried out to satisfy target
  uint32_t target; // steps required to meet desired position
  
  int32_t target_fwd; // absolute forward target
  int32_t target_bwd; // absolute backwards target
} stepper_event_t;

// initialise stepper motor event struct
// provide absolute steps in respective direction to arrive at forward/backward positions
void stepper_event_init(stepper_event_t *event, uint32_t forwards_target, uint32_t backwards_target);

// check: has event met target?
bool stepper_event_done(stepper_event_t *event);

// update event: change direction
void stepper_event_update(stepper_event_t *event, int8_t new_position);

// step a stepper motor to progress the given event, *only* if event is not done
// return if the event is now done after step
bool stepper_event_step(stepper_motor_t *motor, stepper_event_t *event);

// same as stepper_event_step, but steps motor even if done
void stepper_event_force_step(stepper_motor_t *motor, stepper_event_t *event);

#endif
