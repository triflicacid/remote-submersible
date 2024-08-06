#include "counter.h"

#include <stddef.h>

void counter_init(counter_t *counter, uint32_t limit) {
  counter->limit = limit;
  counter->value = 0;
  counter->on_tick = NULL;
}

void counter_on_tick(counter_t *counter, void (*callback)(uint32_t)) {
  counter->on_tick = callback;
}

uint32_t counter_get(counter_t *counter) {
  return counter->value;
}

void counter_tick(counter_t *counter) {
  // increment current value, overflow if needed
  if (counter->value == counter->limit) {
    counter->value = 0;
  } else {
    counter->value++;
  }

  // invoke callback with new tick count
  if (counter->on_tick) {
    counter->on_tick(counter->value);
  }
}

void counter_reset(counter_t *counter) {
  counter->value = 0;
}
