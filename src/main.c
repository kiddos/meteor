#include "main.h"

const int DEFAULT_WINDOW_WIDTH = 1024;
const int DEFAULT_WINDOW_HEIGHT = 768;

int main(int argc, char *argv[]) {
  game *g = game_init(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
  if (g != NULL) {
    game_prepare(g);
    game_main_loop(g);
    game_destroy(g);
  } else {
    error_message("fail to create game object");
  }
  return 0;
}
