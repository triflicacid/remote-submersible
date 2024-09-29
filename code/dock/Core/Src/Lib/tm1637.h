#ifndef _TM1637_H_
#define _TM1637_H_

#include "util.h"

typedef struct {
  const pin_t *clk;
  const pin_t *dio;
  uint8_t brightness;
} tm1637_t;

// initialise structure
void tm1637_init(tm1637_t *tm, const pin_t *clk, const pin_t *dio);

// set brightness from 0 to 8
void tm1637_brightness(tm1637_t *tm, uint8_t brightness);

// display integer on display
void tm1637_display(tm1637_t *tm, uint32_t value, bool separator);

#endif
