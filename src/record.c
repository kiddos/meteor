#include "record.h"

#define RECORD_ENTRY_FORMAT "%s\t%d\n"
#define RECORD_NAME_BUFFER_SIZE 128
#define RECORD_NAME_TITLE "NAME"
#define RECORD_SCORE_TITLE "SCORE"

const char * const RECORD_FILE_PATH = "res/record.data";
const uint32_t RECORD_FONT_SIZE = 16;
const uint32_t RECORD_MAX_DISPLAY_COUNT = 10;
const double RECORD_ENTRY_PADDING = 15;

static bool read_line(FILE *record_file,
                      char name[RECORD_NAME_BUFFER_SIZE],
                      uint32_t *score) {
  uint32_t i = 0;
  char c;
  char score_buffer[64];

  memset(name, '\0', sizeof(char) * RECORD_NAME_BUFFER_SIZE);
  while ((c = fgetc(record_file)) && i < RECORD_NAME_BUFFER_SIZE) {
    if (c != '\t' && c != EOF) {
      name[i ++] = c;
    } else if (c == EOF) {
      return false;
    } else {
      break;
    }
  }

  i = 0;
  while ((c = fgetc(record_file)) && i < RECORD_NAME_BUFFER_SIZE) {
    if (c != '\n' && c != EOF) {
      score_buffer[i ++] = c;
    } else if (c == EOF) {
      return false;
    } else {
      break;
    }
  }

  *score = atoi(score_buffer);
  return true;
}

record *record_init(const char * const font_path,
                    const ALLEGRO_COLOR text_color,
                    const point center,
                    const size window_size) {
  char name[128];
  uint32_t score;
  FILE *record_file = NULL;
  record *r = (record *) malloc(sizeof(record));
  r->entries = NULL;
  r->entry_count = 0;


  record_file = fopen(RECORD_FILE_PATH, "r");
  if (record_file) {
    while (!feof(record_file)) {
      if (!read_line(record_file, name, &score))
        break;
      record_insert(r, name, score);
    }

    fclose(record_file);
  }

  r->center = center;
  r->area = size_init(window_size.w / 2, window_size.h * 0.8);
  r->font = al_load_font(font_path, RECORD_FONT_SIZE,
                         ALLEGRO_TTF_NO_KERNING);
  if (!r->font) {
    error_message("fail to load font for record");
    free(r->entries);
    free(r);
    return NULL;
  }
  r->target_color = text_color;
  r->display_color = al_map_rgba(0, 0, 0, 0);

  return r;
}

void record_insert(record *r, const char * const name, const uint32_t score) {
  if (r != NULL) {
    const size_t name_length = sizeof(char) * (strlen(name) + 1);

    r->entry_count ++;
    if (r->entries == NULL) {
      r->entries = malloc(sizeof(entry));
      r->entries->name = malloc(name_length);
      memset(r->entries->name, '\0', name_length);
      strcpy(r->entries->name, name);
      r->entries->score = score;
    } else {
      r->entries = realloc(r->entries, sizeof(entry) * r->entry_count);
      r->entries[r->entry_count - 1].name = malloc(name_length);
      memset(r->entries[r->entry_count - 1].name, '\0', name_length);
      strcpy(r->entries[r->entry_count - 1].name, name);
      r->entries[r->entry_count - 1].score = score;
    }

    record_save(r);
  }
}

uint32_t record_get_score(const record *r, const char * const name) {
  if (r != NULL) {
    uint32_t i;
    for (i = 0 ; i < r->entry_count ; i ++) {
      if (strcpy(r->entries[i].name, name) == 0) {
        return r->entries[i].score;
      }
    }
    error_message("no such record found for name given");
    return -1;
  } else {
    error_message("record null pointer");
    return -1;
  }
}

const entry *recore_get_entry(record *r, const size_t index) {
  if (r != NULL) {
    uint32_t i;
    for (i = 0 ; i < r->entry_count ; i ++) {
      if (i == index) {
        return r->entries + i;
      }
    }
  } else {
    error_message("record object null pointer");
    return NULL;
  }
  return NULL;
}

void record_remove_entry(record *r, const size_t index) {
  if (r != NULL) {
    entry *temp = NULL;
    if (index < r->entry_count) {
      const size_t temp_size = sizeof(entry) * (r->entry_count - index - 1);
      uint32_t i;

      temp = (entry *) malloc(temp_size);
      memcpy(temp, r->entries + index + 1, temp_size);

      r->entry_count --;
      r->entries = realloc(r->entries, sizeof(entry) * r->entry_count);
      for (i = 0 ; i < r->entry_count - index ; i ++) {
        r->entries[i + index] = temp[i];
      }
    }
  }
}

void record_sort_entries(record *r) {
  if (r != NULL) {
    uint32_t i, j;
    for (i = 0 ; i < r->entry_count ; i ++) {
      for (j = i + 1 ; j < r->entry_count ; j ++) {
        if (r->entries[i].score < r->entries[j].score) {
          char *temp_name = r->entries[i].name;
          int temp_score = r->entries[i].score;

          r->entries[i].name = r->entries[j].name;
          r->entries[i].score = r->entries[j].score;

          r->entries[j].name = temp_name;
          r->entries[j].score = temp_score;
        }
      }
    }
  } else {
    error_message("record null pointer");
  }
}

void record_save(const record *r) {
  if (r != NULL) {
    FILE *record_file = fopen(RECORD_FILE_PATH, "w");
    uint32_t i;
    if (record_file) {
      for (i = 0 ; i < r->entry_count ; i ++) {
        fprintf(record_file, RECORD_ENTRY_FORMAT,
                r->entries[i].name, r->entries[i].score);
      }
      fclose(record_file);
    }
  }
}

void record_reset(record *r) {
  if (r != NULL) {
    r->display_color = al_map_rgba(0, 0, 0, 0);
  } else {
    error_message("record null pointer");
  }
}

void record_update(record *r, const size window_size) {
  unsigned char dr = 0, dg = 0, db = 0, da = 0;
  unsigned char tr = 0, tg = 0, tb = 0, ta = 0;

  al_unmap_rgba(r->display_color, &dr, &dg, &db, &da);
  al_unmap_rgba(r->target_color, &tr, &tg, &tb, &ta);

  if (dr < tr)
    dr ++;
  if (dg < tg)
    dg ++;
  if (db < tb)
    db ++;
  if (da < ta)
    da ++;

  r->display_color = al_map_rgba(dr, dg, db, da);

  r->area = size_init(window_size.w / 2, window_size.h * 0.8);
}

void record_draw(const record *r) {
  uint32_t i;
  char score_buffer[128];
  // title
  al_draw_text(r->font, r->display_color,
               r->center.x - r->area.w / 2,
               r->center.y,
               ALLEGRO_ALIGN_LEFT, RECORD_NAME_TITLE);

  al_draw_text(r->font, r->display_color,
               r->center.x + r->area.w / 2,
               r->center.y,
               ALLEGRO_ALIGN_RIGHT, RECORD_SCORE_TITLE);

  const uint32_t max = r->entry_count > RECORD_MAX_DISPLAY_COUNT ?
        RECORD_MAX_DISPLAY_COUNT : r->entry_count;
  for (i = 0 ; i < max ; i ++) {
    memset(score_buffer, '\0', 128);
    snprintf(score_buffer, 128, "%d", r->entries[i].score);

    al_draw_text(r->font, r->display_color,
                r->center.x - r->area.w / 2,
                r->center.y + (i + 1) * RECORD_ENTRY_PADDING,
                ALLEGRO_ALIGN_LEFT, r->entries[i].name);

    al_draw_text(r->font, r->display_color,
                r->center.x + r->area.w / 2,
                r->center.y + (i + 1) * RECORD_ENTRY_PADDING,
                ALLEGRO_ALIGN_RIGHT, score_buffer);
  }
}

void record_destroy(record *r) {
  if (r != NULL) {
    uint32_t i;
    for (i = 0 ; i < r->entry_count ; i ++) {
      free(r->entries[i].name);
    }
    free(r->entries);
    free(r);
  }
}

#undef RECORD_ENTRY_FORMAT
#undef RECORD_NAME_BUFFER_SIZE
#undef RECORD_NAME_TITLE
#undef RECORD_SCORE_TITLE

