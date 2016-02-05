#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "ship.h"
#include "meteor.h"
#include "statusbar.h"
#include "menu.h"

// constants
extern const double FPS;

// game object
typedef struct game_t {
  struct {
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *event_queue;
    size window_size;
    bool running;
  } core;

  struct {
    ship *s;
    meteor_shower *ms;
  } object;

  struct {
    status_bar *sb;
    menu *m;
  } panel;
} game;

// game functions
game *game_init(const int w, const int h);
bool game_init_library();
void game_prepare(game *g);
void game_main_loop(game *g);
void game_destroy(game *g);
void game_close_library();

#endif /* end of include guard: GAME_H */

