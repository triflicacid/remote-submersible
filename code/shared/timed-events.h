#ifndef _TIMED_EVENTS_H_
#define _TIMED_EVENTS_H_

#include <stdint.h>
#include <stdbool.h>

// number of timed event lists
#define TIMED_EVENTS_LIST_COUNT 1

// maximum number of times events in each list
// each event is 47 bytes unaligned
// note, stack memory allocation is `list_count * (list_capacity * sizeof(timed_event_t) + sizeof(uint8_t))`
#define TIMED_EVENTS_LIST_CAPACITY 16

typedef struct timed_event_t timed_event_t;

// clear all timed events in given list (destroys events)
void timed_events_clear(uint8_t list);

// count registered events in given list
uint8_t timed_events_count(uint8_t list);

// call the callback on all events in list
// note, not safe to destroy events whilst iterating
void timed_events_iterate(uint8_t list, void (*callback)(timed_event_t *));

// create new timed event in the given list, or return NULL if at capacity
// event is created disabled; to enable, call function
timed_event_t *timed_event_create(uint8_t list, void (*handler)(timed_event_t *), uint64_t timeout);

// timed_event_create with user data and callback to be invoked on event destruction which takes said data as a parameter
timed_event_t *timed_event_create_with_data(uint8_t list, void (*handler)(timed_event_t *), uint64_t timeout, void *data, void (*on_delete)(void *));

// destroy given event (calls on-delete handler)
void timed_event_destroy(timed_event_t *event);

// reset given event (factory reset)
void timed_event_reset(timed_event_t *event);

// start given event
void timed_event_start(timed_event_t *event);

// end given event
// `silent=true`: does not call handler nor increment count (can be considered abort/halt)
void timed_event_stop(timed_event_t *event, bool silent);

// prime event to be triggered next tick (enable & set elapsed=target)
void timed_event_prime(timed_event_t *event);

// increment all events by `time` units
// NOTE does not trigger handlers; call _main
void timed_events_tick(uint8_t list, uint32_t time);

// check all events, run handlers of all events which have timed out
void timed_events_main(uint8_t list);

// get times given event has been invoked
void timed_event_get_counter(timed_event_t *event);

// get user data of an event
void *timed_event_get_data(timed_event_t *event);

// set user data of an event (on-delete handler is not called on old data)
void timed_event_set_data(timed_event_t *event, void *data);

#endif
