#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "7-segment.h"
#include "../Lib/counter.h"
#include "../Lib/lora.h"

// four digit 7-segment display
extern display_t g_display;

// LoRa transmission device
extern lora_t g_lora;

// ADC results from joystick
extern volatile uint16_t g_joystick_data[2];

// counter used for movement indication when ascending/descending
extern counter_t movement_counter;

// HAL handles
extern TIM_HandleTypeDef TIMER_DEPTH_HANDLE, TIMER_HANDLE;
extern SPI_HandleTypeDef SPI_HANDLE;
extern ADC_HandleTypeDef ADC_HANDLE;


#endif
