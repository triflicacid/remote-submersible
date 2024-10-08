#include "stepper-motor.h"

// number of states in each sequence (indexed by mode)
static uint8_t sequence_lengths[] = {
  4, // wave drive
  4, // full drive
  8, // half drive
};

// step sequence for wave drive
static uint8_t sequence_wave_drive[] = {
  0b0001,
  0b0010,
  0b0100,
  0b1000
};

// step sequence for full drive
static uint8_t sequence_full_drive[] = {
  0b0011,
  0b0110,
  0b1100,
  0b1001
};

// step sequence for half drive
// (MSB) red, green, blue, black (LSB)
static uint8_t sequence_half_drive[] = {
  0b0001,
  0b0011,
  0b0010,
  0b0110,
  0b0100,
  0b1100,
  0b1000,
  0b1001
};

// map mode to sequences, length determined by sequence_count[]
static uint8_t *sequences[] = { sequence_wave_drive, sequence_full_drive, sequence_half_drive };


void stepper_motor_init(stepper_motor_t *motor, port_t *port, pin_t pins[STEPPER_MOTOR_PINS], uint8_t mode) {
  motor->port = port;
  
  for (uint8_t i = 0; i < STEPPER_MOTOR_PINS; i++) {
    motor->pins[i] = pins[i];
  }

  motor->step = 0;
  
  // store motor drive mode
  motor->mode = mode;
}

// set a stepper motor to the given step
static void stepper_set_state(stepper_motor_t *motor, uint8_t step) {
  // extract sequence data from corresponding step
  uint8_t state = sequences[motor->mode][step];
  
  // mask to check against sequence
  uint32_t mask = 1;
  
  // set motor pins
  for (uint8_t i = 0; i < STEPPER_MOTOR_PINS; i++) {
    write_pin(motor->port, motor->pins[i], state & mask);
    mask <<= 1;
  }
}

void stepper_motor_microstep(stepper_motor_t *motor) {
  stepper_set_state(motor, motor->step);

  // increase step count, overflowing if necessary
  motor->step++;
  
  if (motor->step >= sequence_lengths[motor->mode]) {
    motor->step = 0;
  }
}

void stepper_motor_microstep_back(stepper_motor_t *motor) {
  stepper_set_state(motor, motor->step);

  // decrease step count, underflowing if necessary
  if (motor->step == 0) {
    motor->step = sequence_lengths[motor->mode] - 1;
  } else {
    motor->step--;
  }
}

uint8_t stepper_motor_microstep_count(stepper_motor_t *motor) {
  return sequence_lengths[motor->mode];
}

void stepper_motor_depower(stepper_motor_t *motor) {
  for (uint8_t i = 0; i < STEPPER_MOTOR_PINS; i++) {
    write_pin(motor->port, motor->pins[i], false);
  }
}
