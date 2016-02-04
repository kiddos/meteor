#ifndef SHIP_H
#define SHIP_H

#include "common.h"
#include "bullet.h"
#include "meteor.h"

/* constants */
extern const double SHIP_SIZE;
extern const double SHIP_FORCE;
extern const double SHIP_MAX_SPEED;
extern const double SHIP_STARTING_LIVES;
extern const double SHIP_STARTING_DAMAGE;
extern const double SHIP_MAX_MANA;
extern const double SHIP_BULLET_MANA;
extern const double SHIP_MANA_GAIN;

typedef enum {
  UP, DOWN, LEFT, RIGHT
} ship_direction;

typedef struct ship_attr_t {
  uint8_t lives;
  double mana;
  uint32_t level;
  double damage;
  bool is_buffed;
  bool is_immune;
} ship_attr;

typedef struct ship_t {
  ALLEGRO_BITMAP *bitmap;
  bullet *bullets;
  size_t bullet_count;
  point center;
  velocity v;
  acceleration a;
  double speed, direction;
  ship_attr attr;
} ship;

ship *ship_init(const point start);
void ship_update(ship *s, const size window_size);
void ship_move(ship *s, ship_direction d);
void ship_stop(ship *s, ship_direction d);
int32_t ship_get_lives(const ship *s);
int32_t ship_get_level(const ship *s);
double ship_get_mana(const ship *s);
void ship_increase_level(ship *s);
void ship_increase_mana(ship *s, const double mana);
void ship_shoot_bullet(ship *s);
bool ship_collide_with_meteor(const ship *s, const meteor *m);
bool ship_check_bullet_hit(const ship *s,
                           meteor_shower *ms,
                           const size window_size);
bool ship_check_collision(const ship *s,
                          meteor_shower *ms,
                          const size window_size);
void ship_draw(const ship *s);
void ship_destroy(ship *s);

#endif /* end of include guard: SHIP_H */

