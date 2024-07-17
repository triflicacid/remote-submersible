#ifndef _STEPPER_EVENT_H_
#define _STEPPER_EVENT_H_

// stepper motor: backward position
#define POSITION_BACKWARD -1
// stepper motor: middle position
#define POSITION_MIDDLE 0
// stepper motor: forward position
#define POSITION_FORWARD 1

#include <stdint.h>
#include <stdbool.h>

#include "stepper-motor.h"

typedef struct stepper_event_t stepper_event_t;

// create a new stepper event
// provide absolute steps in respective direction to arrive at forward/backward positions
stepper_event_t stepper_event_create(uint32_t forwards_target, uint32_t backwards_target);

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
