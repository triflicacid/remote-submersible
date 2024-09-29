#ifndef _STORED_CODE_H_
#define _STORED_CODE_H_

#include <stdint.h>

typedef uint32_t code_t;

// retrieve saved code at given index
code_t fetch_code(void);

// save code to storage at given index
void save_code(code_t code);

#endif
