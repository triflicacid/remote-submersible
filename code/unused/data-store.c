#include "data-store.h"

#include <stddef.h>
#include <string.h>

void data_store_init(data_store *store, void *buffer, uint32_t size) {
  store->size = size;
  store->buffer = buffer;
  store->on_change = NULL;
}

void data_store_on_change(data_store *store, void (*callback)(data_store *, uint32_t)) {
  store->on_change = callback;
}

bool data_store_overwrite(data_store *store, void *new_buffer, bool silent) {
  // check if any byte is different
  for (uint32_t i = 0; i < store->size; i++) {
    if (((uint8_t *) store->buffer)[i] != ((uint8_t *) new_buffer)[i]) {
      // update old buffer
      memcpy(store->buffer, new_buffer, store->size);

      // invoke handler
      if (store->on_change && !silent) {
        store->on_change(store, i);
      }

      return true;
    }
  }

  return false;
}

void *data_store_get(data_store *store, uint32_t offset) {
  return (uint8_t *) store->buffer + offset;
}

bool data_store_set(data_store *store, uint32_t offset, void *data, uint32_t data_size, bool silent) {
  // check if any byte is different in data
  for (uint32_t i = 0; i < data_size; i++) {
    if (((uint8_t *) store->buffer + offset)[i] != ((uint8_t *) data)[i]) {
      // update changed bytes
      memcpy((uint8_t *) store->buffer + offset, data, data_size);

      // invoke handler
      if (store->on_change && !silent) {
        store->on_change(store, offset + i);
      }

      return true;
    }
  }

  return false;
}
