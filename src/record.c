#include "record.h"

#define RECORD_ENTRY_FORMAT "%s\t%d\n"
#define RECORD_NAME_BUFFER_SIZE 128

const char * const RECORD_FILE_PATH = "res/record.data";

static bool read_line(FILE *record_file,
                      char name[RECORD_NAME_BUFFER_SIZE],
                      uint32_t *score) {
  uint32_t i = 0;
  char c;
  char score_buffer[64];
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

record *record_init() {
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
  }
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

