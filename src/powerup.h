#ifndef POWERUP_H
#define POWERUP_H

#include "common.h"

extern const double STATUS_BAR_HEIGHT;

typedef enum {
  POWERUP_HEALTH = 0,
  POWERUP_MANA,
  POWERUP_TRIPLE_SHOT,
  POWERUP_SHIELD,
  POWERUP_TYPE_COUNT
} powerup_type;

typedef struct powerup_t {
  ALLEGRO_BITMAP *bitmap;
  point center;
  velocity v;
  powerup_type type;
  double spawn_time;
  struct powerup_t *next;
} powerup;

typedef struct powerup_list_t {
  powerup *head;
} powerup_list;

powerup_list *powerup_list_init();
void powerup_list_spawn(powerup_list *pl, point pos);
void powerup_list_update(powerup_list *pl, size window_size);
void powerup_list_draw(const powerup_list *pl);
void powerup_list_destroy(powerup_list *pl);

#endif /* POWERUP_H */
