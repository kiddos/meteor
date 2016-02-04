#include "timedisplay.h"

#define TIME_DISPLAY_TEXT_FORMAT "TIME:  %02d:%02d"

time_display *time_display_init(ALLEGRO_FONT *font,
                                ALLEGRO_COLOR text_color,
                                point center) {
  if (font != NULL) {
    time_display *td = (time_display *) malloc(sizeof(time_display));
    td->font = font;
    td->text_color = text_color;
    td->updating = false;
    td->start_time = 0;
    td->time_passed = 0;
    td->start = center;
    memset(td->text, '\0', 16);

    sprintf(td->text, TIME_DISPLAY_TEXT_FORMAT,
            ((int) td->time_passed) / 60,
            ((int) td->time_passed) % 60);

    return td;
  } else {
    error_message("fail to create time display | null point of font object");
    return NULL;
  }
}

void time_display_start(time_display *td) {
  if (td != NULL) {
    td->start_time = al_get_time();
    td->time_passed = al_get_time() - td->start_time;
    td->updating = true;

    sprintf(td->text, TIME_DISPLAY_TEXT_FORMAT,
            ((int) td->time_passed) / 60,
            ((int) td->time_passed) % 60);
  } else {
    error_message("time display null pointer");
  }
}

void time_display_stop(time_display *td) {
  if (td != NULL) {
    td->start_time = 0;
    td->time_passed = 0;
    td->updating = false;

    sprintf(td->text, TIME_DISPLAY_TEXT_FORMAT,
            ((int) td->time_passed) / 60,
            ((int) td->time_passed) % 60);
  } else {
    error_message("time display null pointer");
  }
}

void time_display_update(time_display *td) {
  if (td->updating) {
    td->time_passed = al_get_time() - td->start_time;

    sprintf(td->text, TIME_DISPLAY_TEXT_FORMAT,
            ((int) td->time_passed) / 60,
            ((int) td->time_passed) % 60);
  }
}

void time_display_set_point(time_display *td, point start) {
  if (td != NULL) {
    td->start = start;
  } else {
    error_message("time display null pointer");
  }
}

const double time_display_get_text_width(const time_display *td) {
  if (td != NULL) {
    return al_get_text_width(td->font, td->text);
  } else {
    error_message("time display null pointer");
    return -1;
  }
}

void time_display_draw(time_display *td) {
  al_draw_text(td->font, td->text_color,
               td->start.x, td->start.y,
               ALLEGRO_ALIGN_LEFT, td->text);
}

const double time_display_get_time_passed(const time_display *td) {
  return td->time_passed;
}

void time_display_destroy(time_display *td) {
  if (td != NULL) {
    if (td->font) {
      al_destroy_font(td->font);
    }
    free(td);
    regular_message("destroy time display object");
  }
}

