#ifndef _STORED_CODE_H_
#define _STORED_CODE_H_

#include <stdint.h>

typedef uint32_t code_t;

// retrieve saved code
code_t fetch_code(void);

// save code to storage
void save_code(code_t code);

#endif
