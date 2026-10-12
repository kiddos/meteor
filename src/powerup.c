#include "powerup.h"

#define POWERUP_BITMAP_PATH "res/images/buff.png"
#define POWERUP_SIZE 30.0
#define POWERUP_LIFESPAN 12.0

powerup_list *powerup_list_init() {
  powerup_list *pl = (powerup_list *) malloc(sizeof(powerup_list));
  if (pl) {
    pl->head = NULL;
  }
  return pl;
}

void powerup_list_spawn(powerup_list *pl, point pos) {
  if (!pl) return;
  // 35% chance to drop powerup when a meteor is destroyed
  if ((rand() % 100) > 35) return;

  powerup *p = (powerup *) malloc(sizeof(powerup));
  if (!p) return;

  p->bitmap = al_load_bitmap(POWERUP_BITMAP_PATH);
  p->center = pos;
  double angle = ((double) rand() / RAND_MAX) * 2.0 * M_PI;
  double speed = 0.8 + ((double) rand() / RAND_MAX) * 1.0;
  p->v = velocity_init(speed, angle);
  p->type = (powerup_type) (rand() % POWERUP_TYPE_COUNT);
  p->spawn_time = al_get_time();
  p->next = pl->head;
  pl->head = p;
}

void powerup_list_update(powerup_list *pl, size window_size) {
  if (!pl) return;
  powerup *curr = pl->head;
  powerup *prev = NULL;
  double now = al_get_time();

  while (curr) {
    curr->center.x += curr->v.dx;
    curr->center.y += curr->v.dy;

    // Bounce off walls
    if (curr->center.x < POWERUP_SIZE / 2 || curr->center.x > window_size.w - POWERUP_SIZE / 2) {
      curr->v.dx = -curr->v.dx;
    }
    if (curr->center.y < POWERUP_SIZE / 2 || curr->center.y > window_size.h - STATUS_BAR_HEIGHT - POWERUP_SIZE / 2) {
      curr->v.dy = -curr->v.dy;
    }

    bool expired = (now - curr->spawn_time) > POWERUP_LIFESPAN;
    if (expired) {
      powerup *to_free = curr;
      if (prev) {
        prev->next = curr->next;
        curr = curr->next;
      } else {
        pl->head = curr->next;
        curr = curr->next;
      }
      if (to_free->bitmap) al_destroy_bitmap(to_free->bitmap);
      free(to_free);
    } else {
      prev = curr;
      curr = curr->next;
    }
  }
}

void powerup_list_draw(const powerup_list *pl) {
  if (!pl) return;
  powerup *curr = pl->head;
  while (curr) {
    ALLEGRO_COLOR tint;
    switch (curr->type) {
      case POWERUP_HEALTH:
        tint = al_map_rgb(255, 80, 80); // Red
        break;
      case POWERUP_MANA:
        tint = al_map_rgb(80, 150, 255); // Blue
        break;
      case POWERUP_TRIPLE_SHOT:
        tint = al_map_rgb(255, 220, 50); // Yellow/Gold
        break;
      case POWERUP_SHIELD:
        tint = al_map_rgb(180, 80, 255); // Purple
        break;
      default:
        tint = color_white();
        break;
    }

    if (curr->bitmap) {
      uint32_t bw = al_get_bitmap_width(curr->bitmap);
      uint32_t bh = al_get_bitmap_height(curr->bitmap);
      al_draw_tinted_scaled_rotated_bitmap(curr->bitmap, tint,
                                           bw / 2.0, bh / 2.0,
                                           curr->center.x, curr->center.y,
                                           POWERUP_SIZE / bw, POWERUP_SIZE / bh,
                                           0, 0);
    } else {
      al_draw_filled_circle(curr->center.x, curr->center.y, POWERUP_SIZE / 2, tint);
    }
    curr = curr->next;
  }
}

void powerup_list_destroy(powerup_list *pl) {
  if (!pl) return;
  powerup *curr = pl->head;
  while (curr) {
    powerup *next = curr->next;
    if (curr->bitmap) al_destroy_bitmap(curr->bitmap);
    free(curr);
    curr = next;
  }
  free(pl);
}
