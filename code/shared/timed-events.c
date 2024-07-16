#include "timed-events.h"

#include <stdlib.h>

struct timed_event_t {
  bool enabled; // enabled flag
  uint32_t elapsed; // measure elapsed time (ms)
  uint32_t target; // measure target timeout time (ms)
  uint32_t counter; // count times invoked
  void (*handler)(timed_event_t *); // handler, called on completion
  bool is_registered; // check if event is registered
  void (*on_delete)(timed_event_t *); // called on deletion
  void *user_data;
};

typedef struct node node;

struct node {
  timed_event_t *event;
  node *next;
};

// keep record of all timed events
static volatile node *events = NULL;
static volatile int event_count = 0;

// invoke an event handler and other necessary actions
static void invoke_handler(timed_event_t *event) {
  event->counter++;
  
  if (event->handler) {
    event->handler(event);
  }
}

void timed_event_register(timed_event_t *event) {
  // check if already registered
  if (event->is_registered) {
    return;
  }

  // create new node
  node *node = malloc(sizeof(node));
  node->event = event;
  
  // make new head
  node->next = events;
  events = node;
  
  event_count++;
  event->is_registered = true;
}

bool timed_event_is_registered(timed_event_t *event) {
  return event->is_registered;
}

bool timed_event_deregister(timed_event_t *event) {
  // only continue if event is registered
  if (!event->is_registered) {
    return;
  }
  
  // check if event is in head
  if (events->event == event) {
    node *node = events;

    // update head to second item
    events = node->next;

    // free old node
    free(node);
    
    event_count--;
    event->is_registered = false;
    
    return true;
  }
  
  // iterate through list, keep track of previous item
  node *previous = events, *current = events;
  
  while (current = current->next) {
    // is this the event we are looking for?
    if (current->event == event) {
      // update previous' pointer to skip this item
      previous->next = current->next;

      // free old node
      free(current);
      
      event_count--;
      event->is_registered = false;
      
      return true;
    }
    
    // update previous pointer
    previous = current;
  }
  
  return false;
}

void timed_events_clear(void) {
  node *current = NULL, *next = events;
  
  while (current = next) {
    // point to next node in list
    next = current->next;
    
    // destroy event
    timed_event_destroy(current->event);
    
    // destroy node
    free(current);
  }
  
  // reset global variables
  events = NULL;
  event_count = 0;
}

int timed_events_count(void) {
  return event_count;
}

void timed_events_iterate(void (*callback)(timed_event_t *)) {
  node *current = events;
  
  while (current) {
    // invoke callback on event
    callback(current->event);
    
    // advance to next node
    current = current->next;
  }
}

timed_event_t *timed_event_create(void (*handler)(timed_event_t *), uint32_t target_ms) {
  return timed_event_create_with_data(handler, target_ms, NULL, NULL);
}

timed_event_t *timed_event_create_with_data(void (*handler)(timed_event_t *), uint32_t target_ms, void *user_data, void (*on_delete)(timed_event_t *)) {
  // initialise read-only fields
  timed_event_t *event = malloc(sizeof(timed_event_t));
  event->target = target_ms;
  event->handler = handler;
  event->is_registered = false;
  event->on_delete = on_delete;
  event->user_data = user_data;
  
  // reset dynamic fields
  timed_event_reset(event);
  
  return event;
}

void timed_event_destroy(timed_event_t *event) {
  // if provided, call custom on-delete handler
  if (event->on_delete) {
    event->on_delete(event);
  }
  
  // de-register event
  timed_event_deregister(event);
  
  // free event
  free(event);
}

void timed_event_reset(timed_event_t *event) {
  event->enabled = false;
  event->elapsed = 0;
  event->counter = 0;
}

void timed_event_start(timed_event_t *event) {
  event->enabled = true;
  event->elapsed = 0;
}

void timed_event_stop(timed_event_t *event, bool silent) {
  event->enabled = false;
  event->elapsed = 0;
  
  // if not silent, call handler etc.
  if (!silent) {
    invoke_handler(event);
  }
}

void timed_event_prime(timed_event_t *event) {
  event->enabled = true;
  event->elapsed = event->target;
}

void timed_events_tick(uint32_t time_ms) {
  node *current = events;
  
  while (current) {
    // increase elapsed time only if enabled
    if (current->event->enabled) {
      current->event->elapsed += time_ms;
    }
    
    // point to next item
    current = current->next;
  }
}

void timed_events_guard_tick(uint32_t time_ms, bool (*guard)(timed_event_t *)) {
  node *current = events;
  
  while (current) {
    // increase elapsed time only if enabled and passes guard
    if (current->event->enabled && guard(current->event)) {
      current->event->elapsed += time_ms;
    }
    
    // point to next item
    current = current->next;
  }
}

void timed_events_main(void) {
  node *current = events;
  timed_event_t *event;
  
  while (current) {
    event = current->event;
    
    // invoke handler if enabled and complete
    if (event->enabled && event->elapsed >= event->target) {
      invoke_handler(event);
    }
    
    // point to next item
    current = current->next;
  }
}

void timed_events_guard_main(bool (*guard)(timed_event_t *)) {
  node *current = events;
  timed_event_t *event;
  
  while (current) {
    event = current->event;
    
    // invoke handler if enabled and complete and passes guard
    if (event->enabled && event->elapsed >= event->target && guard(event)) {
      invoke_handler(event);
    }
    
    // point to next item
    current = current->next;
  }
}

void timed_event_get_counter(timed_event_t *event) {
  return event->counter;
}

void *timed_event_get_data(timed_event_t *event) {
  return event->user_data;
}
