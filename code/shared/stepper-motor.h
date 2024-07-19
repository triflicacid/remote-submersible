#ifndef _STEPPER_MOTOR_H_
#define _STEPPER_MOTOR_H_

#include <stdint.h>

// defines number of pins motor has
// DO NOT CHANGE
#define STEPPER_MOTOR_PINS 4

#define STEPPER_MOTOR_WAVE_DRIVE 0
#define STEPPER_MOTOR_FULL_DRIVE 1
#define STEPPER_MOTOR_HALF_DRIVE 2

typedef struct {
  GPIO_TypeDef port; // port of pins
  uint16_t pins[STEPPER_MOTOR_PINS]; // motor pins a, b, ...
  uint8_t step; // store current step
  uint8_t mode; // stepper motor mode
} stepper_motor_t;

// initialise a stepper motor struct
void stepper_motor_init(stepper_motor_t *motor, GPIO_TypeDef port, uint16_t pins[STEPPER_MOTOR_PINS], uint8_t drive_mode);

// drive stepper motor forward one step
void stepper_motor_step(stepper_motor_t *motor);

// drive stepper motor backwards one step
void stepper_motor_step_back(stepper_motor_t *motor);

// calculate delay in micro seconds between steps such that we achieve `rpm` rpm/min
uint64_t stepper_motor_calculate_delay(uint32_t steps_per_rev, uint32_t rpm);

#endif
