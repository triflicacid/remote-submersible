#ifndef _ACTION_MGR_H_
#define _ACTION_MGR_H_

#ifndef MAX_PENDING_ACTIONS
// maximum number of pending action handlers
// if `create_action` exceeds this limit, action is ignored
// each handler has size 8 bytes
#define MAX_PENDING_ACTIONS 16
#endif

#include <stdint.h>
#include <stdbool.h>

typedef void (*handler)(void);

// create a new action with the given handler; add to pending action list
// return if added to pending list successfully (i.e., if at capacity)
bool create_action(handler action_handler);

// count number of pending actions
uint32_t count_pending_actions(void);

// get the next action in the pending action list and remove from list
handler next_action(void);

// execute all pending actions.
void execute_pending_actions(void);

#endif
