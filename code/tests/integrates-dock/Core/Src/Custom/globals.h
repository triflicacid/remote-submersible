#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "../Lib/lora.h"
#include "../Lib/tm1637.h"

// LoRa transmission device
extern lora_t g_lora;

// four digit 7-segment display (TIM1637 module)
extern tm1637_t g_display;

// HAL handles
extern SPI_HandleTypeDef SPI_HANDLE;

#endif
