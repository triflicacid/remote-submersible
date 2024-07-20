#ifndef _MAIN_H_
#define _MAIN_H_

#include "shared/dc-motor.h"
#include "shared/lora.h"

// LoRa transmission device
extern lora_t g_lora;

// DC motor: primary propeller
extern dc_motor_t g_primary_motor;

// DC motor: secondary propeller
extern dc_motor_t g_secondary_motor;

void setup(void);

void loop(void);

#endif
