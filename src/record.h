#ifndef RECORD_H
#define RECORD_H

#include "common.h"

extern const char * const RECORD_FILE_PATH;
extern const uint32_t RECORD_FONT_SIZE;
extern const uint32_t RECORD_MAX_DISPLAY_COUNT;
extern const double RECORD_ENTRY_PADDING;

typedef struct entry_t {
  char *name;
  uint32_t score;
} entry;

typedef struct record_t {
  entry *entries;
  size_t entry_count;
  point center;
  size area;
  ALLEGRO_FONT *font;
  ALLEGRO_COLOR target_color, display_color;
} record;

record *record_init(const char * const font_path,
                    const ALLEGRO_COLOR text_color,
                    const point center,
                    const size window_size);
void record_insert(record *r, const char * const name, const uint32_t score);
uint32_t record_get_score(const record *r, const char * const name);
const entry *recore_get_entry(record *r, const size_t index);
void record_remove_entry(record *r, const size_t index);
void record_sort_entries(record *r);
void record_save(const record *r);
void record_reset(record *r);
void record_update(record *r, const size window_size);
void record_draw(const record *r);
void record_destroy(record *r);

#endif /* end of include guard: RECORD_H */

