#ifndef _MAIN_H_
#define _MAIN_H_

#include "shared/7-segment.h"
#include "shared/lora.h"

// four digit 7-segment display
extern display_t g_display;

// LoRa transmission device
extern lora_t g_lora;

void setup(void);

void loop(void);

#endif
