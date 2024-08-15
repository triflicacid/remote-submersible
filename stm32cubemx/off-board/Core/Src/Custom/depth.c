#include "depth.h"
#include "constants.h"
#include "../Lib/util.h"

#ifdef PREDICT_DEPTH

// current platform direction
static volatile tristate_t _dir = TRISTATE_UNDEF;

// time spent in this direction (s)
static volatile double _time = 0;

// estimated current depth (m)
static volatile double _depth = 0;

// estimate depth change given time in this state
static double calc_delta(void) {
  // TODO improve? is SUVAT good enought?
  // s = 1/2 * a * t^2
  return 0.5 * _time * _time * PLATFORM_ACCELERATION;
}

int8_t get_vert_dir(void) {
  return _dir;
}

void set_vert_dir(int8_t dir) {
  // apply delta to depth
  _depth = estimate_depth();

  // change direction
  _dir = dir;
}

double estimate_depth(void) {
  // mid=hover, so delta makes no difference
  if (_dir == TRISTATE_UNDEF) {
    return _depth;
  }

  // calculate depth change due to `_dir` for `_time`
  double delta = calc_delta();

  // clamp at zero
  if (_dir == TRISTATE_TRUE && delta > _depth) {
    return 0;
  }

  // otherwise, apply delta in direction
  return _depth + delta * _dir;
}

void inc_time_in_dir(double t) {
  _time += t;
}

#endif
