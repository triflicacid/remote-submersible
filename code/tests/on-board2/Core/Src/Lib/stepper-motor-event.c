#include "stepper-motor-event.h"

void stepper_event_init(stepper_event_t *event, uint32_t forwards_target, uint32_t backwards_target) {
  event->target_pos = POSITION_MIDDLE;
  event->step_fwd = true;
  event->steps = 0;
  event->target = 0;
  event->target_fwd = forwards_target;
  event->target_bwd = backwards_target;
}

bool stepper_event_done(stepper_event_t *event) {
  return event->steps == event->target;
}

void stepper_event_update(stepper_event_t *event, int8_t new_position) {
  // same as old direction? all's good
  if (new_position == event->target_pos) {
    return;
  }
  
  // arrive at centre by undoing steps
  event->target = event->steps;
  
  // from new position, update target steps and step direction
  switch (new_position) {
    case POSITION_BACKWARD:
      event->target += event->target_bwd;
      event->step_fwd = false;
      break;
    case POSITION_MIDDLE:
      event->step_fwd = !event->step_fwd;
      break;
    case POSITION_FORWARD:
      event->target = event->target_fwd;
      event->step_fwd = true;
      break;
  }
  
  // reset steps
  event->steps = 0;
  
  // set new target position
  event->target_pos = new_position;
}

bool stepper_event_step(stepper_motor_t *motor, stepper_event_t *event) {
  // are we already done?
  if (stepper_event_done(event)) {
    return true;
  }
  
  // we are not done, so same as forcing step
  stepper_event_force_step(motor, event);
  
  // return if event is now done
  return stepper_event_done(event);
}

void stepper_event_force_step(stepper_motor_t *motor, stepper_event_t *event) {
  // step motor in stated direction
  if (event->step_fwd) {
    stepper_motor_step(motor);
  } else {
    stepper_motor_step_back(motor);
  }
  
  // increase step count
  event->steps++;
}
