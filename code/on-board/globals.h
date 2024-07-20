#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "shared/dc-motor.h"
#include "shared/lora.h"
#include "shared/stepper-motor-event.h"

// LoRa transmission device
extern lora_t g_lora;

// DC motor: primary propeller
extern dc_motor_t g_primary_motor;

// DC motor: secondary propeller
extern dc_motor_t g_secondary_motor;

// event for the ballast stepper motor
extern volatile stepper_event_t g_ballast;

#endif
