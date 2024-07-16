#ifndef _ACTION_MGR_H_
#define _ACTION_MGR_H_

typedef void (*handler)(void);

// create a new action with the given handler; add to pending action list
void create_action(handler action_handler);

// count number of pending actions
int count_pending_actions(void);

// get the next action in the pending action list and remove from list
handler *next_action(void);

// execute all pending actions.
void execute_pending_actions(void);

#endif
