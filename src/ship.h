#ifndef SHIP_H
#define SHIP_H

#include "common.h"
#include "bullet.h"

/* constants */
extern const double SHIP_SIZE;
extern const double SHIP_FORCE;
extern const double SHIP_MAX_SPEED;

typedef enum {
  UP, DOWN, LEFT, RIGHT
} ship_direction;

typedef struct ship_t {
  ALLEGRO_BITMAP *bitmap;
  bullet *bullets;
  size_t bullet_count;
  point center;
  velocity v;
  acceleration a;
  double speed, direction;
  uint32_t level;
} ship;

ship *ship_init(const point start);
void ship_update(ship *s, const size window_size);
void ship_move(ship *s, ship_direction d);
void ship_stop(ship *s, ship_direction d);
void ship_shoot_bullet(ship *s);
bool ship_detect_collision(ship *s, const double x, const double y);
void ship_draw(ship *s);
void ship_destroy(ship *s);

#endif /* end of include guard: SHIP_H */

