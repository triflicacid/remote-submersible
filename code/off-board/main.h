#ifndef _MAIN_H_
#define _MAIN_H_

#include "shared/7-segment.h"
#include "shared/lora.h"
#include "shared/timed-events.h"

// data type of DMA used by ADC1
typedef uint16_t dma_t;

// four digit 7-segment display
extern display_t g_display;

// LoRa transmission device
extern lora_t g_lora;

// timed event for joystick
extern volatile timed_event_t *g_joystick_event;

// ADC results from joystick
extern volatile uint32_t g_joystick_data[2];

// timed event for depth prediction
extern volatile timed_event_t *g_depth_event;

void setup(void);

void loop(void);

#endif
