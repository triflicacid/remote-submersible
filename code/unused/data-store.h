#ifndef _DATA_STORE_H_
#define _DATA_STORE_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct data_store data_store;

struct data_store {
  uint32_t size; // number of bytes in store, size of buffer
  void *buffer;
  void (*on_change)(data_store *, uint32_t); // callback invoked on data change, state first byte which changed
};

// initialise a data store with an existing buffer
void data_store_init(data_store *store, void *buffer, uint32_t size);

// set a data store's on change handler
void data_store_on_change(data_store *store, void (*callback)(data_store *, uint32_t));

// insert new data buffer to store (assumed size equal to `store->size`)
// invoke handler if different and return flag
bool data_store_overwrite(data_store *store, void *new_buffer, bool silent);

// get pointer to data at offset
void *data_store_get(data_store *store, uint32_t offset);

// same as `data_store_overwrite`, but overwrites at a position
bool data_store_set(data_store *store, uint32_t offset, void *data, uint32_t data_size, bool silent);


#endif
