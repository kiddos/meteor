#include "meteor.h"

#define METEOR_BITMAP_PATH1 "res/images/stone.png"
#define METEOR_BITMAP_PATH2 "res/images/stone2.png"
#define METEOR_BITMAP_PATH3 "res/images/stone3.png"

const double METEOR_MIN_SPEED = 1.6;
const double METEOR_MAX_SPEED = 6.0;
const double METEOR_MIN_COUNT = 10;
const double METEOR_MAX_COUNT = 30;
const double METEOR_SMALL_SIZE = 30;
const double METEOR_MEDIUM_SIZE = 60;
const double METEOR_LARGE_SIZE = 120;
const double METEOR_ROTATION_SPEED = M_PI / 30;
const double METEOR_SMALL_HIT_POINT = 10;
const double METEOR_MEDIUM_HIT_POINT = 20;
const double METEOR_LARGE_HIT_POINT = 30;

static double random_range(const double min, const double max) {
  const double range = max - min;
  const long long int r = (int) (range * 1000);
  return (double) (rand() % r) / 1000.0 + min;
}

meteor *meteor_init(const size window_size) {
  int ran = 0;
  meteor *m = (meteor *) malloc(sizeof(meteor));

  // bitmap
  ran = rand() % 3;
  switch (ran) {
    case 0:
      m->bitmap = al_load_bitmap(METEOR_BITMAP_PATH1);
      break;
    case 1:
      m->bitmap = al_load_bitmap(METEOR_BITMAP_PATH2);
      break;
    case 2:
      m->bitmap = al_load_bitmap(METEOR_BITMAP_PATH3);
      break;
  }
  if (!m->bitmap) {
    error_message("fail to load meteor bitmap");
    free(m);
    return NULL;
  }

  // size
  ran = rand() % 3;
  switch (ran) {
    case 0:
      m->size = METEOR_SMALL;
      m->hit_point = METEOR_SMALL_HIT_POINT;
      break;
    case 1:
      m->size = METEOR_MEDIUM;
      m->hit_point = METEOR_MEDIUM_HIT_POINT;
      break;
    case 2:
      m->size = METEOR_LARGE;
      m->hit_point = METEOR_LARGE_HIT_POINT;
      break;
  }

  // speed
  m->speed = random_range(METEOR_MIN_SPEED, METEOR_MAX_SPEED);

  m->next = NULL;
  m->angle = random_range(0, 2 * M_PI);
  m->w = random_range(-METEOR_ROTATION_SPEED, METEOR_ROTATION_SPEED);

  // spawn point
  ran = rand() % 4;
  switch (ran) {
    case 0:
      if (m->size == METEOR_SMALL) {
        m->center.y = - METEOR_SMALL_SIZE;
      } else if (m->size == METEOR_MEDIUM) {
        m->center.y = - METEOR_MEDIUM_SIZE;
      } else {
        m->center.y = - METEOR_LARGE_SIZE;
      }
      m->center.x = random_range(-METEOR_LARGE_SIZE,
                                 window_size.w + METEOR_LARGE_SIZE);
      m->direction = random_range(M_PI * 3 / 4, M_PI * 5 / 4);
      break;
    case 1:
      if (m->size == METEOR_SMALL) {
        m->center.x = window_size.w + METEOR_SMALL_SIZE;
      } else if (m->size == METEOR_MEDIUM) {
        m->center.x = window_size.w + METEOR_MEDIUM_SIZE;
      } else {
        m->center.x = window_size.w + METEOR_LARGE_SIZE;
      }
      m->center.y = random_range(-METEOR_LARGE_SIZE,
                                 window_size.h + METEOR_LARGE_SIZE);
      m->direction = random_range(M_PI * 5 / 4, M_PI * 7 / 4);
      break;
    case 2:
      if (m->size == METEOR_SMALL) {
        m->center.y = window_size.h + METEOR_SMALL_SIZE;
      } else if (m->size == METEOR_MEDIUM) {
        m->center.y = window_size.h + METEOR_MEDIUM_SIZE;
      } else {
        m->center.y = window_size.h + METEOR_LARGE_SIZE;
      }
      m->center.x = random_range(-METEOR_LARGE_SIZE,
                                 window_size.w + METEOR_LARGE_SIZE);
      if (rand() % 2 == 0)
        m->direction = random_range(0, M_PI * 1 / 4);
      else
        m->direction = random_range(M_PI * 7 / 4, M_PI * 2);
      break;
    case 3:
      if (m->size == METEOR_SMALL) {
        m->center.x = - METEOR_SMALL_SIZE;
      } else if (m->size == METEOR_MEDIUM) {
        m->center.x = - METEOR_MEDIUM_SIZE;
      } else {
        m->center.x = - METEOR_LARGE_SIZE;
      }
      m->center.y = random_range(-METEOR_LARGE_SIZE,
                                 window_size.h + METEOR_LARGE_SIZE);
      m->direction = random_range(M_PI * 1 / 4, M_PI * 3 / 4);
      break;
  }

  m->v = velocity_init(m->speed, m->direction);
  /*regular_message("meteor created");*/
  /*printf("dx = %lf, dy = %lf speed = %lf, direction = %lf id=%p\n",*/
      /*m->v.dx, m->v.dy, m->speed, m->direction, m);*/
  return m;
}

bool meteor_is_inside_screen(const meteor *m, const size window_size) {
  if (m->center.x + m->v.dx < -2 * METEOR_LARGE_SIZE &&
      m->center.x + m->v.dx > window_size.w + 2 * METEOR_LARGE_SIZE) {
    return false;
  }
  if (m->center.y + m->v.dy < -2 * METEOR_LARGE_SIZE &&
      m->center.y + m->v.dy > window_size.h + 2 * METEOR_LARGE_SIZE) {
    return false;
  }
  return true;
}

bool meteor_is_broken_down(const meteor *m) {
  if (m->hit_point <= 0) {
    return true;
  } else {
    return false;
  }
}

double meteor_get_size(const meteor *m) {
  double ms = 0;
  switch (m->size) {
    case METEOR_SMALL:
      ms = METEOR_SMALL_SIZE;
      break;
    case METEOR_MEDIUM:
      ms = METEOR_MEDIUM_SIZE;
      break;
    case METEOR_LARGE:
      ms = METEOR_LARGE_SIZE;
      break;
  }
  return ms;
}

void meteor_take_damage(meteor *m, const double damage) {
  if (m != NULL) {
    m->hit_point -= damage;
    regular_message("============ meteor take damage ============");

    if (m->hit_point >= -1e-6 && m->hit_point <= 1e-6) {
      m->hit_point = 0;
    }
  } else {
    error_message("meteor object null pointer");
  }
}

void meteor_update(meteor *m, const size window_size) {
  // movement
  if (m->center.x + m->v.dx > -2 * METEOR_LARGE_SIZE &&
      m->center.x + m->v.dx < window_size.w + 2 * METEOR_LARGE_SIZE) {
    m->center.x += m->v.dx;
  } else {
    m->speed = 0;
    m->v = velocity_init(m->speed, m->direction);
  }

  if (m->center.y + m->v.dy > -2 * METEOR_LARGE_SIZE &&
      m->center.y + m->v.dy < window_size.h + 2 * METEOR_LARGE_SIZE) {
    m->center.y += m->v.dy;
  } else {
    m->speed = 0;
    m->v = velocity_init(m->speed, m->direction);
  }

  // rotation
  m->angle += m->w;
}

void meteor_draw(const meteor *m) {
  const double ms = meteor_get_size(m);
  al_draw_scaled_rotated_bitmap(m->bitmap,
                                al_get_bitmap_width(m->bitmap) / 2.0,
                                al_get_bitmap_height(m->bitmap) / 2.0,
                                m->center.x, m->center.y,
                                ms / al_get_bitmap_width(m->bitmap),
                                ms / al_get_bitmap_height(m->bitmap),
                                m->angle, 0);
}

void meteor_destroy(meteor *m) {
  if (m != NULL) {
    if (m->bitmap) {
      al_destroy_bitmap(m->bitmap);
    }
    free(m);
    /*regular_message("meteor memory freed");*/
  }
}

meteor_shower *meteor_shower_init(const size window_size) {
  int i;
  meteor_shower *ms = (meteor_shower *) malloc(sizeof(meteor_shower));
  meteor *m = meteor_init(window_size);
  meteor *iter = NULL, *temp = NULL;

  if (m != NULL) {
    iter = m;
    for (i = 0 ; i < METEOR_MIN_COUNT - 1; i ++) {
      iter->next = meteor_init(window_size);

      switch (iter->next->size) {
        case METEOR_SMALL:
          ms->live.small ++;
          break;
        case METEOR_MEDIUM:
          ms->live.medium ++;
          break;
        case METEOR_LARGE:
          ms->live.large ++;
          break;
      }

      if (iter->next == NULL) {
        error_message("fail to create meteor shower");
        iter = m;
        do {
          temp = iter->next;
          meteor_destroy(iter);
          iter = temp;
        } while (iter != NULL);
        return NULL;
      }

      iter = iter->next;
    }

    ms->meteors = m;
    ms->destroyed.small = 0;
    ms->destroyed.medium = 0;
    ms->destroyed.large = 0;

    return ms;
  } else {
    return NULL;
  }
}

void meteor_shower_add_meteor(meteor_shower *ms, const size window_size) {
  meteor *iter = NULL;

  if (ms != NULL) {
    if (ms->meteors != NULL &&
        meteor_shower_get_live_count(ms) <= METEOR_MAX_COUNT) {
      iter = ms->meteors;
      while (iter->next != NULL) {
        iter = iter->next;
      }

      iter->next = meteor_init(window_size);

      switch (iter->next->size) {
        case METEOR_SMALL:
          ms->live.small ++;
          break;
        case METEOR_MEDIUM:
          ms->live.medium ++;
          break;
        case METEOR_LARGE:
          ms->live.large ++;
          break;
      }
    }
  }
}

uint32_t meteor_shower_get_live_count(const meteor_shower *ms) {
  uint32_t count = 0;
  count += ms->live.small;
  count += ms->live.medium;
  count += ms->live.large;
  return count;
}

meteor_count meteor_shower_get_destroy_count(meteor_shower *ms) {
  meteor_count count = ms->destroyed;
  ms->destroyed.small = 0;
  ms->destroyed.medium = 0;
  ms->destroyed.large = 0;
  return count;
}

void meteor_shower_update(meteor_shower *ms, const size window_size) {
  meteor *iter = NULL, *temp = NULL, *m = NULL;
  if (ms->meteors->speed == 0 ||
      !meteor_is_inside_screen(ms->meteors, window_size)) {
    temp = ms->meteors->next;
    meteor_destroy(ms->meteors);

    ms->meteors = meteor_init(window_size);
    ms->meteors->next = temp;

    regular_message("first meteor out of screen or broken | creating a new one");
  } else if (meteor_is_broken_down(ms->meteors)) {
    switch (ms->meteors->size) {
      case METEOR_SMALL:
        ms->destroyed.small ++;
        break;
      case METEOR_MEDIUM:
        ms->destroyed.medium ++;
        break;
      case METEOR_LARGE:
        ms->destroyed.large ++;
        break;
    }

    temp = ms->meteors->next;
    meteor_destroy(ms->meteors);

    ms->meteors = meteor_init(window_size);
    ms->meteors->next = temp;

    regular_message("first meteor broken | creating a new one");
  }

  iter = ms->meteors;
  do {
    if (iter->next != NULL) {
      m = iter->next;
      if (m->speed == 0 ||
          !meteor_is_inside_screen(m, window_size)) {
        temp = m->next;
        meteor_destroy(m);
        iter->next = meteor_init(window_size);
        iter->next->next = temp;

        regular_message("meteor out of screen | creating a new one");
      } else if (meteor_is_broken_down(m)) {
        switch (ms->meteors->size) {
          case METEOR_SMALL:
            ms->destroyed.small ++;
            break;
          case METEOR_MEDIUM:
            ms->destroyed.medium ++;
            break;
          case METEOR_LARGE:
            ms->destroyed.large ++;
            break;
        }

        temp = m->next;
        meteor_destroy(m);
        iter->next = meteor_init(window_size);
        iter->next->next = temp;

        regular_message("meteor broken | creating a new one");
      }
    }
    meteor_update(iter, window_size);
    iter = iter->next;
  } while (iter != NULL);
}

void meteor_shower_draw(const meteor_shower *ms) {
  meteor *iter = NULL;
  iter = ms->meteors;

  do {
    meteor_draw(iter);
    iter = iter->next;
  } while (iter != NULL);
}

void meteor_shower_destroy(meteor_shower *ms) {
  meteor *iter = NULL, *temp = NULL;
  iter = ms->meteors;

  do {
    temp = iter->next;
    meteor_destroy(iter);
    iter = temp;
  } while (iter != NULL);
  free(ms);
}

