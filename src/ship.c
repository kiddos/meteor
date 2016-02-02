#include "ship.h"

/* configs */
#define CONFIG_FILE_PATH "config/ship.conf"
#define CONFIG_SHIP_SECTION "ship"
#define CONFIG_SHIP_BITMAP_KEY "bitmap"
#define CONFIG_SHIP_LEVEL_KEY "level"

/* constants */
const double SHIP_SIZE = 60;
const double SHIP_FORCE = 0.36;
const double SHIP_MAX_SPEED = 3.6;

ship *ship_init(const point start) {
  ALLEGRO_CONFIG *config = al_load_config_file(CONFIG_FILE_PATH);
  if (config != NULL) {
    ship *s = (ship *) malloc(sizeof(ship));
    s->center.x = start.x;
    s->center.y = start.y;
    s->v.dx = 0;
    s->v.dy = 0;
    s->a.ax = 0;
    s->a.ay = 0;
    s->direction = velocity_compute_direction(s->v);
    s->speed = velocity_compute_speed(s->v);
    s->bullets = NULL;
    s->bullet_count = 0;

    const char *bitmap_path = al_get_config_value(config,
                                                  CONFIG_SHIP_SECTION,
                                                  CONFIG_SHIP_BITMAP_KEY);

    s->bitmap = al_load_bitmap(bitmap_path);
    if (!s->bitmap) {
      error_message("fail to load ship bitmap");
      return NULL;
    }
    s->level = atoi(al_get_config_value(config,
                                        CONFIG_SHIP_SECTION,
                                        CONFIG_SHIP_LEVEL_KEY));
    return s;
  } else {
    error_message("fail to load ship config file");
    return NULL;
  }
}

void ship_update(ship *s, const size window_size) {
  const double ship_diag = SHIP_SIZE * sqrt(2) / 2;

  if (s->v.dx + s->a.ax < SHIP_MAX_SPEED &&
      s->v.dx + s->a.ax > -SHIP_MAX_SPEED) {
    s->v.dx += s->a.ax;
  }
  if (s->v.dy + s->a.ay < SHIP_MAX_SPEED &&
      s->v.dy + s->a.ay > -SHIP_MAX_SPEED) {
    s->v.dy += s->a.ay;
  }

  if (s->center.x - ship_diag + s->v.dx > 0 &&
      s->center.x + ship_diag + s->v.dx < window_size.w) {
    s->center.x += s->v.dx;
  }
  if (s->center.y - ship_diag + s->v.dy > 0 &&
      s->center.y + ship_diag + s->v.dy < window_size.h) {
    s->center.y += s->v.dy;
  }

  s->direction = velocity_compute_direction(s->v);
  s->speed = velocity_compute_speed(s->v);

  if (s->bullets != NULL) {
    bullet_update(s->bullets, window_size);
    s->bullet_count = bullet_get_count(s->bullets);

    bullet *temp = NULL;
    if (!bullet_is_inside_screen(s->bullets, window_size)) {
      regular_message("this bullet not inside screen, cleaning up");
      temp = s->bullets->next;
      free(s->bullets);
      s->bullets = temp;
      s->bullet_count --;
    }
  }
}

void ship_move(ship *s, ship_direction d) {
  switch (d) {
    case UP:
      s->a.ay = -SHIP_FORCE;
      break;
    case DOWN:
      s->a.ay = SHIP_FORCE;
      break;
    case LEFT:
      s->a.ax = -SHIP_FORCE;
      break;
    case RIGHT:
      s->a.ax = SHIP_FORCE;
      break;
  }
}

void ship_stop(ship *s, ship_direction d) {
  switch (d) {
    case UP:
      if (s->v.dy < 0) {
        s->a.ay = 0;
      }
      break;
    case DOWN:
      if (s->v.dy > 0) {
        s->a.ay = 0;
      }
      break;
    case LEFT:
      if (s->v.dx > 0) {
        s->a.ax = 0;
      }
      break;
    case RIGHT:
      if (s->v.dx > 0) {
        s->a.ax = 0;
      }
      break;
  }
}

void ship_shoot_bullet(ship *s) {
  const point start = point_init(s->center.x, s->center.y);
  if (s->bullets == NULL) {
    s->bullets = bullet_init(start, s->direction);
  } else {
    bullet_add(s->bullets, start, s->direction);
    s->bullet_count = bullet_get_count(s->bullets);
  }
}

bool ship_collide_with_meteor(ship *s, meteor *m) {
  const double dx = s->center.x - m->center.x;
  const double dy = s->center.y - m->center.y;
  const double dis = sqrt(dx * dx + dy * dy);

  if (dis <= SHIP_SIZE / 2 + meteor_get_size(m) / 2) {
    return true;
  } else {
    return false;
  }
}

bool ship_check_collision(ship *s, meteor_shower *ms) {
  meteor *iter = NULL;
  iter = ms->meteors;
  do {
    if (ship_collide_with_meteor(s, iter)) {
      return true;
    }
    iter = iter->next;
  } while (iter != NULL);
  return false;
}

void ship_draw(ship *s) {
  bullet_draw(s->bullets);

  al_draw_scaled_rotated_bitmap(s->bitmap,
                                al_get_bitmap_width(s->bitmap) / 2,
                                al_get_bitmap_height(s->bitmap) / 2,
                                s->center.x, s->center.y,
                                SHIP_SIZE / al_get_bitmap_width(s->bitmap),
                                SHIP_SIZE / al_get_bitmap_height(s->bitmap),
                                s->direction, 0);

#ifdef DEBUG
  al_draw_circle(s->center.x, s->center.y, 5, al_map_rgb(255, 0, 0), 3);
#endif
}

void ship_destroy(ship *s) {
  if (s != NULL) {
    if (s->bitmap) {
      al_destroy_bitmap(s->bitmap);
    }
    if (s->bullets) {
      bullet_destroy(s->bullets);
    }
    free(s);
  }
}

