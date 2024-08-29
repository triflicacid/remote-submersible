#include "timed-lock.h"

void timed_lock_init(timed_lock_t *lock, uint16_t waitfor, void (*callback)(void)) {
  lock->callback = callback;
  lock->last = 0;
  lock->waitfor = waitfor;
}

void timed_lock_reset(timed_lock_t *lock) {
  lock->last = 0;
}

void timed_lock_call(timed_lock_t *lock, uint32_t elapsed) {
  // ensure sufficient time has passed
  if (lock->last == 0 || lock->waitfor <= elapsed - lock->last) {
    lock->last = elapsed;
    lock->callback();
  }
}
