#ifndef _TIMED_EVENTS_H_
#define _TIMED_EVENTS_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct timed_event_t timed_event_t;

// clear all timed events (free's events)
void timed_events_clear(void);

// count registered events
uint32_t timed_events_count(void);

// call the callback on all events in list
// safe to destroy events in callback
void timed_events_iterate(void (*callback)(timed_event_t *));

// create new timed event
// event is created disabled; to enable, call function
timed_event_t *timed_event_create(void (*handler)(timed_event_t *), uint32_t target_ms);

// timed_event_create with user data and callback to be invoked on event destroy
timed_event_t *timed_event_create_with_data(void (*handler)(timed_event_t *), uint32_t target_ms, void *user_data, void (*on_delete)(timed_event_t *));

// register event so it will 'tick'
// NOTE, event cannot be registered twice
void timed_event_register(timed_event_t *event);

// check is event is registered to 'tick'
bool timed_event_is_registered(timed_event_t *event);

// de-register an event (opposite of _register), return if success
bool timed_event_deregister(timed_event_t *event);

// destroy given event (free's event)
void timed_event_destroy(timed_event_t *event);

// reset given event (factory reset)
void timed_event_reset(timed_event_t *event);

// start given event
void timed_event_start(timed_event_t *event);

// end given event
// silent=true: does not call handler nor increment count (can be considered abort/halt)
void timed_event_stop(timed_event_t *event, bool silent);

// prime event to be triggered next tick (enable & set elapsed=target)
void timed_event_prime(timed_event_t *event);

// increment all events by `time` ms
// NOTE does not trigger handlers; call _main
void timed_events_tick(uint32_t time_ms);

// same as timed_events_tick, but only ticks events which pass the `guard`
void timed_events_guard_tick(uint32_t time_ms, bool (*guard)(timed_event_t *));

// check all events, run handlers of all events which have timed out
void timed_events_main(void);

// same as timed_events_main, but only runs events which pass the `guard`
void timed_events_guard_main(bool (*guard)(timed_event_t *));

// get times given event has been invoked
void timed_event_get_counter(timed_event_t *event);

// get user data of an event
void *timed_event_get_data(timed_event_t *event);

#endif
