#include <pebble.h>
#define BUSAL_COLOR GColorFromHEX(0xee6e73)
  
#include <splash_window.h>

static Window *s_main_window;

static void main_window_load(Window *window) {
  // Get the root layer
  Layer *window_layer = window_get_root_layer(window);
  splash_window_set_text(window_layer);
}

static void main_window_unload(Window *window) {
  splash_window_destroy_text();
}

static void init() {
  // Create main Window
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  
  window_set_background_color(s_main_window, BUSAL_COLOR);
  
  window_stack_push(s_main_window, true);
}

static void deinit() {
  // Destroy main Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}