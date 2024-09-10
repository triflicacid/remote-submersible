#include "util.h"

extern inline void write_pin(const pin_t *, bool);
extern inline void set_pin(const pin_t *);
extern inline void reset_pin(const pin_t *);
extern inline void toggle_pin(const pin_t *);
extern inline bool read_pin(const pin_t *);

void toggle_reset(const pin_t *pin, uint32_t final_delay) {
  set_pin(pin);
  HAL_Delay(1);

  reset_pin(pin);
  HAL_Delay(1);

  set_pin(pin);
  HAL_Delay(final_delay);
}

tristate_t read_tristate_pins(const pin_t *false_pin, const pin_t *true_pin) {
  if (read_pin(false_pin)) {
    return TRISTATE_FALSE;
  } else if (read_pin(true_pin)) {
    return TRISTATE_TRUE;
  } else {
    return TRISTATE_UNDEF;
  }
}

extern inline bool is_within_tolerance(int target, int value, int tolerance);

double map_range_decimal(uint32_t value, uint32_t old_min, uint32_t old_max, double new_min, double new_max) {
  if (value <= old_min) return new_min;
  if (value >= old_max) return new_max;

  double percentage = (double) (value - old_min) / (old_max - old_min);
  return percentage * (new_max - new_min) + new_min;
}

int32_t map_range_int(int32_t value, int32_t old_min, int32_t old_max, int32_t new_min, int32_t new_max) {
  if (value <= old_min) return new_min;
  if (value >= old_max) return new_max;

  double percentage = (double) (value - old_min) / (old_max - old_min);
  return percentage * (new_max - new_min) + new_min;
}
