#ifndef RECORD_H
#define RECORD_H

#include "common.h"

extern const char * const RECORD_FILE_PATH;

typedef struct entry_t {
  char *name;
  uint32_t score;
} entry;

typedef struct record_t {
  entry *entries;
  size_t entry_count;
} record;

record *record_init();
void record_insert(record *r, const char * const name, const uint32_t score);
uint32_t record_get_score(const record *r, const char * const name);
const entry *recore_get_entry(record *r, const size_t index);
void record_remove_entry(record *r, const size_t index);
void record_save(const record *r);
void record_destroy(record *r);

#endif /* end of include guard: RECORD_H */

