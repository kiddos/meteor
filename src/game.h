#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "ship.h"

// constants
extern const double FPS;

// game object
typedef struct game_t {
  struct {
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *event_queue;
    int width, height;
    bool running;
  } core;

  struct {
    ship *s;
  } object;
} game;

// game functions
game *game_init(const int w, const int h);
bool game_init_library();
void game_prepare(game *g);
void game_main_loop(game *g);
void game_destroy(game *g);
void game_close_library();

#endif /* end of include guard: GAME_H */

