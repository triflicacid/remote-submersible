#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "../Lib/lora.h"
#include "../Lib/tm1637.h"

// four digit 7-segment display
extern tm1637_t g_display;

// LoRa transmission device
extern lora_t g_lora;

// HAL handles
extern SPI_HandleTypeDef SPI_HANDLE;

#endif
