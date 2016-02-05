#include "help.h"

const char * const HELP_TEXT_FILE_PATH = "res/help.txt";
const uint32_t HELP_TEXT_FONT_SIZE = 16;
const double HELP_TEXT_PADDING = 10;

static char *read_line(FILE *file) {
  if (file != NULL) {
    uint32_t i = 0;
    char buffer[2048] = {'\0'};
    char *result = NULL;
    char c;

    do {
      c = fgetc(file);
      buffer[i++] = c;
    } while (c != '\n' && c != '\0' && c != EOF);

    if (c != EOF) {
      result = (char *) malloc(sizeof(char) * (strlen(buffer) + 1));
      memset(result, '\0', sizeof(char) * (strlen(buffer) + 1));
      strcpy(result, buffer);
      return result;
    } else {
      regular_message("help file read to end");
      return NULL;
    }
  }
  error_message("help file null pointer");
  return NULL;
}

help *help_init(const char * const text_font,
                const ALLEGRO_COLOR target_color,
                const point center) {
  FILE *help_text = fopen(HELP_TEXT_FILE_PATH, "r");
  if (help_text) {
    help *h = (help *) malloc(sizeof(help));

    h->font = al_load_font(text_font, HELP_TEXT_FONT_SIZE,
                           ALLEGRO_TTF_NO_KERNING);
    if (!h->font) {
      error_message("fail to load help text font");
      free(h);
      fclose(help_text);
      return NULL;
    }

    // load the help text
    char *line = read_line(help_text);
    uint32_t max_length = 0;

    h->line_count = 0;
    h->text = (char **) malloc(sizeof(char *) * h->line_count);
    while (line != NULL) {
      h->line_count ++;
      h->text = (char **) realloc(h->text, sizeof(char *) * h->line_count);
      h->text[h->line_count - 1] = line;

      const uint32_t this_width = al_get_text_width(h->font,
                                                    h->text[h->line_count - 1]);
      if (this_width > max_length) {
        max_length = this_width;
      }

      line = read_line(help_text);
    }

    h->left = point_init(center.x - max_length / 2, center.y);
    h->target_color = target_color;
    h->display_color = al_map_rgba(0, 0, 0, 0);

    fclose(help_text);
    return h;
  } else {
    error_message("fail to load help text");
    return NULL;
  }
}

void help_reset(help *h) {
  if (h != NULL) {
    h->display_color = al_map_rgba(0, 0, 0, 0);
  }
}

void help_update(help *h) {
  unsigned char r, g, b, a, target_r, target_g, target_b, target_a;
  al_unmap_rgba(h->display_color, &r, &g, &b, &a);
  al_unmap_rgba(h->target_color,
                &target_r, &target_g, &target_b, &target_a);
  if (r < target_r) r ++;
  if (g < target_g) g ++;
  if (b < target_b) b ++;
  if (a < target_a) a ++;
  h->display_color = al_map_rgba(r, g, b, a);
}

void help_draw(const help *h) {
  uint32_t i;
  for (i = 0 ; i < h->line_count ; i ++) {
    al_draw_text(h->font, h->display_color,
                 h->left.x,
                 h->left.y + i * (HELP_TEXT_FONT_SIZE + HELP_TEXT_PADDING),
                 ALLEGRO_ALIGN_LEFT, h->text[i]);
  }
}

void help_destroy(help *h) {
  if (h != NULL) {
    if (h->text) {
      uint32_t i;
      for (i = 0 ; i < h->line_count ; i ++) {
        free(h->text[i]);
      }
      free(h->text);
    }
  }
}

