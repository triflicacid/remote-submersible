#ifndef _TM1637_H_
#define _TM1637_H_

#include "util.h"

typedef struct {
  port_t *port;
  pin_t clk;
  pin_t dio;
  uint8_t brightness;
} tm1637_t;

// initialise structure
void tm1637_init(tm1637_t *tm, port_t *port, pin_t clk, pin_t dio);

// set brightness from 0 to 8
void tm1637_brightness(tm1637_t *tm, uint8_t brightness);

// display integer on display
void tm1637_display(tm1637_t *tm, int32_t digit, bool separator);

// externally defined delay function in micro-seconds
extern void delay_us(uint32_t us);

#endif
