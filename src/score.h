#ifndef SCORE_H
#define SCORE_H

#include "common.h"

/* constants */
extern const uint32_t SCORE_TIME_VALUE;
extern const uint32_t SCORE_METEOR_SMALL_VALUE;
extern const uint32_t SCORE_METEOR_MEDIUM_VALUE;
extern const uint32_t SCORE_METEOR_LARGE_VALUE;
extern const uint32_t SCORE_BUFF_VALUE;

typedef enum {
  SCORE_METEOR_SMALL,
  SCORE_METEOR_MEDIUM,
  SCORE_METEOR_LARGE,
  SCORE_BUFF
} score_type;

typedef struct score_t {
  ALLEGRO_FONT *font;
  ALLEGRO_COLOR text_color;
  double time_score;
  uint64_t game_score;
  long double total_score, display_score;
  point start;
  char text[64];
} score;

score *score_init(ALLEGRO_FONT *font,
                  const ALLEGRO_COLOR text_color,
                  const point start);
uint64_t score_get_total_score(const score *s);
void score_set_point(score *s, const point start);
void score_set_time_score(score *s, const double time_passed);
void score_add_score_type(score *s, const score_type type);
void score_reset(score *s);
void score_update(score *s);
void score_draw(score *s);
void score_destroy(score *s);

#endif /* end of include guard: SCORE_H */

