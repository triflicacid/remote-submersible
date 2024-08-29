#ifndef _TIMED_LOCK_H_
#define _TIMED_LOCK_H_

#include <stdint.h>

typedef struct {
  uint32_t last; // ticks of last call
  uint16_t waitfor; // ticks to wait for unit next call
  void (*callback)(void); // callback function on success
} timed_lock_t;

// initialise lock to wait for given ticks before next call
void timed_lock_init(timed_lock_t *lock, uint16_t waitfor, void (*callback)(void));

// reset lock
void timed_lock_reset(timed_lock_t *lock);

// attempt to call a lock's callback, given the ticks elapsed (absolute)
void timed_lock_call(timed_lock_t *lock, uint32_t elapsed);

#endif
