#include "timed-events.h"

struct timed_event_t {
  bool enabled; // enabled flag
  uint64_t elapsed; // measure elapsed time units
  uint64_t target; // measure target timeout time units
  uint32_t counter; // count times invoked
  void (*handler)(timed_event_t *); // handler, called on completion

  uint8_t index; // index of parent array in `events`
  uint8_t index2; // index in `events[index]`

  void (*on_delete)(void *); // called on deletion, given this->data
  void *data;
};

// array of arrays of timed events
static volatile timed_event_t events[TIMED_EVENTS_LIST_COUNT][TIMED_EVENTS_LIST_CAPACITY];

// array of array sizes, where `sizes[i] = sizeof(events[i])`
static volatile uint8_t sizes[TIMED_EVENTS_LIST_COUNT] = {0};

// invoke an event handler and other necessary actions
static void invoke_handler(timed_event_t *event) {
  event->counter++;
  
  if (event->handler) {
    event->handler(event);
  }
}

void timed_events_clear(uint8_t list) {
  // move pointer to 0 (old entries will be overwritten)
  sizes[list] = 0;
}

uint8_t timed_events_count(uint8_t list) {
  return sizes[list];
}

void timed_events_iterate(uint8_t list_idx, void (*callback)(timed_event_t *)) {
  uint8_t size = sizes[list_idx];
  timed_event_t *list = events[list_idx];

  for (uint16_t i = 0; i < size; i++) {
    callback(list + i);
  }
}

timed_event_t *timed_event_create(uint8_t list, void (*handler)(timed_event_t *), uint64_t timeout) {
  return timed_event_create_with_data(list, handler, timeout, NULL, NULL);
}

timed_event_t *timed_event_create_with_data(uint8_t list, void (*handler)(timed_event_t *), uint64_t timeout, void *data, void (*on_delete)(void *)) {
  // check capacity
  if (sizes[list] >= TIMED_EVENTS_LIST_CAPACITY) {
    return NULL;
  }

  // get pointer to event structure
  timed_event_t *event = &events[list][sizes[list]++];

  // initialise fields
  event->target = timeout;
  event->handler = handler;
  event->index = list;
  event->on_delete = on_delete;
  event->data = data;
  
  // reset dynamic fields
  timed_event_reset(event);
  
  return event;
}

void timed_event_destroy(timed_event_t *event) {
  // if provided, call custom on-delete handler
  if (event->on_delete) {
    event->on_delete(event->data);
  }

  // get event list that we are located in, its size, and our index into it
  uint8_t list_idx = event->index, size = sizes[list_idx], idx = event->index2;
  timed_event_t *list = events[list_idx];

  // shift all events after this index back one place
  for (uint16_t i = idx + 1; i < size; i++) {
    // copy event to previous position
    list[i - 1] = list[i];

    // update event's index into `list`
    list[i - 1].index2--;
  }

  // decrease `list` size
  sizes[list_idx]--;
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

void timed_events_tick(uint8_t idx, uint32_t time) {
  uint8_t size = sizes[idx];
  timed_event_t *list = events[idx], *event;
  
  for (uint16_t i = 0; i < size; i++) {
    event = list + i;

    // increase elapsed time only if enabled
    if (event->enabled) {
      event->elapsed += time;
    }
  }
}

void timed_events_main(uint8_t idx) {
  uint8_t size = sizes[idx];
  timed_event_t *list = events[idx], *event;
  
  for (uint16_t i = 0; i < size; i++) {
    event = list + i;

    // invoke handler if enabled and complete
    if (event->enabled && event->elapsed >= event->target) {
      timed_event_stop(event, false);
    }
  }
}

void timed_event_get_counter(timed_event_t *event) {
  return event->counter;
}

void *timed_event_get_data(timed_event_t *event) {
  return event->data;
}

void timed_event_set_data(timed_event_t *event, void *data) {
  event->data = data;
}
