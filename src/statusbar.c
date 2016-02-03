#include "statusbar.h"

const double STATUS_BAR_HEIGHT = 60;
const double STATUS_BAR_PADDING = 6;
const uint32_t STATUS_BAR_FONT_SIZE = 36;

status_bar *status_bar_init(ship *s, const size window_size) {
  ALLEGRO_FONT *time_display_font = NULL;
  if (s != NULL) {
    status_bar *sb = (status_bar *) malloc(sizeof(status_bar));
    sb->font = al_load_font(FONT_PATH,
                            STATUS_BAR_FONT_SIZE,
                            ALLEGRO_TTF_NO_KERNING);
    sb->background_color = color_black();

    sb->start.x = 0;
    sb->start.y = window_size.h - STATUS_BAR_HEIGHT;
    sb->area.w = window_size.w;
    sb->area.h = STATUS_BAR_HEIGHT;

    time_display_font = al_load_font(FONT_PATH,
                                     STATUS_BAR_FONT_SIZE,
                                     ALLEGRO_TTF_NO_KERNING);
    sb->td = time_display_init(time_display_font, color_white(),
                               point_init(sb->start.x + STATUS_BAR_PADDING,
                                          sb->start.y + STATUS_BAR_PADDING));

    sb->s = s;
    return sb;
  } else {
    error_message("fail to create status bar | ship null pointer");
    return NULL;
  }
}

void status_bar_start(status_bar *sb) {
  if (sb != NULL && sb->td != NULL) {
    time_display_start(sb->td);
  } else {
    error_message("unable to start status bar | null pointer");
  }
}

void status_bar_reset(status_bar *sb) {
  if (sb != NULL && sb->td != NULL) {
    time_display_stop(sb->td);
  } else {
    error_message("unable to reset status bar | null pointer");
  }
}

void status_bar_update(status_bar *sb, const size window_size) {
  sb->start.x = 0;
  sb->start.y = window_size.h - STATUS_BAR_HEIGHT;
  sb->area.w = window_size.w;
  sb->area.h = STATUS_BAR_HEIGHT;

  time_display_update(sb->td);
}

void status_bar_draw(status_bar *sb) {
  al_draw_filled_rectangle(sb->start.x, sb->start.y,
                    sb->start.x + sb->area.w,
                    sb->start.y + sb->area.h,
                    sb->background_color);

  time_display_draw(sb->td);

  // TODO draw ship status
}

void status_bar_destroy(status_bar *sb) {
  if (sb != NULL) {
    if (sb->td)
      time_display_destroy(sb->td);
    if (sb->font)
      al_destroy_font(sb->font);

    free(sb);
    regular_message("destroy status bar");
  }
}

