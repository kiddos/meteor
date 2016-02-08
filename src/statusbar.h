#ifndef STATUSBAR_H
#define STATUSBAR_H

#include "common.h"
#include "timedisplay.h"
#include "ship.h"
#include "meteor.h"
#include "score.h"

/* constants */
extern const double STATUS_BAR_HEIGHT;
extern const double STATUS_BAR_PADDING;
extern const uint32_t STATUS_BAR_FONT_SIZE;
extern const double STATUS_BAR_LIVE_ICON_SIZE;
extern const double STATUS_BAR_MANA_BAR_HIEGHT;
extern const double STATUS_BAR_MANA_BAR_MAX_LENGTH;
extern const char * const STATUS_BAR_LIVE_ICON_FILE_PATH;

typedef struct status_bar_t {
  ALLEGRO_FONT *font;
  ALLEGRO_BITMAP *live_icon;
  ALLEGRO_COLOR background_color;
  point start;
  size area;

  struct {
    time_display *td;
    score *s;
  } display;

  struct {
    ship *s;
    meteor_shower *ms;
  } object;
} status_bar;

status_bar *status_bar_init(ship *s, meteor_shower *ms,
                            const size window_size);
void status_bar_start(status_bar *sb);
void status_bar_reset(status_bar *sb);
void status_bar_set_new_ship(status_bar *sb, ship *s);
uint64_t status_bar_get_score(const status_bar *sb);
void status_bar_update(status_bar *sb, const size window_size);
void status_bar_draw(status_bar *sb);
void status_bar_destroy(status_bar *sb);

#endif /* end of include guard: STATUSBAR_H */
