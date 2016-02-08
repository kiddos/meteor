#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include "common.h"

#define INPUT_DIALOG_BUFFER_SIZE 1024

typedef struct input_dialog_t {
  ALLEGRO_COLOR border_color;
  char *prompt;
  char text[INPUT_DIALOG_BUFFER_SIZE];
  bool should_intercept_keyboard_input;
} input_dialog;

input_dialog *input_dialog_init(const char * const prompt,
                                const ALLEGRO_COLOR border_color);
void input_dialog_retrieve_text(const input_dialog *id,
                                char text[INPUT_DIALOG_BUFFER_SIZE]);
void input_dialog_enter_char(input_dialog *id, const char c);
void input_dialog_draw(const input_dialog *id);
void input_dialog_destroy(input_dialog *id);

#undef INPUT_DIALOG_BUFFER_SIZE

#endif /* end of include guard: INPUTDIALOG_H */

