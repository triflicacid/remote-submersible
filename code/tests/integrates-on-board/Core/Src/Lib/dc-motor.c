#include "dc-motor.h"

void dc_motor_init(dc_motor_t *motor, TIM_HandleTypeDef *htim, uint32_t channel, const pin_t *in1, const pin_t *in2) {
  motor->htim = htim;
  motor->channel = channel;
  motor->in1 = in1;
  motor->in2 = in2;
  motor->vel = 0;
  motor->on_switch_direction = NULL;
}

void dc_motor_stop(dc_motor_t *motor) {
  if (motor->vel != 0) {
    // 0 0 / 1 1
    write_pin(motor->in1, read_pin(motor->in2));
    motor->vel = 0;
    __HAL_TIM_SET_COMPARE(motor->htim, motor->channel, 0);
  }
}

void dc_motor_forwards(dc_motor_t *motor) {
  if (motor->vel <= 0) {
    // 1 0
    set_pin(motor->in1);
    reset_pin(motor->in2);
    motor->vel = -motor->vel;
  }
}

void dc_motor_backwards(dc_motor_t *motor) {
  if (motor->vel >= 0) {
    // 0 1
    reset_pin(motor->in1);
    set_pin(motor->in2);
    motor->vel = -motor->vel;
  }
}

void dc_motor_reverse(dc_motor_t *motor) {
  toggle_pin(motor->in1);
  toggle_pin(motor->in2);
  motor->vel = -motor->vel;
}

void dc_motor_set_speed(dc_motor_t *motor, float frac) {
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

void dc_motor_set_velocity(dc_motor_t *motor, float frac) {
  if (motor->vel == frac) {
    return;
  }

  // halt if =0 or near 0
  if (frac > -DC_MOTOR_STOP_LIMIT && frac < DC_MOTOR_STOP_LIMIT) {
	dc_motor_stop(motor);
    return;
  }

  // direction reversal?
  if (SIGN(frac) != SIGN(motor->vel) && motor->on_switch_direction) {
    motor->on_switch_direction(motor);
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
