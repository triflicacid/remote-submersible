#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "../Lib/dc-motor.h"
#include "../Lib/lora.h"
#include "../Lib/stepper-motor-event.h"

// LoRa transmission device
extern lora_t g_lora;

// DC motor: primary propeller
extern dc_motor_t g_primary_motor;

// DC motor: secondary propeller
extern dc_motor_t g_secondary_motor;

// event for the ballast stepper motor
extern volatile stepper_event_t g_ballast;

// HAL handles
extern TIM_HandleTypeDef TIMER_ELECTROMAGNET_HANDLE, TIMER_STEPPER_HANDLE, TIMER_PWM_HANDLE;
extern SPI_HandleTypeDef SPI_HANDLE;

#endif
