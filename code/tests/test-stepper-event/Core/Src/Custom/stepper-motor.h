#ifndef _STEPPER_MOTOR_H_
#define _STEPPER_MOTOR_H_

#include "util.h"

// defines number of pins the motor has
#define STEPPER_MOTOR_PINS 4

#define STEPPER_MOTOR_WAVE_DRIVE 0
#define STEPPER_MOTOR_FULL_DRIVE 1
#define STEPPER_MOTOR_HALF_DRIVE 2

// !DO NOT ACCESS MANUALLY
typedef struct {
  port_t *port; // port of pins
  pin_t pins[STEPPER_MOTOR_PINS]; // motor pins a, b, ...
  uint8_t step; // store current step
  uint8_t mode; // stepper motor mode
} stepper_motor_t;

// initialise a stepper motor struct
void stepper_motor_init(stepper_motor_t *motor, port_t *port, pin_t pins[STEPPER_MOTOR_PINS], uint8_t drive_mode);

// drive stepper motor forward one step
void stepper_motor_microstep(stepper_motor_t *motor);

// drive stepper motor backwards one step
void stepper_motor_microstep_back(stepper_motor_t *motor);

// get number of microsteps in a step
uint8_t stepper_motor_microstep_count(stepper_motor_t *motor);

// de-power all coils in stepper motor
void stepper_motor_depower(stepper_motor_t *motor);

#endif
