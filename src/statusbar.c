#include "statusbar.h"

const double STATUS_BAR_HEIGHT = 60;
const double STATUS_BAR_PADDING = 6;
const uint32_t STATUS_BAR_FONT_SIZE = 36;
const double STATUS_BAR_LIVE_ICON_SIZE = 26;
const double STATUS_BAR_MANA_BAR_HIEGHT = 15;
const double STATUS_BAR_MANA_BAR_MAX_LENGTH = 110;
const char * const STATUS_BAR_LIVE_ICON_FILE_PATH = "res/images/lives.png";

status_bar *status_bar_init(ship *shp, const size window_size) {
  ALLEGRO_FONT *display_font = NULL;
  if (shp != NULL) {
    status_bar *sb = (status_bar *) malloc(sizeof(status_bar));
    sb->font = al_load_font(FONT_PATH,
                            STATUS_BAR_FONT_SIZE,
                            ALLEGRO_TTF_NO_KERNING);
    if (!sb->font) {
      error_message("fail to create status bar font");
      free(sb);
      return NULL;
    }

    sb->live_icon = al_load_bitmap(STATUS_BAR_LIVE_ICON_FILE_PATH);
    if (!sb->live_icon) {
      error_message("fail to load live icon");
      al_destroy_font(sb->font);
      free(sb);
      return NULL;
    }

    // drawing
    sb->background_color = color_black();
    sb->start.x = 0;
    sb->start.y = window_size.h - STATUS_BAR_HEIGHT;
    sb->area.w = window_size.w;
    sb->area.h = STATUS_BAR_HEIGHT;

    // time display
    display_font = al_load_font(FONT_PATH,
                                STATUS_BAR_FONT_SIZE,
                                ALLEGRO_TTF_NO_KERNING);
    if (!display_font) {
      error_message("fail to create font for time display");
      al_destroy_font(sb->font);
      al_destroy_bitmap(sb->live_icon);
      free(sb);
      return NULL;
    }
    sb->td = time_display_init(display_font, color_white(),
                               point_init(sb->start.x + STATUS_BAR_PADDING,
                                          sb->start.y + STATUS_BAR_PADDING));

    // score
    display_font = al_load_font(FONT_PATH,
                                STATUS_BAR_FONT_SIZE,
                                ALLEGRO_TTF_NO_KERNING);
    if (!display_font) {
      error_message("fail to create font for score");
      al_destroy_font(sb->font);
      al_destroy_bitmap(sb->live_icon);
      time_display_destroy(sb->td);
      free(sb);
      return NULL;
    }
    sb->sco = score_init(display_font, color_white(),
                         point_init(time_display_get_text_width(sb->td) +
                                    6 * STATUS_BAR_PADDING,
                                    sb->start.y + STATUS_BAR_PADDING));

    sb->shp = shp;
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
    score_reset(sb->sco);
  } else {
    error_message("unable to reset status bar | null pointer");
  }
}

void status_bar_update(status_bar *sb, const size window_size) {
  if (window_size.h - STATUS_BAR_HEIGHT != sb->start.y) {
    sb->start.y = window_size.h - STATUS_BAR_HEIGHT;
    time_display_set_point(sb->td, point_init(STATUS_BAR_PADDING,
                                              sb->start.y + STATUS_BAR_PADDING));

    score_set_point(sb->sco, point_init(time_display_get_text_width(sb->td) +
                                        6 * STATUS_BAR_PADDING,
                                        sb->start.y + STATUS_BAR_PADDING));
  }

  sb->start.x = 0;
  sb->area.w = window_size.w;
  sb->area.h = STATUS_BAR_HEIGHT;

  score_add_score_type(sb->sco, SCORE_TIME);
  score_update(sb->sco);
}

void status_bar_draw(status_bar *sb) {
  const double live_icon_width = al_get_bitmap_width(sb->live_icon);
  const double live_icon_height = al_get_bitmap_height(sb->live_icon);
  const double live_icon_start_x = sb->area.w - STATUS_BAR_LIVE_ICON_SIZE -
        STATUS_BAR_PADDING;
  const double live_icon_start_y = sb->start.y + sb->area.h -
        STATUS_BAR_LIVE_ICON_SIZE - STATUS_BAR_PADDING;
  const double live_icon_shift = STATUS_BAR_LIVE_ICON_SIZE +
        2 * STATUS_BAR_PADDING;
  const double mana_bar_length = ship_get_mana(sb->shp) / SHIP_MAX_MANA *
        STATUS_BAR_MANA_BAR_MAX_LENGTH;
  int i;

  al_draw_filled_rectangle(sb->start.x, sb->start.y,
                    sb->start.x + sb->area.w,
                    sb->start.y + sb->area.h,
                    sb->background_color);

  time_display_draw(sb->td);
  score_draw(sb->sco);

  for (i = 0 ; i < ship_get_lives(sb->shp) ; i ++) {
    al_draw_scaled_bitmap(sb->live_icon, 0, 0,
                          live_icon_width, live_icon_height,
                          live_icon_start_x - i * (live_icon_shift),
                          live_icon_start_y,
                          STATUS_BAR_LIVE_ICON_SIZE,
                          STATUS_BAR_LIVE_ICON_SIZE, 0);
  }

  al_draw_filled_rectangle(sb->area.w - STATUS_BAR_PADDING - mana_bar_length,
                           sb->start.y + STATUS_BAR_PADDING,
                           sb->area.w - STATUS_BAR_PADDING,
                           sb->start.y + STATUS_BAR_PADDING +
                           STATUS_BAR_MANA_BAR_HIEGHT, color_blue());
}

void status_bar_destroy(status_bar *sb) {
  if (sb != NULL) {
    if (sb->font)
      al_destroy_font(sb->font);
    if (sb->live_icon)
      al_destroy_bitmap(sb->live_icon);
    if (sb->td)
      time_display_destroy(sb->td);
    if (sb->sco)
      score_destroy(sb->sco);

    free(sb);
    regular_message("destroy status bar");
  }
}

