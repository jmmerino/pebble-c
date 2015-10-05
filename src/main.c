#include <pebble.h>
#define BUSAL_COLOR GColorFromHEX(0xee6e73)
  
#include <splash_window.h>
#include <api_lines.h>

static void init() {  
  splash_window_show();
  init_api_lines();
}

static void deinit() {
  splash_window_hide();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}