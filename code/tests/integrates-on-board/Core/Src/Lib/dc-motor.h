#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_

#include "util.h"

// stop dc motors if abs(frac) < this value
#define DC_MOTOR_STOP_LIMIT 0.1

// represents a DC motor interface
typedef struct dc_motor {
  TIM_HandleTypeDef *htim;  // handle of PWM timer
  uint32_t channel;  // PWM timer channel
  const pin_t *in1;  // IN1 pin
  const pin_t *in2;  // IN2 pin
  double vel;  // cache velocity
  void (*on_switch_direction)(struct dc_motor *);  // callback invoked on direction reversal
} dc_motor_t;

// initialise a DC motor struct
// provide timer handle and channel which generates PWM signal
void dc_motor_init(dc_motor_t *motor, TIM_HandleTypeDef *htim, uint32_t channel, const pin_t *in1, const pin_t *in2);

// halt the motor
void dc_motor_stop(dc_motor_t *motor);

// set motor to forward drive
void dc_motor_forwards(dc_motor_t *motor);

// set motor to backwards drive
void dc_motor_backwards(dc_motor_t *motor);

// reverse motor's direction
void dc_motor_reverse(dc_motor_t *motor);

// set motor's speed by providing a fraction between [0, 1]
void dc_motor_set_speed(dc_motor_t *motor, float frac);

// get the speed [0, 1] of the motor
double dc_motor_get_speed(dc_motor_t *motor);

// set a motor's speed and direction by providing a fraction [-1, 1]
void dc_motor_set_velocity(dc_motor_t *motor, float frac);

// get the velocity [-1, 1] of the motor
double dc_motor_get_velocity(dc_motor_t *motor);

#endif
