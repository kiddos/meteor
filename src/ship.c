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
const double SHIP_STARTING_LIVES = 3;
const double SHIP_STARTING_DAMAGE = 10;
const double SHIP_MAX_MANA = 100;
const double SHIP_BULLET_MANA = 10;
const double SHIP_MANA_GAIN = 1;
const double SHIP_IMMUNE_DURATION = 6;

ship *ship_init(const point start) {
  ALLEGRO_CONFIG *config = al_load_config_file(CONFIG_FILE_PATH);
  if (config != NULL) {
    ship *s = (ship *) malloc(sizeof(ship));
    s->center.x = start.x;
    s->center.y = start.y;
    s->v.dx = 0;
    s->v.dy = 0;
    s->a.ax = SHIP_FORCE;
    s->a.ay = SHIP_FORCE;
    s->direction = velocity_compute_direction(s->v);
    s->speed = velocity_compute_speed(s->v);
    s->bullets = NULL;
    s->bullet_count = 0;
    s->m.movement[UP] = false;
    s->m.movement[DOWN] = false;
    s->m.movement[LEFT] = false;
    s->m.movement[RIGHT] = false;

    const char *bitmap_path = al_get_config_value(config,
                                                  CONFIG_SHIP_SECTION,
                                                  CONFIG_SHIP_BITMAP_KEY);

    s->bitmap = al_load_bitmap(bitmap_path);
    if (!s->bitmap) {
      error_message("fail to load ship bitmap");
      return NULL;
    }

    // ship attributes
    s->attr.lives = SHIP_STARTING_LIVES;
    s->attr.mana = SHIP_MAX_MANA;
    s->attr.level = atoi(al_get_config_value(config,
                                             CONFIG_SHIP_SECTION,
                                             CONFIG_SHIP_LEVEL_KEY));
    s->attr.damage = SHIP_STARTING_DAMAGE;
    s->attr.is_buffed = false;
    s->attr.is_immune = false;
    s->attr.time_stamp = 0;

    al_destroy_config(config);
    return s;
  } else {
    error_message("fail to load ship config file");
    return NULL;
  }
}

void ship_update(ship *s, const size window_size) {
  const double ship_diag = SHIP_SIZE * sqrt(2) / 2;
  double new_dx = s->v.dx, new_dy = s->v.dy;

  // update movement
  if (s->m.movement[LEFT])
    new_dx -= s->a.ax;
  if (s->m.movement[RIGHT])
    new_dx += s->a.ax;

  if (new_dx < SHIP_MAX_SPEED &&
      new_dx > -SHIP_MAX_SPEED) {
    s->v.dx = new_dx;
  }

  if (s->m.movement[UP])
    new_dy -= s->a.ay;
  if (s->m.movement[DOWN])
    new_dy += s->a.ay;

  if (new_dy < SHIP_MAX_SPEED &&
      new_dy > -SHIP_MAX_SPEED) {
    s->v.dy = new_dy;
  }

  if (s->center.x - ship_diag + s->v.dx > 0 &&
      s->center.x + ship_diag + s->v.dx < window_size.w) {
    s->center.x += s->v.dx;
  }
  if (s->center.y - ship_diag + s->v.dy > 0 &&
      s->center.y + ship_diag + s->v.dy < window_size.h) {
    s->center.y += s->v.dy;
  }

  // compute speed and velocity
  s->direction = velocity_compute_direction(s->v);
  s->speed = velocity_compute_speed(s->v);

  // update ship bullets
  if (s->bullets != NULL) {
    bullet_update(s->bullets, window_size);
    s->bullet_count = bullet_get_count(s->bullets);

    bullet *temp = NULL;
    if (!bullet_is_inside_screen(s->bullets, window_size) ||
        bullet_is_used(s->bullets)) {
      regular_message("this bullet not inside screen, cleaning up");
      temp = s->bullets->next;
      free(s->bullets);
      s->bullets = temp;
      s->bullet_count --;
    }
  }

  // increase mana
  ship_increase_mana(s, SHIP_MANA_GAIN);

  // check immune duration
  if (s->attr.is_immune) {
    const double time_passed = al_get_time() - s->attr.time_stamp;
    if (time_passed >= SHIP_IMMUNE_DURATION) {
      s->attr.is_immune = false;
      s->attr.time_stamp = 0;
    }
  }
}

void ship_move(ship *s, const ship_direction d) {
  switch (d) {
    case UP:
      s->m.movement[UP] = true;
      break;
    case DOWN:
      s->m.movement[DOWN] = true;
      break;
    case LEFT:
      s->m.movement[LEFT] = true;
      break;
    case RIGHT:
      s->m.movement[RIGHT] = true;
      break;
  }
}

void ship_stop(ship *s, const ship_direction d) {
  switch (d) {
    case UP:
      s->m.movement[UP] = false;
      break;
    case DOWN:
      s->m.movement[DOWN] = false;
      break;
    case LEFT:
      s->m.movement[LEFT] = false;
      break;
    case RIGHT:
      s->m.movement[RIGHT] = false;
      break;
  }
}

int32_t ship_get_lives(const ship *s) {
  if (s != NULL) {
    return s->attr.lives;
  } else {
    error_message("ship object null pointer");
    return -1;
  }
}

int32_t ship_get_level(const ship *s) {
  if (s != NULL) {
    return s->attr.level;
  } else {
    error_message("ship object null pointer");
    return -1;
  }
}

double ship_get_mana(const ship *s) {
  if (s != NULL) {
    return s->attr.mana;
  } else {
    error_message("ship object null pointer");
    return -1.0;
  }
}

void ship_increase_level(ship *s) {
  if (s != NULL) {
    s->attr.level ++;
  } else {
    error_message("ship object null pointer");
  }
}

void ship_increase_mana(ship *s, const double mana) {
  if (s != NULL) {
    if (s->attr.mana + mana <= SHIP_MAX_MANA)
      s->attr.mana += mana;
  } else {
    error_message("ship object null pointer");
  }
}

void ship_shoot_bullet(ship *s) {
  if (s != NULL) {
    const point start = point_init(s->center.x, s->center.y);
    if (s->bullets == NULL) {
      s->bullets = bullet_init(start, s->direction, s->attr.damage);
    } else {
      bullet_add(s->bullets, start, s->direction, s->attr.damage);
      s->bullet_count = bullet_get_count(s->bullets);
    }

    // shooting bullet cost mana
    if (s->attr.mana - SHIP_BULLET_MANA >= 0)
      s->attr.mana -= SHIP_BULLET_MANA;
  } else {
    error_message("ship object null pointer");
  }
}

void ship_take_damage(ship *s) {
  if (s != NULL) {
    if (!s->attr.is_immune) {
      s->attr.lives --;
      s->attr.is_immune = true;
      s->attr.time_stamp = al_get_time();
    }
  } else {
    error_message("ship object null pointer");
  }
}

bool ship_game_over(const ship *s) {
  if (s != NULL) {
    return s->attr.lives < 0;
  } else {
    error_message("ship object null pointer");
    return true;
  }
}

void ship_reset(ship *s, const size window_size) {
  if (s != NULL) {
    ALLEGRO_CONFIG *config = al_load_config_file(CONFIG_FILE_PATH);
    if (config != NULL) {
      // reset ship attributes
      s->attr.lives = SHIP_STARTING_LIVES;
      s->attr.mana = SHIP_MAX_MANA;
      s->attr.level = atoi(al_get_config_value(config,
                                                CONFIG_SHIP_SECTION,
                                                CONFIG_SHIP_LEVEL_KEY));
      s->attr.damage = SHIP_STARTING_DAMAGE;
      s->attr.is_buffed = false;
      s->attr.is_immune = false;
      s->attr.time_stamp = 0;
      al_destroy_config(config);
    }

    s->center.x = window_size.w / 2;
    s->center.y = window_size.h / 2;
    s->v.dx = 0;
    s->v.dy = 0;
    s->direction = velocity_compute_direction(s->v);
    s->speed = velocity_compute_speed(s->v);

    // destroy bullets
    bullet_destroy(s->bullets);
    s->bullets = NULL;
    s->bullet_count = 0;

    // reset movement
    s->m.movement[UP] = false;
    s->m.movement[DOWN] = false;
    s->m.movement[LEFT] = false;
    s->m.movement[RIGHT] = false;
  } else {
    error_message("ship object null pointer");
  }
}

bool ship_collide_with_meteor(const ship *s, const meteor *m) {
  const double dx = s->center.x - m->center.x;
  const double dy = s->center.y - m->center.y;
  const double dis = sqrt(dx * dx + dy * dy);

  if (dis <= SHIP_SIZE / 2 + meteor_get_size(m) / 2) {
    return true;
  } else {
    return false;
  }
}

bool ship_check_collision(const ship *s,
                          meteor_shower *ms,
                          const size window_size) {
  bool hit = false;
  meteor *iter = NULL;
  iter = ms->meteors;
  do {
    if (ship_collide_with_meteor(s, iter)) {
      hit = true;
    }
    iter = iter->next;
  } while (iter != NULL);

  ship_check_bullet_hit(s, ms, window_size);
  return hit;
}

bool ship_check_bullet_hit(const ship *s,
                           meteor_shower *ms,
                           const size window_size) {
  bool hit = false;
  meteor *iter = ms->meteors;
  do {
    if (iter != NULL) {
      if (bullet_check_collision(s->bullets, iter)) {
        meteor_take_damage(iter, s->attr.damage);
        hit = true;

        iter = iter->next;
      }
    }
    if (iter != NULL)
      iter = iter->next;
  } while (iter != NULL);
  return hit;
}

void ship_draw(const ship *s) {
  // draw the bullets
  bullet_draw(s->bullets);

  // draw the ship
  const uint32_t bitmap_width = al_get_bitmap_width(s->bitmap);
  const uint32_t bitmap_height = al_get_bitmap_height(s->bitmap);

  if (s->attr.is_immune) {
    const double time_passed = al_get_time() - s->attr.time_stamp;
    if (time_passed - floor(time_passed) < 0.5) {
      al_draw_tinted_scaled_rotated_bitmap(s->bitmap, color_gray(),
                                           bitmap_width / 2,
                                           bitmap_height / 2,
                                           s->center.x, s->center.y,
                                           SHIP_SIZE / bitmap_width,
                                           SHIP_SIZE / bitmap_height,
                                           s->direction, 0);
    } else {
      al_draw_scaled_rotated_bitmap(s->bitmap,
                                    bitmap_width / 2,
                                    bitmap_height / 2,
                                    s->center.x, s->center.y,
                                    SHIP_SIZE / bitmap_width,
                                    SHIP_SIZE / bitmap_height,
                                    s->direction, 0);
    }
  } else {
    al_draw_scaled_rotated_bitmap(s->bitmap,
                                  bitmap_width / 2,
                                  bitmap_height / 2,
                                  s->center.x, s->center.y,
                                  SHIP_SIZE / bitmap_width,
                                  SHIP_SIZE / bitmap_height,
                                  s->direction, 0);
  }

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

    regular_message("destroy ship object");
    free(s);
  }
}

