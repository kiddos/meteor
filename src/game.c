#include "game.h"

// constants
const double FPS = 60;

game *game_init(const int w, const int h) {
  game *g = (game *) malloc(sizeof(game));
  game_init_library();

  g->core.width = w;
  g->core.height = h;
  al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
  al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
  g->core.display = al_create_display(w, h);

  if (!g->core.display) {
    error_message("fail to create allegro display");
    free(g);
    return NULL;
  }

  g->core.timer = al_create_timer(1.0 / FPS);
  if (!g->core.timer) {
    error_message("fail to create allegro timer");
    al_destroy_display(g->core.display);
    free(g);
  }

  g->core.event_queue = al_create_event_queue();
  if (!g->core.event_queue) {
    error_message("fail to create allegro event queue");
    al_destroy_display(g->core.display);
    al_destroy_timer(g->core.timer);
    free(g);
    return NULL;
  }

  g->object.s = ship_init(point_init(g->core.width / 2, g->core.height / 2));
  if (!g->object.s) {
    error_message("fail to create allegro event queue");
    al_destroy_display(g->core.display);
    al_destroy_timer(g->core.timer);
    al_destroy_event_queue(g->core.event_queue);
    free(g);
    return NULL;
  }

  g->object.ms = meteor_shower_init(size_init(g->core.width, g->core.height));
  if (!g->object.ms) {
    error_message("fail to create allegro event queue");
    al_destroy_display(g->core.display);
    al_destroy_timer(g->core.timer);
    al_destroy_event_queue(g->core.event_queue);
    ship_destroy(g->object.s);
    free(g);
    return NULL;
  }
  return g;
}

bool game_init_library() {
  /* allegro main library */
  srand(time(NULL));
  if(!al_init()) {
    error_message("fail to initialize allegro library");
    return false;
  }
  /* inputs */
  if(!al_install_mouse()) {
    error_message("fail to install mouse for allegro");
    return false;
  }
  if(!al_install_keyboard()) {
    error_message("fail to install keyboard for allegro");
    return false;
  }
  /* outputs */
  if(!al_install_audio()) {
    error_message("fail to install audio for allegro");
    return false;
  }

  /* addon */
  if(!al_init_acodec_addon()) {
    error_message("fail to initialize audio codec addon for allegro");
    return false;
  }
  al_init_font_addon();
  if(!al_init_ttf_addon()) {
    error_message("fail to initialize font addon for allegro");
    return false;
  }
  if(!al_init_image_addon()) {
    error_message("fail to initialize image addon for allegro");
    return false;
  }
  if(!al_init_native_dialog_addon()) {
    error_message("fail to initialize dialog addon for allegro");
    return false;
  }
  if(!al_init_primitives_addon()) {
    error_message("fail to initialize primitive addon for allegro");
    return false;
  }

  /*reserve sample for audio output*/
  if(!al_reserve_samples(20)) {
    error_message("fail to reserve sample for audio output");
  }
  return true;
}

void game_prepare(game *g) {
  if (g != NULL) {
    g->core.running = true;
    // mouse event
    al_register_event_source(g->core.event_queue, al_get_mouse_event_source());
    // keyboard event
    al_register_event_source(g->core.event_queue, al_get_keyboard_event_source());

    // timer event
    al_register_event_source(g->core.event_queue,
                             al_get_timer_event_source(g->core.timer));
    // display event
    al_register_event_source(g->core.event_queue,
                             al_get_display_event_source(g->core.display));

    // start timer
    al_start_timer(g->core.timer);
    regular_message("start timer");
  }
}

void game_main_loop(game *g) {
  ALLEGRO_EVENT event;
  bool redraw = false;
  if (g != NULL) {
     do {
      do {
        al_wait_for_event(g->core.event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
          regular_message("event close display");
          g->core.running = false;
          break;
        } else if (event.type == ALLEGRO_EVENT_TIMER) {
          redraw = true;

          ship_update(g->object.s,
                      size_init(g->core.width, g->core.height));
          meteor_shower_update(g->object.ms,
                               size_init(g->core.width, g->core.height));
          if (ship_check_collision(g->object.s, g->object.ms,
                                   size_init(g->core.width, g->core.height))) {
            regular_message("ship collide with meteor !!!");
          }
        } else if (event.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
          al_acknowledge_resize(g->core.display);
          g->core.width = event.display.width;
          g->core.height = event.display.height;

          regular_message("window resize event");

          ship_update(g->object.s,
                      size_init(g->core.width, g->core.height));
          meteor_shower_update(g->object.ms,
                               size_init(g->core.width, g->core.height));
        } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
          regular_message("key up event");
          switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
              ship_stop(g->object.s, UP);
              break;
            case ALLEGRO_KEY_DOWN:
              ship_stop(g->object.s, DOWN);
              break;
            case ALLEGRO_KEY_LEFT:
              ship_stop(g->object.s, LEFT);
              break;
            case ALLEGRO_KEY_RIGHT:
              ship_stop(g->object.s, RIGHT);
              break;
            case ALLEGRO_KEY_SPACE:
              ship_shoot_bullet(g->object.s);
              break;
          }
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
          regular_message("key down event");
          switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
              ship_move(g->object.s, UP);
              break;
            case ALLEGRO_KEY_DOWN:
              ship_move(g->object.s, DOWN);
              break;
            case ALLEGRO_KEY_LEFT:
              ship_move(g->object.s, LEFT);
              break;
            case ALLEGRO_KEY_RIGHT:
              ship_move(g->object.s, RIGHT);
              break;
          }
        }
      } while (!al_is_event_queue_empty(g->core.event_queue));

      if (redraw) {
        al_clear_to_color(al_map_rgb(0, 0, 0));

        ship_draw(g->object.s);
        meteor_shower_draw(g->object.ms);

        al_flip_display();

        redraw = false;
      }
    } while (g->core.running);
  }
}

void game_destroy(game *g) {
  if (g != NULL) {
    if (g->core.display)
      al_destroy_display(g->core.display);
    if (g->core.timer)
      al_destroy_timer(g->core.timer);
    if (g->core.event_queue)
      al_destroy_event_queue(g->core.event_queue);
    if (g->object.s)
      ship_destroy(g->object.s);
    if (g->object.ms)
      meteor_shower_destroy(g->object.ms);
    free(g);
  }

  game_close_library();
}

void game_close_library() {
  regular_message("shutdown primitive addon");
  al_shutdown_primitives_addon();

  regular_message("shutdown native dialog addon");
  al_shutdown_native_dialog_addon();

  regular_message("shutdown image addon");
  al_shutdown_image_addon();

  regular_message("uninstall audio");
  al_uninstall_audio();

  regular_message("uninstall keyboard");
  al_uninstall_keyboard();

  regular_message("uinstall mouse");
  al_uninstall_mouse();

  regular_message("uninstll allegro system");
  al_uninstall_system();
}

