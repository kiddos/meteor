#include "common.h"

/* constants */
const char * const FONT_PATH = "res/fonts/SquadaOne-Regular.ttf";

point point_init(const double x, const double y) {
  point p;
  p.x = x;
  p.y = y;
  return p;
}

size size_init(const double width, const double height) {
  size s;
  s.w = width;
  s.h = height;
  return s;
}

velocity velocity_init(const double speed, const double direction) {
  velocity v;
  v.dx = speed * sin(direction);
  v.dy = -1.0 * speed * cos(direction);
  return v;
}

acceleration acceleration_init(const double ax, const double ay) {
  acceleration a;
  a.ax = ax;
  a.ay = ay;
  return a;
}

double velocity_compute_direction(velocity v) {
  if (v.dy == 0) {
    if (v.dx > 0) {
      return M_PI / 2;
    } else if (v.dx < 0) {
      return M_PI / 2 * 3;
    } else {
      return 0;
    }
  } else {
    double value = atan(v.dx / (-1.0 * v.dy));
    if (v.dy > 0) {
      return M_PI + value;
    } else if (v.dy < 0) {
      return 2 * M_PI + value;
    }
    return value;
  }
}

double velocity_compute_speed(velocity v) {
  return sqrt(v.dx * v.dx + v.dy * v.dy);
}

double acceleration_compute_direction(acceleration a) {
  if (a.ay == 0) {
    if (a.ax > 0) {
      return M_PI / 2;
    } else if (a.ax < 0) {
      return M_PI / 2 * 3;
    } else {
      return 0;
    }
  } else {
    double value = atan(a.ax / (-1.0 * a.ay));
    return value;
  }
}

/* colors */
ALLEGRO_COLOR color_white() {
  return al_map_rgb(250, 250, 250);
}

ALLEGRO_COLOR color_black() {
  return al_map_rgb(0, 0, 0);
}

ALLEGRO_COLOR color_light_gray() {
  return al_map_rgb(160, 160, 160);
}

ALLEGRO_COLOR color_gray() {
  return al_map_rgb(128, 128, 128);
}

ALLEGRO_COLOR color_dark_gray() {
  return al_map_rgb(66, 66, 66);
}

ALLEGRO_COLOR color_blue() {
  return al_map_rgb(80, 198, 245);
}

