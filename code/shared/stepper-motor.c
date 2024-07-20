#include "stepper-motor.h"

// number of sequences, for each mode
static uint8_t sequence_count[] = {
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

// map mode to sequuences, length determined by sequence_count[]
static uint8_t *sequences[] = {
  sequence_wave_drive,
  sequence_full_drive,
  sequence_half_drive
};


void stepper_motor_init(stepper_motor_t *motor, GPIO_TypeDef *port, uint16_t pins[STEPPER_MOTOR_PINS], uint8_t mode) {
  motor->port = port;
  
  for (uint8_t i = 0; i < STEPPER_MOTOR_PINS; i++) {
    motor->pins[i] = pins[i];
  }
  
  // set current step to max, so 'next' will trigger step 0
  motor->step = sequence_count[mode];
  
  // store motor drive mode
  motor->mode = mode;
}

// set a stepper motor to the given step
static void step_to(stepper_motor_t *motor, uint8_t step) {
  // extract sequence data from corresponding step
  uint8_t sequence = sequences[motor->mode][step];
  
  // mask to check against sequence
  uint32_t mask = 1;
  
  // set motor pins
  for (uint8_t i = 0; i < STEPPER_MOTOR_PINS; i++) {
    HAL_GPIO_WritePin(motor->port, motor->pins[i], (sequence & mask) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    mask <<= 1;
  }
}

void stepper_motor_step(stepper_motor_t *motor) {
  // increase step count, overflowing if necessary
  motor->step++;
  
  if (motor->step >= sequence_count[motor->mode]) {
    motor->step = 0;
  }
  
  step_to(motor, motor->step);
}

void stepper_motor_step_back(stepper_motor_t *motor) {
  step_to(motor, motor->step);

  // decrease step count, underfloweing if necessary
  if (motor->step == 0) {
    motor->step = sequence_count[motor->mode] - 1;
  } else {
    motor->step--;
  }
}
