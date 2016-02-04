#include "bullet.h"

const double BULLET_SPEED = 5;
const double BULLET_WIDTH = 2;
const double BULLET_LENGTH = 6;

bullet *bullet_init(const point start, const double direction,
                    const double damage) {
  bullet *b = (bullet *) malloc(sizeof(bullet));
  b->start.x = start.x;
  b->start.y = start.y;
  b->speed = BULLET_SPEED;
  b->direction = direction;
  b->v = velocity_init(b->speed, b->direction);
  b->is_used = false;
  b->next = NULL;
  return b;
}

void bullet_add(bullet *bullets, const point start,
                const double direction, const double damage) {
  bullet *iter = NULL, *new_bullet;
  if (bullets != NULL) {
    iter = bullets;
    while (iter->next != NULL) {
      iter = iter->next;
    }

    // create new bullet
    new_bullet = (bullet *) malloc(sizeof(bullet));
    new_bullet->start.x = start.x;
    new_bullet->start.y = start.y;
    new_bullet->speed = BULLET_SPEED;
    new_bullet->direction = direction;
    new_bullet->v = velocity_init(new_bullet->speed, new_bullet->direction);
    new_bullet->is_used = false;
    new_bullet->next = NULL;

    iter->next = new_bullet;
  }
}

size_t bullet_get_count(bullet * const bullets) {
  size_t count = 0;
  bullet *iter = NULL;
  if (bullets != NULL) {
    iter = bullets;
    count ++;
    while (iter->next != NULL) {
      count ++;
      iter = iter->next;
    }
  }
  return count;
}

bool bullet_is_inside_screen(bullet *b, const size window_size) {
  if (b->start.x + b->v.dx <= - BULLET_LENGTH ||
      b->start.x + b->v.dx >= window_size.w + BULLET_LENGTH) {
    return false;
  }
  if (b->start.y + b->v.dy <= - BULLET_LENGTH ||
      b->start.y + b->v.dy >= window_size.h + BULLET_LENGTH) {
    return false;
  }
  return true;
}

bool bullet_hit_meteor(bullet *b, const meteor *m) {
  const double dx = b->start.x - m->center.x;
  const double dy = b->start.y - m->center.y;
  const double dis = sqrt(dx * dx + dy * dy);
  if (dis < meteor_get_size(m) / 2) {
    return true;
  } else {
    return false;
  }
}

bool bullet_check_collision(bullet *bullets, meteor *m) {
  bullet *iter = NULL;
  if (bullets != NULL) {
    iter = bullets;
    do {
      if (bullet_hit_meteor(iter, m)) {
        regular_message("bullet is used");
        iter->is_used = true;

        return true;
      }

      iter = iter->next;
    } while (iter != NULL);
    return false;
  }
  return false;
}

bool bullet_is_used(bullet *b) {
  return b->is_used;
}

void bullet_use(bullet *b) {
  if (b != NULL) {
    b->is_used = true;
  }
}

void bullet_update(bullet *bullets, const size window_size) {
  bullet *iter = NULL, *temp = NULL;
  if (bullets != NULL) {
    iter = bullets;
    do {
      if (iter->start.x + iter->v.dx > - BULLET_LENGTH &&
          iter->start.x + iter->v.dx < window_size.w + BULLET_LENGTH) {
        iter->start.x += iter->v.dx;
      }
      if (iter->start.y + iter->v.dy > - BULLET_LENGTH &&
          iter->start.y + iter->v.dy < window_size.h + BULLET_LENGTH) {
        iter->start.y += iter->v.dy;
      }

      if (iter->next != NULL) {
        if (!bullet_is_inside_screen(iter->next, window_size) ||
            bullet_is_used(iter->next)) {
          regular_message("next bullet not inside screen or is used");

          temp = iter->next->next;
          free(iter->next);
          iter->next = temp;
        }
      }

      iter = iter->next;
    } while (iter != NULL);
  }
}

void bullet_draw(bullet *bullets) {
  bullet *iter = NULL;
  if (bullets != NULL) {
    iter = bullets;
    do {
      al_draw_line(iter->start.x,
                   iter->start.y,
                   iter->start.x + sin(iter->direction) * BULLET_LENGTH,
                   iter->start.y - cos(iter->direction) * BULLET_LENGTH,
                   color_white(), BULLET_WIDTH);

      iter = iter->next;
    } while (iter != NULL);
  }
}

void bullet_destroy(bullet *bullets) {
  bullet *iter = NULL, *temp = NULL;
  if (bullets != NULL) {
    iter = bullets;
    do {
      regular_message("destroy bullet object");
      temp = iter->next;

      free(iter);
      iter = temp;
    } while (iter != NULL);
  }
}

