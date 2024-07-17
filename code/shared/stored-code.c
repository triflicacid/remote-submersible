#include "stored-code.h"

#ifdef STORED_CODE_INITIAL
static volatile code_t _code = STORED_CODE_INITIAL;
#else
static volatile code_t _code;
#endif

code_t fetch_code(void) {
  return _code;
}

void save_code(code_t code) {
  _code = code;
}
