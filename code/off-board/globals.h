#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "shared/7-segment.h"
#include "shared/counter.h"
#include "shared/lora.h"

// data type of DMA used by ADC1
typedef uint16_t dma_t;

// four digit 7-segment display
extern display_t g_display;

// LoRa transmission device
extern lora_t g_lora;

// ADC results from joystick
extern volatile uint32_t g_joystick_data[2];

// counter used for movement indication when ascending/descending
extern counter_t movement_counter;

#endif
