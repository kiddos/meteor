#include "menu.h"

#define MENU_COVER_PATH "res/images/cover.png"
#define MENU_RECORD_NAME_PROMPT "NAME:"
#define MENU_NAME_INPUT_BUFFER_SIZE 1024

#define COMPUTE_TITLE_FONT_SIZE(window_size) (window_size.h / 20)
#define COMPUTE_OPTION_FONT_SIZE(window_size) (window_size.h / 30)

const char * const MENU_START_TEXT = "START";
const char * const MENU_RESTART_TEXT = "PLAY AGAIN";
const char * const MENU_HELP_TEXT = "HELP";
const char * const MENU_RECORD_TEXT = "RECORD";
const char * const MENU_EXIT_TEXT = "EXIT";
const double MENU_OPTION_PADDING = 20;

menu *menu_init(const char * const font_path, const size window_size) {
  menu *m = (menu *) malloc(sizeof(menu));

  // load background color bitmap
  m->cover = al_load_bitmap(MENU_COVER_PATH);
  if (!m->cover) {
    error_message("fail to load menu cover bitmap");
    free(m);
    return NULL;
  }

  m->title_font = al_load_font(font_path,
                               COMPUTE_TITLE_FONT_SIZE(window_size),
                               ALLEGRO_TTF_NO_KERNING);
  if (!m->title_font) {
    error_message("fail to load menu title font");
    al_destroy_bitmap(m->cover);
    free(m);
    return NULL;
  }

  m->option_font = al_load_font(font_path,
                                COMPUTE_OPTION_FONT_SIZE(window_size),
                                ALLEGRO_TTF_NO_KERNING);
  if (!m->option_font) {
    error_message("fail to load menu title font");
    al_destroy_bitmap(m->cover);
    al_destroy_font(m->title_font);
    free(m);
    return NULL;
  }

  m->is_visible = true;

  m->title_color = color_white();
  m->option_selected_color = color_light_gray();
  m->option_nonselect_color = color_dark_gray();

  m->mode = MENU_START;
  m->selection = 0;
  m->selections[0] = MENU_SELECTION_START;
  m->selections[1] = MENU_SELECTION_HELP;
  m->selections[2] = MENU_SELECTION_RECORD;
  m->selections[3] = MENU_SELECTION_EXIT;

  m->title_center.x = window_size.w / 2;
  m->title_center.y = (window_size.h - 5 * COMPUTE_OPTION_FONT_SIZE(window_size) -
          4 * MENU_OPTION_PADDING - COMPUTE_TITLE_FONT_SIZE(window_size)) / 2;

  m->option_center.x = window_size.w / 2;
  m->option_center.y = m->title_center.y + 3 * MENU_OPTION_PADDING;
  m->window_size = window_size;

  // help menu
  m->help_menu.h = help_init(FONT_PATH, color_white(),
                             point_init(m->option_center.x,
                                        m->option_center.y));
  m->help_menu.display_help = false;

  m->record_menu.r = record_init(FONT_PATH, color_white(),
                                 point_init(m->option_center.x,
                                            m->option_center.y +
                                            MENU_OPTION_PADDING),
                                 window_size);
  m->record_menu.id = input_dialog_init(MENU_RECORD_NAME_PROMPT,
                                        color_white(), color_dark_gray(),
                                        FONT_PATH, window_size);
  m->record_menu.record_entered = false;
  m->record_menu.display_record = false;
  return m;
}

bool menu_is_visible(const menu *m) {
  if (m != NULL) {
    return m->is_visible;
  } else {
    error_message("menu object null pointer");
    return false;
  }
}

bool menu_is_in_start_mode(const menu *m) {
  return m->mode == MENU_START;
}

void menu_set_visible(menu *m, const bool visible) {
  if (m != NULL) {
    m->is_visible = visible;
  } else {
    error_message("menu object null pointer");
  }
}

void menu_set_mode(menu *m, const menu_mode mode) {
  if (m != NULL) {
    m->mode = mode;
    if (m->mode == MENU_START) {
      m->selections[0] = MENU_SELECTION_START;
    } else if (m->mode == MENU_GAME_OVER) {
      m->selections[0] = MENU_SELECTION_RESTART;
    }
  } else {
    error_message("menu object null pointer");
  }
}

bool menu_is_displaying_help(const menu *m) {
  if (m != NULL) {
    return m->help_menu.display_help;
  } else {
    error_message("menu object null pointer");
    return false;
  }
}

void menu_toggle_display_help(menu *m) {
  if (m != NULL) {
    help_reset(m->help_menu.h);
    m->help_menu.display_help = !m->help_menu.display_help;
  } else {
    error_message("menu object null pointer");
  }
}

void menu_toggle_display_record(menu *m) {
  if (m != NULL) {
    m->record_menu.display_record = !m->record_menu.display_record;
  } else {
    error_message("menu object null pointer");
  }
}

void menu_move_up_selection(menu *m) {
  if (m != NULL) {
    if (m->selection > 0) {
      m->selection --;
    }
  } else {
    error_message("menu object null pointer");
  }
}

void menu_move_down_selection(menu *m) {
  if (m != NULL) {
    if (m->selection < 3) {
      m->selection ++;
    }
  } else {
    error_message("menu object null pointer");
  }
}

void menu_change_selection_with_mouse(menu *m, const point mouse) {
  const char *option_text[4];
  if (m->mode == MENU_START) {
    option_text[0] = MENU_START_TEXT;
  } else {
    option_text[0] = MENU_RESTART_TEXT;
  }
  option_text[1] = MENU_HELP_TEXT;
  option_text[2] = MENU_RECORD_TEXT;
  option_text[3] = MENU_EXIT_TEXT;

  int i;
  for (i = 0 ; i < 4 ; i ++) {
    const int text_width = al_get_text_width(m->option_font, option_text[i]);
    if (mouse.x >= m->option_center.x - text_width/2 - MENU_OPTION_PADDING/2 &&
        mouse.x <= m->option_center.x + text_width/2 + MENU_OPTION_PADDING/2 &&
        mouse.y >= m->option_center.y + (i) * MENU_OPTION_PADDING &&
        mouse.y <= m->option_center.y + (i) * MENU_OPTION_PADDING +
        (i + 1) * m->option_font->height) {
      m->selection = i;
      break;
    }
  }
}

menu_selection menu_get_selection(const menu *m) {
  if (m != NULL) {
    return m->selections[m->selection];
  } else {
    error_message("menu object null pointer");
    return -1;
  }
}

void menu_enable_intercept_keyboard_input(menu *m, const bool enable) {
  if (m != NULL) {
    if (!m->record_menu.record_entered && enable) {
      input_dialog_enable_intercept_keyboard_input(m->record_menu.id, enable);
    }
  } else {
    error_message("menu object null pointer");
  }
}

bool menu_should_intercept_keyboard_input(const menu *m) {
  return m->record_menu.id->should_intercept_keyboard_input;
}

bool menu_input_name_char(menu *m, const char c) {
  if (m != NULL) {
    if (input_dialog_enter_char(m->record_menu.id, c)) {
      memset(m->record_menu.name, '\0', sizeof(char) * MENU_NAME_INPUT_BUFFER_SIZE);
      input_dialog_retrieve_text(m->record_menu.id, m->record_menu.name);

      regular_message("name retreived: ");
      printf("name: %s\n", m->record_menu.name);

      return true;
    }
  } else {
    error_message("menu object null pointer");
  }
  return false;
}

void menu_retrieve_name(const menu *m, char name[MENU_NAME_INPUT_BUFFER_SIZE]) {
  if (m != NULL) {
    strncpy(name, m->record_menu.name,
            sizeof(char) * MENU_NAME_INPUT_BUFFER_SIZE);
  } else {
    error_message("menu object null pointer");
  }
}

void menu_enter_record(menu *m, const uint32_t score) {
  if (m != NULL) {
    regular_message("******* enter record *******");

    record_insert(m->record_menu.r, m->record_menu.name, score);
    m->record_menu.record_entered = true;
  } else {
    error_message("menu object null pointer");
  }
}

void menu_reset(menu *m) {
  if (m != NULL) {
    m->record_menu.record_entered = false;
    memset(m->record_menu.name, '\0', sizeof(char) * MENU_NAME_INPUT_BUFFER_SIZE);
    input_dialog_reset(m->record_menu.id);
  }
}

void menu_update(menu *m, const size window_size) {
  if (m->is_visible) {
    m->title_center.x = window_size.w / 2;
    m->title_center.y = (window_size.h - 5 * COMPUTE_OPTION_FONT_SIZE(window_size) -
            4 * MENU_OPTION_PADDING - COMPUTE_TITLE_FONT_SIZE(window_size)) / 2;

    m->option_center.x = window_size.w / 2;
    m->option_center.y = m->title_center.y + 3 * MENU_OPTION_PADDING;
    m->window_size = window_size;

    if (m->help_menu.display_help) {
      help_update(m->help_menu.h);
    }

    if (m->record_menu.id->should_intercept_keyboard_input) {
      input_dialog_update(m->record_menu.id, window_size);
    }

    if (m->record_menu.display_record) {
      record_update(m->record_menu.r, window_size);
    }
  }
}

void menu_draw(const menu *m) {
  if (m->is_visible) {
    uint32_t i;
    const char *option_text[4];
    if (m->mode == MENU_START) {
      option_text[0] = MENU_START_TEXT;
    } else {
      option_text[0] = MENU_RESTART_TEXT;
    }
    option_text[1] = MENU_HELP_TEXT;
    option_text[2] = MENU_RECORD_TEXT;
    option_text[3] = MENU_EXIT_TEXT;

    if (m->mode == MENU_GAME_OVER) {
      al_draw_scaled_bitmap(m->cover, 0, 0,
                            al_get_bitmap_width(m->cover),
                            al_get_bitmap_height(m->cover),
                            0, 0,
                            m->window_size.w, m->window_size.h, 0);
    }

    // title
    al_draw_text(m->title_font, m->title_color,
                m->title_center.x, m->title_center.y,
                ALLEGRO_ALIGN_CENTER, PROJECT_NAME);

    // decide rather to draw help, record, or option
    if (m->help_menu.display_help) {
      // help
      help_draw(m->help_menu.h);
    } else if (m->record_menu.display_record) {
      // record
      record_draw(m->record_menu.r);
    } else {
      // option
      for (i = 0 ; i < 4 ; i ++) {
        if (m->selection == i) {
          al_draw_text(m->option_font, m->option_selected_color,
                      m->option_center.x,
                      m->option_center.y + MENU_OPTION_PADDING * i +
                      m->option_font->height * i,
                      ALLEGRO_ALIGN_CENTER, option_text[i]);
        } else {
          al_draw_text(m->option_font, m->option_nonselect_color,
                      m->option_center.x,
                      m->option_center.y + MENU_OPTION_PADDING * i +
                      m->option_font->height * i,
                      ALLEGRO_ALIGN_CENTER, option_text[i]);
        }
      }
    }

    if (m->record_menu.id->should_intercept_keyboard_input) {
      input_dialog_draw(m->record_menu.id);
    }
  }
}

void menu_destroy(menu *m) {
  if (m != NULL) {
    if (m->cover)
      al_destroy_bitmap(m->cover);
    if (m->title_font)
      al_destroy_font(m->title_font);
    if (m->option_font)
      al_destroy_font(m->option_font);
    if (m->help_menu.h)
      help_destroy(m->help_menu.h);
    if (m->record_menu.r)
      record_destroy(m->record_menu.r);
    if (m->record_menu.id)
      input_dialog_destroy(m->record_menu.id);

    free(m);
  }
}

#undef MENU_COVER_PATH
#undef MENU_RECORD_NAME_PROMPT
#undef MENU_NAME_INPUT_BUFFER_SIZE
#undef COMPUTE_TITLE_FONT_SIZE
#undef COMPUTE_OPTION_FONT_SIZE

