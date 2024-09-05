#ifndef _DEPTH_H_
#define _DEPTH_H_

#include <stdint.h>

// get current depth direction as `TRISTATE_*`
int8_t get_vert_dir(void);

// update stored depth direction as `TRISTATE_*`
void set_vert_dir(int8_t dir);

// get estimated depth in metres
double estimate_depth(void);

// increase time spent in current direction by given time in seconds
void inc_time_in_dir(double t);

#endif
