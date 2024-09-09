#include "action-mgr.h"

#include <stddef.h>

// array of pending action handlers
static volatile handler pending_actions[MAX_PENDING_ACTIONS] = {NULL};

// number of pending actions
static volatile uint32_t count = 0;

bool create_action(handler action_handler) {
  // check if at capacity
  if (count >= MAX_PENDING_ACTIONS) {
    return false;
  }

  // insert into array and increase size
  pending_actions[count++] = action_handler;

  return true;
}

uint32_t count_pending_actions(void) {
  return count;
}

handler next_action(void) {
  // get last item in list, or NULL if empty
  return count == 0 ? NULL : pending_actions[--count];
}

void execute_pending_actions(void) {
  // iterate over array and call handlers
  for (uint16_t i = 0; i < count; i++, count--) {
    pending_actions[i]();
  }
}
