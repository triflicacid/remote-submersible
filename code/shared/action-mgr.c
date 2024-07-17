#include <stdlib.h>

#include "action-mgr.h"

typedef struct action_node action_node;

struct action_node {
  handler handler;  // action handler
  action_node *next; // point to next action_node in list
};

// linked list of all pending actions.
static volatile action_node *pending_actions = NULL;

void create_action(handler action_handler) {
  // create new action node, refering the given handler
  action_node *node = malloc(sizeof(*node));
  node->handler = action_handler;
  node->next = pending_actions;
  
  // insert at start of linked list
  pending_actions = node;
}

int count_pending_actions(void) {
  int count = 0;
  action_node *current = pending_actions;
  
  while (current) {
    count++;
    current = current->next;
  }
  
  return count;
}

handler *next_action(void) {
  if (pending_actions) {
    // save current action node
    action_node *action = pending_actions;
    
    // shift pointer to next action node
    pending_actions = action->next;
    
    // extract action handler before freeing current node
    handler action_handler = action->handler;
    free(action);
    
    return action_handler;
  }
  
  return NULL;
}

void execute_pending_actions(void) {
  action_node *current = NULL, *next = pending_actions;
  
  while (current = next) {
    // run action handler
    current->handler();
    
    // set pointer to next action node
    next = current->next;
    
    // free current node from memory
    free(current);
  }
  
  pending_actions = NULL;
}
