#ifndef METEOR_H
#define METEOR_H

#include "common.h"

/* constants */
extern const double METEOR_MIN_SPEED;
extern const double METEOR_MAX_SPEED;
extern const double METEOR_MIN_COUNT;
extern const double METEOR_MAX_COUNT;
extern const double METEOR_SMALL_SIZE;
extern const double METEOR_MEDIUM_SIZE;
extern const double METEOR_LARGE_SIZE;
extern const double METEOR_ROTATION_SPEED;

typedef enum {
  METEOR_SMALL,
  METEOR_MEDIUM,
  METEOR_LARGE
} meteor_size;

typedef struct meteor_t {
  ALLEGRO_BITMAP *bitmap;
  point center;
  velocity v;
  double speed, direction;
  double angle, w;
  meteor_size size;
  struct meteor_t *next;
} meteor;

typedef struct meteor_shower_t {
  meteor *meteors;
  size_t meteor_count;
} meteor_shower;

meteor *meteor_init(const size window_size);
bool meteor_is_inside_screen(meteor *m, const size window_size);
double meteor_get_size(meteor *m);
void meteor_update(meteor *m, const size window_size);
void meteor_draw(meteor *m);
void meteor_destroy(meteor *m);

meteor_shower *meteor_shower_init(const size window_size);
void meteor_shower_add_meteor(meteor_shower *ms, const size window_size);
void meteor_shower_update(meteor_shower *ms, const size window_size);
void meteor_shower_draw(meteor_shower *ms);
void meteor_shower_destroy(meteor_shower *ms);

#endif /* end of include guard: METEOR_H */
