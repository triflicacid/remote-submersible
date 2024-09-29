#ifndef _COUNTER_H_
#define _COUNTER_H_

#include <stdint.h>

typedef struct {
  uint32_t limit;
  uint32_t value;
  void (*on_tick)(uint32_t);
} counter_t;

// initialise counter
void counter_init(counter_t *counter, uint32_t limit);

// set on-tick callback
void counter_on_tick(counter_t *counter, void (*callback)(uint32_t));

// get current tick of counter
uint32_t counter_get(counter_t *counter);

// tick counter
void counter_tick(counter_t *counter);

// force reset counter, does not call on-tick callback
void counter_reset(counter_t *counter);

#endif
