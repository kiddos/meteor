#ifndef COMMON_H
#define COMMON_H

/* allegro libraries */
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

/* standard libraries */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "message.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

extern const char * const FONT_PATH;

typedef struct point_t {
  double x, y;
} point;

typedef struct size {
  double w, h;
} size;

typedef struct velocity_t {
  double dx, dy;
} velocity;

typedef struct acceleration_t {
  double ax, ay;
} acceleration;

point point_init(const double x, const double y);
size size_init(const double width, const double height);
velocity velocity_init(const double speed, const double direction);
acceleration acceleration_init(const double ax, const double ay);
double velocity_compute_direction(velocity v);
double velocity_compute_speed(velocity v);
double acceleration_compute_direction(acceleration v);

/* colors */
ALLEGRO_COLOR color_white();
ALLEGRO_COLOR color_black();
ALLEGRO_COLOR color_dark_gray();
ALLEGRO_COLOR color_gray();
ALLEGRO_COLOR color_blue();

#endif /* end of include guard: COMMON_H */

