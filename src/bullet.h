#ifndef BULLET_H
#define BULLET_H

#include "common.h"
#include "meteor.h"

/* constants */
extern const double BULLET_SPEED;
extern const double BULLET_WIDTH;
extern const double BULLET_LENGTH;

typedef struct bullet_t {
  point start;
  velocity v;
  double speed, direction;
  struct bullet_t *next;
} bullet;

bullet *bullet_init(const point start, const double direction);
void bullet_add(bullet *bullets, const point start, const double direction);
size_t bullet_get_count(bullet * const bullets);
bool bullet_is_inside_screen(bullet *b, const size window_size);
bool bullet_hit_meteor(bullet *b, const meteor *m);
bool bullet_check_collision(bullet *bullets, const meteor *m);
void bullet_update(bullet *bullets, const size window_size);
void bullet_draw(bullet *bullets);
void bullet_destroy(bullet *bullets);

#endif /* end of include guard: BULLET_H */

