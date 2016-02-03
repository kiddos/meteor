#include "score.h"

#define SCORE_TEXT_FORMAT "SCORE:  %010lu"
#define SCORE_DELTA 0.5

/* constants */
const uint32_t SCORE_TIME_VALUE = 1;
const uint32_t SCORE_METEOR_SMALL_VALUE = 10;
const uint32_t SCORE_METEOR_MEDIUM_VALUE = 30;
const uint32_t SCORE_METEOR_LARGE_VALUE = 60;
const uint32_t SCORE_BUFF_VALUE = 10;

score *score_init(ALLEGRO_FONT *font,
                  const ALLEGRO_COLOR text_color,
                  const point start) {
  if (font != NULL) {
    score *s = (score *) malloc(sizeof(score));
    s->font = font;
    s->text_color = text_color;
    s->target_score = 0;
    s->display_score = 0;
    s->start = start;
    memset(s->text, '\0', 64);
    return s;
  } else {
    error_message("font null pointer");
    return NULL;
  }
}

void score_set_point(score *s, const point start) {
  if (s != NULL) {
    s->start = start;
  }
}

void score_add_score_type(score *s, const score_type type) {
  if (s != NULL) {
    switch (type) {
      case SCORE_TIME:
        s->target_score += SCORE_TIME_VALUE;
        break;
      case SCORE_METEOR_SMALL:
        s->target_score += SCORE_METEOR_SMALL_VALUE;
        break;
      case SCORE_METEOR_MEDIUM:
        s->target_score += SCORE_METEOR_MEDIUM_VALUE;
        break;
      case SCORE_METEOR_LARGE:
        s->target_score += SCORE_METEOR_LARGE_VALUE;
        break;
      case SCORE_BUFF:
        s->target_score += SCORE_BUFF_VALUE;
        break;
    }
  } else {
    error_message("score object null pointer");
  }
}

void score_reset(score *s) {
  if (s != NULL) {
    s->target_score = 0;
    s->display_score = 0;

    sprintf(s->text, SCORE_TEXT_FORMAT, (uint64_t) s->display_score);
  } else {
    error_message("score object null pointer");
  }
}

void score_update(score *s) {
  if (s->display_score < s->target_score) {
    s->display_score += SCORE_DELTA;
  }
  sprintf(s->text, SCORE_TEXT_FORMAT, (uint64_t) round(s->display_score));
}

void score_draw(score *s) {
  al_draw_text(s->font, s->text_color,
               s->start.x, s->start.y,
               ALLEGRO_ALIGN_LEFT, s->text);
}

void score_destroy(score *s) {
  if (s != NULL) {
    if (s->font)
      al_destroy_font(s->font);

    free(s);
    regular_message("destroy score object");
  }
}

