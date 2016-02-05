#ifndef HELP_H
#define HELP_H

#include "common.h"

extern const char * const HELP_TEXT_FILE_PATH;
extern const uint32_t HELP_TEXT_FONT_SIZE;
extern const double HELP_TEXT_PADDING;

typedef struct help_t {
  ALLEGRO_FONT *font;
  ALLEGRO_COLOR target_color, display_color;
  char **text;
  size_t line_count;
  point left;
} help;

help *help_init(const char * const text_font,
                const ALLEGRO_COLOR text_color,
                const point center);
void help_reset(help *h);
void help_update(help *h);
void help_draw(const help *h);
void help_destroy(help *h);

#endif /* end of include guard: HELP_H */

