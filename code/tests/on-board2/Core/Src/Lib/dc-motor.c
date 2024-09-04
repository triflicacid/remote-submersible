#include "dc-motor.h"

#include <stdbool.h>

static inline void write_in1(dc_motor_t *motor, bool state) {
  write_pin(motor->port, motor->in1, state);
}

static inline void write_in2(dc_motor_t *motor, bool state) {
  write_pin(motor->port, motor->in2, state);
}

static inline bool read_in1(dc_motor_t *motor) {
  return read_pin(motor->port, motor->in1);
}

static inline bool read_in2(dc_motor_t *motor) {
  return read_pin(motor->port, motor->in2);
}

void dc_motor_init(dc_motor_t *motor, TIM_HandleTypeDef* htim, uint32_t channel, port_t *port, pin_t in1, pin_t in2) {
  motor->htim = htim;
  motor->channel = channel;
  motor->port = port;
  motor->in1 = in1;
  motor->in2 = in2;
  motor->vel = 0;
}

void dc_motor_stop(dc_motor_t *motor) {
  if (motor->vel != 0) {
    // 0 0 / 1 1
    write_in2(motor, read_in1(motor));
    motor->vel = 0;
  }
}

void dc_motor_forwards(dc_motor_t *motor) {
  if (motor->vel <= 0) {
    // 1 0
    write_in1(motor, true);
    write_in2(motor, false);
    motor->vel = -motor->vel;
  }
}

void dc_motor_backwards(dc_motor_t *motor) {
  if (motor->vel >= 0) {
    // 0 1
    write_in1(motor, false);
    write_in2(motor, true);
    motor->vel = -motor->vel;
  }
}

void dc_motor_reverse(dc_motor_t *motor) {
  write_in1(motor, !read_in1(motor));
  write_in2(motor, !read_in2(motor));
  motor->vel = -motor->vel;
}

void dc_motor_set_speed(dc_motor_t *motor, double frac) {
  // clamp fraction
  if (frac < 0) {
    frac = 0;
  } else if (frac > 1) {
    frac = 1;
  }

  // already at speed?
  if (motor->vel == frac || motor->vel == -frac) {
    return;
  }

  // calculate and set CCR
  uint32_t arr = __HAL_TIM_GET_AUTORELOAD(motor->htim);
  uint32_t ccr = frac * (arr + 1);
  __HAL_TIM_SET_COMPARE(motor->htim, motor->channel, ccr);

  // update cached velocity (ensure correct sign)
  motor->vel = motor->vel < 0 ? -frac : frac;
}

double dc_motor_get_speed(dc_motor_t *motor) {
  return motor->vel < 0 ? -motor->vel : motor->vel;
}

void dc_motor_set_velocity(dc_motor_t *motor, double frac) {
  if (motor->vel == frac) {
    return;
  }

  if (frac < 0) {
    dc_motor_backwards(motor);
    dc_motor_set_speed(motor, -frac);
  } else {
    dc_motor_forwards(motor);
    dc_motor_set_speed(motor, frac);
  }
}

double dc_motor_get_velocity(dc_motor_t *motor) {
  return motor->vel;
}
