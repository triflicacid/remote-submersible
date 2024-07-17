#ifndef _STORED_CODE_H_
#define _STORED_CODE_H_

#include <stdint.h>

typedef uint32_t code_t;

#ifndef STORED_CODE_COUNT
// number of stored codes (uint8_t)
#define STORED_CODE_COUNT 1
#endif

// retrieve saved code at given index
code_t fetch_code(uint8_t index);

// save code to storage at given index
void save_code(uint8_t index, code_t code);

#endif
