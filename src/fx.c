#include "fx.h"

fx_system *fx_system_init(size window_size) {
  fx_system *fx = (fx_system *) malloc(sizeof(fx_system));
  if (!fx) return NULL;

  for (int i = 0; i < STAR_COUNT; i++) {
    fx->stars[i].x = ((double) rand() / RAND_MAX) * window_size.w;
    fx->stars[i].y = ((double) rand() / RAND_MAX) * window_size.h;
    fx->stars[i].speed = 0.5 + ((double) rand() / RAND_MAX) * 2.5;
    fx->stars[i].size = 1.0 + ((double) rand() / RAND_MAX) * 2.0;
    int b = 150 + rand() % 106;
    fx->stars[i].color = al_map_rgb(b, b, 255);
  }

  for (int i = 0; i < PARTICLE_MAX; i++) {
    fx->particles[i].active = false;
  }

  fx->combo_count = 0;
  fx->combo_timer = 0;

  return fx;
}

void fx_system_spawn_explosion(fx_system *fx, point pos, ALLEGRO_COLOR color, int count) {
  if (!fx) return;
  int spawned = 0;
  for (int i = 0; i < PARTICLE_MAX && spawned < count; i++) {
    if (!fx->particles[i].active) {
      fx->particles[i].active = true;
      fx->particles[i].x = pos.x;
      fx->particles[i].y = pos.y;
      double angle = ((double) rand() / RAND_MAX) * 2.0 * M_PI;
      double speed = 1.0 + ((double) rand() / RAND_MAX) * 4.0;
      fx->particles[i].dx = cos(angle) * speed;
      fx->particles[i].dy = sin(angle) * speed;
      fx->particles[i].life = 0;
      fx->particles[i].max_life = 0.3 + ((double) rand() / RAND_MAX) * 0.5;
      fx->particles[i].color = color;
      spawned++;
    }
  }
}

void fx_system_add_combo(fx_system *fx) {
  if (!fx) return;
  fx->combo_count++;
  fx->combo_timer = al_get_time();
}

uint32_t fx_system_get_combo(const fx_system *fx) {
  if (!fx) return 1;
  return fx->combo_count > 0 ? fx->combo_count : 1;
}

void fx_system_update(fx_system *fx, size window_size) {
  if (!fx) return;

  // Update starfield
  for (int i = 0; i < STAR_COUNT; i++) {
    fx->stars[i].y += fx->stars[i].speed;
    if (fx->stars[i].y > window_size.h) {
      fx->stars[i].y = 0;
      fx->stars[i].x = ((double) rand() / RAND_MAX) * window_size.w;
    }
  }

  // Update particles
  for (int i = 0; i < PARTICLE_MAX; i++) {
    if (fx->particles[i].active) {
      fx->particles[i].x += fx->particles[i].dx;
      fx->particles[i].y += fx->particles[i].dy;
      fx->particles[i].life += 1.0 / 60.0;
      if (fx->particles[i].life >= fx->particles[i].max_life) {
        fx->particles[i].active = false;
      }
    }
  }

  // Update combo decay (2.5 second combo window)
  if (fx->combo_count > 0) {
    if (al_get_time() - fx->combo_timer > 2.5) {
      fx->combo_count = 0;
    }
  }
}

void fx_system_draw(const fx_system *fx, ALLEGRO_FONT *font, size window_size) {
  if (!fx) return;

  // Draw starfield
  for (int i = 0; i < STAR_COUNT; i++) {
    al_draw_filled_circle(fx->stars[i].x, fx->stars[i].y, fx->stars[i].size, fx->stars[i].color);
  }

  // Draw particles
  for (int i = 0; i < PARTICLE_MAX; i++) {
    if (fx->particles[i].active) {
      double alpha = 1.0 - (fx->particles[i].life / fx->particles[i].max_life);
      ALLEGRO_COLOR c = fx->particles[i].color;
      unsigned char r, g, b;
      al_unmap_rgb(c, &r, &g, &b);
      al_draw_filled_circle(fx->particles[i].x, fx->particles[i].y, 2.5, al_map_rgba(r, g, b, (unsigned char)(alpha * 255)));
    }
  }

  // Draw combo multiplier
  if (fx->combo_count > 1 && font) {
    char combo_str[32];
    snprintf(combo_str, sizeof(combo_str), "%dX COMBO!", fx->combo_count);
    al_draw_text(font, al_map_rgb(255, 215, 0), window_size.w / 2.0, 15, ALLEGRO_ALIGN_CENTER, combo_str);
  }
}

void fx_system_destroy(fx_system *fx) {
  if (fx) {
    free(fx);
  }
}
