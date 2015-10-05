#include <pebble.h>
#define BUSAL_COLOR GColorFromHEX(0xee6e73)
  
#include <splash_window.h>
#include <lines_window.h>

static void init() {  
  splash_window_show();
  init_lines();
}

static void deinit() {
  splash_window_hide();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}