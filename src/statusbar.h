#ifndef STATUSBAR_H
#define STATUSBAR_H

#include "common.h"
#include "timedisplay.h"
#include "ship.h"

/* constants */
extern const double STATUS_BAR_HEIGHT;
extern const double STATUS_BAR_PADDING;
extern const uint32_t STATUS_BAR_FONT_SIZE;

typedef struct status_bar_t {
  ALLEGRO_FONT *font;
  ALLEGRO_COLOR background_color;
  point start;
  size area;
  time_display *td;
  ship *s;
} status_bar;

status_bar *status_bar_init(ship *s, const size window_size);
void status_bar_start(status_bar *sb);
void status_bar_reset(status_bar *sb);
void status_bar_update(status_bar *sb, const size window_size);
void status_bar_draw(status_bar *sb);
void status_bar_destroy(status_bar *sb);

#endif /* end of include guard: STATUSBAR_H */
