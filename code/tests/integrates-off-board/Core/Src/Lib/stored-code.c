#include "stored-code.h"

static volatile code_t _code;

code_t fetch_code(void) {
  return _code;
}

void save_code(code_t code) {
  _code = code;
}
