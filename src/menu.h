#ifndef MENU_H
#define MENU_H

#include "common.h"
#include "help.h"
#include "record.h"
#include "inputdialog.h"

/* constants */
extern const char * const MENU_START_TEXT;
extern const char * const MENU_RESTART_TEXT;
extern const char * const MENU_HELP_TEXT;
extern const char * const MENU_EXIT_TEXT;
extern const double MENU_OPTION_PADDING;

typedef enum {
  MENU_START,
  MENU_IN_GAME,
  MENU_GAME_OVER
} menu_mode;

typedef enum {
  MENU_SELECTION_START,
  MENU_SELECTION_RESTART,
  MENU_SELECTION_HELP,
  MENU_SELECTION_EXIT
} menu_selection;

typedef struct menu_t {
  ALLEGRO_BITMAP *cover;
  ALLEGRO_FONT *title_font, *option_font;
  ALLEGRO_COLOR title_color;
  ALLEGRO_COLOR option_selected_color, option_nonselect_color;
  menu_mode mode;
  bool is_visible;
  uint8_t selection;
  menu_selection selections[3];
  point title_center, option_center;
  size window_size;

  struct {
    help *h;
    bool display_help;
  } help_menu;

  struct {
    record *r;
    input_dialog *id;
  } record_menu;
} menu;

menu *menu_init(const char * const font_path, const size window_size);
bool menu_is_visible(const menu *m);
bool menu_is_in_start_mode(const menu *m);
void menu_set_visible(menu *m, const bool visible);
void menu_set_mode(menu *m, const menu_mode mode);
bool menu_is_displaying_help(const menu *m);
void menu_toggle_display_help(menu *m);
void menu_move_up_selection(menu *m);
void menu_move_down_selection(menu *m);
void menu_change_selection_with_mouse(menu *m, const point mouse);
menu_selection menu_get_selection(const menu *m);
bool menu_should_intercept_keyboard_input(const menu *m);
void menu_input_name_char(menu *m, const char c);
void menu_update(menu *m, const size window_size);
void menu_draw(const menu *m);
void menu_destroy(menu *m);

#endif /* end of include guard: MENU_H */

