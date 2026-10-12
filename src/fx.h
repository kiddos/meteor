#ifndef FX_H
#define FX_H

#include "common.h"

#define STAR_COUNT 80
#define PARTICLE_MAX 150

typedef struct star_t {
  double x, y;
  double speed;
  double size;
  ALLEGRO_COLOR color;
} star;

typedef struct particle_t {
  double x, y;
  double dx, dy;
  double life;
  double max_life;
  ALLEGRO_COLOR color;
  bool active;
} particle;

typedef struct fx_system_t {
  star stars[STAR_COUNT];
  particle particles[PARTICLE_MAX];
  uint32_t combo_count;
  double combo_timer;
} fx_system;

fx_system *fx_system_init(size window_size);
void fx_system_spawn_explosion(fx_system *fx, point pos, ALLEGRO_COLOR color, int count);
void fx_system_add_combo(fx_system *fx);
uint32_t fx_system_get_combo(const fx_system *fx);
void fx_system_update(fx_system *fx, size window_size);
void fx_system_draw(const fx_system *fx, ALLEGRO_FONT *font, size window_size);
void fx_system_destroy(fx_system *fx);

#endif /* FX_H */
