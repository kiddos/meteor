#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include "common.h"

#define INPUT_DIALOG_BUFFER_SIZE 1024

extern const double INPUT_DIALOG_MIN_WIDTH;
extern const double INPUT_DIALOG_MIN_HEIGHT;
extern const double INPUT_DIALOG_PROMPT_FONT_SIZE;
extern const double INPUT_DIALOG_PADDING;
extern const double INPUT_DIALOG_INNER_PADDING;

typedef struct input_dialog_t {
  ALLEGRO_FONT *font;
  ALLEGRO_COLOR border_color, bg_color;
  char *prompt;
  char text[INPUT_DIALOG_BUFFER_SIZE];
  bool should_intercept_keyboard_input;
  point center;
  size display_size;
} input_dialog;

input_dialog *input_dialog_init(const char * const prompt,
                                const ALLEGRO_COLOR border_color,
                                const ALLEGRO_COLOR bg_color,
                                const char * const font_path,
                                const size window_size);
void input_dialog_retrieve_text(const input_dialog *id,
                                char text[INPUT_DIALOG_BUFFER_SIZE]);
void input_dialog_enable_intercept_keyboard_input(input_dialog *id,
                                                  const bool enable);
bool input_dialog_enter_char(input_dialog *id, const char c);
void input_dialog_reset(input_dialog *id);
void input_dialog_update(input_dialog *id, const size window_size);
void input_dialog_draw(const input_dialog *id);
void input_dialog_destroy(input_dialog *id);

#undef INPUT_DIALOG_BUFFER_SIZE

#endif /* end of include guard: INPUTDIALOG_H */

