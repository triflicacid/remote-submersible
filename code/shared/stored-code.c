#include "stored-code.h"

static volatile code_t codes[STORED_CODE_COUNT] = {0};

code_t fetch_code(uint8_t index) {
  return codes[index];
}

void save_code(uint8_t index, code_t code) {
  codes[index] = code;
}
