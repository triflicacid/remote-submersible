#include "dc-motor.h"

#include <stdbool.h>

inline void write_in1(dc_motor_t *motor, bool state) {
  HAL_GPIO_WritePin(motor->port, motor->in1, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

inline void write_in2(dc_motor_t *motor, bool state) {
  HAL_GPIO_WritePin(motor->port, motor->in2, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

inline bool read_in1(dc_motor_t *motor) {
  return HAL_GPIO_ReadPin(motor->port, motor->in1) == GPIO_PIN_SET;
}

inline bool read_in2(dc_motor_t *motor) {
  return HAL_GPIO_ReadPin(motor->port, motor->in2) == GPIO_PIN_SET;
}


void dc_motor_init(dc_motor_t *motor, TIM_HandleTypeDef* tim, uint32_t channel, GPIO_TypeDef *port, uint16_t in1, uint16_t in2) {
  motor->tim = tim;
  motor->channel = channel;
  motor->port = port;
  motor->in1 = in1;
  motor->in2 = in2;
}

void dc_motor_stop(dc_motor_t *motor) {
  // 0 0 / 1 1
  write_in2(motor, read_in1(motor));
}

void dc_motor_forwards(dc_motor_t *motor) {
  // 1 0
  write_in1(motor, true);
  write_in2(motor, false);
}

void dc_motor_backwards(dc_motor_t *motor) {
  // 0 1
  write_in1(motor, false);
  write_in2(motor, true);
}

void dc_motor_reverse(dc_motor_t *motor) {
  write_in1(motor, !read_in1(motor));
  write_in2(motor, !read_in2(motor));
}

void dc_motor_speed(dc_motor_t *motor, double frac) {
  // clamp fraction
  if (frac < 0) {
    frac = 0;
  } else if (frac > 1) {
    frac = 1;
  }

  // calculate and set CCR
  uint32_t arr = __HAL_TIM_GET_AUTORELOAD(motor->tim);
  uint32_t ccr = frac * (arr + 1);
  __HAL_TIM_SET_COMPARE(motor->tim, motor->channel, ccr);
}

void dc_motor_velocity(dc_motor_t *motor, double frac) {
  if (frac < 0) {
    dc_motor_backwards(motor);
    dc_motor_speed(motor, -frac);
  } else {
    dc_motor_forwards(motor);
    dc_motor_speed(motor, frac);
  }
}
