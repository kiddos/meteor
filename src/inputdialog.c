#include "inputdialog.h"

#define INPUT_DIALOG_BUFFER_SIZE 1024

const double INPUT_DIALOG_MIN_WIDTH = 200;
const double INPUT_DIALOG_MIN_HEIGHT = 100;
const double INPUT_DIALOG_PROMPT_FONT_SIZE = 18;
const double INPUT_DIALOG_PADDING = 15;
const double INPUT_DIALOG_INNER_PADDING = 5;

input_dialog *input_dialog_init(const char * const prompt,
                                const ALLEGRO_COLOR border_color,
                                const ALLEGRO_COLOR bg_color,
                                const char * const font_path,
                                const size window_size) {
  input_dialog *id = (input_dialog *) malloc(sizeof(input_dialog));

  id->font = al_load_font(font_path, INPUT_DIALOG_PROMPT_FONT_SIZE,
                          ALLEGRO_TTF_NO_KERNING);
  if (!id->font) {
    error_message("fail to load font for input dialog");
    free(id);
    return NULL;
  }


  id->border_color = border_color;
  id->bg_color = bg_color;

  id->prompt = (char *) malloc(sizeof(char) * (strlen(prompt) + 1));
  memset(id->prompt, '\0', sizeof(char) * (strlen(prompt) + 1));
  strcpy(id->prompt, prompt);

  memset(id->text, '\0', sizeof(char) * INPUT_DIALOG_BUFFER_SIZE);

  id->should_intercept_keyboard_input = false;

  input_dialog_update(id, window_size);
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

void input_dialog_enable_intercept_keyboard_input(input_dialog *id,
                                                  const bool enable) {
  if (id != NULL) {
    id->should_intercept_keyboard_input = enable;
  } else {
    error_message("input dialog object null pointer");
  }
}

bool input_dialog_enter_char(input_dialog *id, const char c) {
  if (id != NULL) {
    const int32_t str_length = strlen(id->text);
    bool finish = false;

    if (str_length < INPUT_DIALOG_BUFFER_SIZE) {
      printf("character %i\n", c);

      switch (c) {
        case 27:
          finish = false;
          id->should_intercept_keyboard_input = false;
          break;
        case 10:
        case 13:
          id->should_intercept_keyboard_input = false;
          finish = true;
          break;
        case 8:
          if (str_length - 1 >= 0) {
            id->text[str_length - 1] = '\0';
          } else {
            error_message("backspace out of bound");
          }
          break;
        case '\0':
          regular_message("null character");
          break;
        default:
          id->text[str_length] = c;
          break;
      }

      return finish;
    } else {
      error_message("input dialog character input out of bound");
      finish = true;
    }

    return finish;
  } else {
    error_message("input dialog object null pointer");
  }

  return false;
}

void input_dialog_reset(input_dialog *id) {
  if (id != NULL) {
    id->should_intercept_keyboard_input = false;
    memset(id->text, '\0', sizeof(char) * INPUT_DIALOG_BUFFER_SIZE);
  }
}

void input_dialog_update(input_dialog *id, const size window_size) {
  double display_width = 0, display_height = 0;
  id->center = point_init(window_size.w / 2, window_size.h / 2);

  if (window_size.w < INPUT_DIALOG_MIN_WIDTH) {
    display_width = INPUT_DIALOG_MIN_WIDTH;
  } else {
    display_width = window_size.w / 2;
  }

  if (window_size.h < INPUT_DIALOG_MIN_HEIGHT) {
    display_height = INPUT_DIALOG_MIN_HEIGHT;
  } else {
    display_height = window_size.h / 2;
  }

  id->display_size = size_init(display_width, display_height);
}

void input_dialog_draw(const input_dialog *id) {
  const double left_bound = id->center.x - id->display_size.w / 2;
  const double right_bound = id->center.x + id->display_size.w / 2;
  const double top_bound = id->center.y - id->display_size.h / 2;
  const double bot_bound = id->center.y + id->display_size.h / 2;

  al_draw_filled_rectangle(left_bound, top_bound,
                           right_bound, bot_bound, id->bg_color);
  al_draw_rectangle(left_bound, top_bound,
                    right_bound, bot_bound, id->border_color, 3);
  al_draw_text(id->font, id->border_color,
               left_bound + INPUT_DIALOG_PADDING,
               top_bound + INPUT_DIALOG_PADDING + id->font->height,
               ALLEGRO_ALIGN_LEFT, id->prompt);

  al_draw_rectangle(left_bound + INPUT_DIALOG_PADDING,
                    top_bound + 2 * INPUT_DIALOG_PADDING + 2 * id->font->height -
                    INPUT_DIALOG_INNER_PADDING,
                    right_bound - INPUT_DIALOG_PADDING,
                    top_bound + 2 * INPUT_DIALOG_PADDING + 3 * id->font->height +
                    INPUT_DIALOG_INNER_PADDING,
                    id->border_color, 2);
  al_draw_text(id->font, id->border_color,
               left_bound + INPUT_DIALOG_PADDING + INPUT_DIALOG_INNER_PADDING,
               top_bound + 2 * INPUT_DIALOG_PADDING + 2 * id->font->height,
               ALLEGRO_ALIGN_LEFT, id->text);
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

