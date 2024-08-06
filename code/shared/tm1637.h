#ifndef _TM1637_H_
#define _TM1637_H_

#include "util.h"

typedef struct {
  port_t *port;
  pin_t clk;
  pin_t dio;
  uint8_t brightness; // brightness byte
} tm1637_t;

// initialise structure
void tm1637_init(tm1637_t *tm, port_t *port, pin_t clk, pin_t dio);

// set brightness from 0 to 7
void tm1637_brightness(tm1637_t *tm, uint8_t brightness, bool on);

// display integer on display, starting at `pos`
void tm1637_write_int(tm1637_t *tm, int32_t digit, uint8_t pos);

// display float on display, starting at `pos`
void tm1637_write_float(tm1637_t *tm, float digit, uint8_t dp, uint8_t pos);

#endif
