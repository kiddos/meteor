#ifndef TIMER_H
#define TIMER_H

#include "common.h"

typedef struct time_display_t {
  ALLEGRO_FONT *font;
  ALLEGRO_COLOR text_color;
  bool updating;
  double start_time, time_passed;
  point start;
  char text[16];
} time_display;

time_display *time_display_init(ALLEGRO_FONT *font,
                                ALLEGRO_COLOR text_color,
                                point center);
void time_display_start(time_display *td);
void time_display_stop(time_display *td);
void time_display_update(time_display *td);
void time_display_set_point(time_display *td, point start);
const double time_display_get_text_width(const time_display *td);
void time_display_draw(time_display *td);
double time_display_get_time_passed(time_display *td);
void time_display_destroy(time_display *td);

#endif /* end of include guard: TIMER_H */

