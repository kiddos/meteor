#include "inputdialog.h"

#define INPUT_DIALOG_BUFFER_SIZE 1024

const double INPUT_DIALOG_MIN_WIDTH = 200;
const double INPUT_DIALOG_MIN_HEIGHT = 100;
const double INPUT_DIALOG_PROMPT_FONT_SIZE = 18;
const double INPUT_DIALOG_PADDING = 15;

input_dialog *input_dialog_init(const char * const prompt,
                                const ALLEGRO_COLOR border_color,
                                const char * const font_path,
                                const size window_size) {
  double display_width = 0, display_height = 0;
  input_dialog *id = (input_dialog *) malloc(sizeof(input_dialog));

  id->font = al_load_font(font_path, INPUT_DIALOG_PROMPT_FONT_SIZE,
                          ALLEGRO_TTF_NO_KERNING);
  if (!id->font) {
    error_message("fail to load font for input dialog");
    free(id);
    return NULL;
  }


  id->border_color = border_color;

  id->prompt = (char *) malloc(sizeof(char) * (strlen(prompt) + 1));
  memset(id->prompt, '\0', sizeof(char) * (strlen(prompt) + 1));
  strcpy(id->prompt, prompt);

  memset(id->text, '\0', sizeof(char) * INPUT_DIALOG_BUFFER_SIZE);

  id->should_intercept_keyboard_input = false;

  id->center = point_init(window_size.w / 2, window_size.h / 2);

  if (window_size.w < INPUT_DIALOG_MIN_WIDTH) {
    display_width = INPUT_DIALOG_MIN_WIDTH;
  } else {
    display_width = window_size.w;
  }

  if (window_size.h < INPUT_DIALOG_MIN_HEIGHT) {
    display_height = INPUT_DIALOG_MIN_HEIGHT;
  } else {
    display_height = window_size.h;
  }

  id->display_size = size_init(display_width, display_height);
  return id;
}

void input_dialog_retrieve_text(const input_dialog *id,
                                char text[INPUT_DIALOG_BUFFER_SIZE]) {
  if (id != NULL) {
    strcpy(text, id->text);
  } else {
    error_message("input dialog object null pointer");
  }
}

void input_dialog_enter_char(input_dialog *id, const char c) {
  if (id != NULL) {
    const uint32_t str_length = strlen(id->text);
    if (str_length < INPUT_DIALOG_BUFFER_SIZE) {
      id->text[str_length] = c;
    } else {
      error_message("input dialog character input out of bound");
    }
  } else {
    error_message("input dialog object null pointer");
  }
}

void input_dialog_update(input_dialog *id, const size window_size) {
  double display_width = 0, display_height = 0;
  id->center = point_init(window_size.w / 2, window_size.h / 2);

  if (window_size.w < INPUT_DIALOG_MIN_WIDTH) {
    display_width = INPUT_DIALOG_MIN_WIDTH;
  } else {
    display_width = window_size.w;
  }

  if (window_size.h < INPUT_DIALOG_MIN_HEIGHT) {
    display_height = INPUT_DIALOG_MIN_HEIGHT;
  } else {
    display_height = window_size.h;
  }

  id->display_size = size_init(display_width, display_height);
}

void input_dialog_draw(const input_dialog *id) {
  if (id != NULL) {
    al_draw_rectangle(id->center.x - id->display_size.w / 2,
                      id->center.y - id->display_size.h / 2,
                      id->center.x + id->display_size.w / 2,
                      id->center.y + id->display_size.h / 2,
                      id->border_color, 3);
    al_draw_text(id->font, id->border_color,
                 id->center.x - id->display_size.w / 2 + INPUT_DIALOG_PADDING,
                 id->center.y - id->display_size.w / 2 + INPUT_DIALOG_PADDING,
                 ALLEGRO_ALIGN_LEFT, id->prompt);
    al_draw_rectangle(id->center.x - id->display_size.w / 2 + INPUT_DIALOG_PADDING,
                      id->center.y - id->display_size.h / 2 + 2 * INPUT_DIALOG_PADDING,
                      id->center.x + id->display_size.w / 2 - INPUT_DIALOG_PADDING,
                      id->center.y + id->display_size.h / 2 +
                      INPUT_DIALOG_PROMPT_FONT_SIZE,
                      id->border_color, 2);
    al_draw_text(id->font, id->border_color,
                 id->center.x - id->display_size.w / 2 + INPUT_DIALOG_PADDING,
                 id->center.y - id->display_size.w / 2 + 2 * INPUT_DIALOG_PADDING,
                 ALLEGRO_ALIGN_LEFT, id->text);
  } else {
    error_message("input dialog object null pointer");
  }
}

void input_dialog_destroy(input_dialog *id) {
  if (id != NULL) {
    if (id->font)
      al_destroy_font(id->font);
    free(id->prompt);
    free(id);
  }
}

#undef INPUT_DIALOG_BUFFER_SIZE

