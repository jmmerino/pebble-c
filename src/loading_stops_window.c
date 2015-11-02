#include <pebble.h>

#include "lines_window.h"

#ifdef PBL_COLOR
  #define BUSAL_COLOR GColorFromHEX(0xee6e73)
#endif

  
static TextLayer *title_text_layer;
static TextLayer *loading_text_layer;
static Window *s_main_window;

void loading_stops_window_set_text(Layer *window_layer) {
    
  // Get the bounds of the window for sizing the text layer
  GRect bounds = layer_get_bounds(window_layer);

  // Create and Add to layer hierarchy:
  title_text_layer = text_layer_create(GRect(0, (bounds.size.h / 2) - 50, bounds.size.w, bounds.size.h));
  text_layer_set_text(title_text_layer, "BuSal");
  #ifdef PBL_COLOR
    text_layer_set_text_color(title_text_layer, GColorBlack);
    text_layer_set_background_color(title_text_layer, BUSAL_COLOR);
  #endif
  text_layer_set_font(title_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(title_text_layer, GTextAlignmentCenter); 
  layer_add_child(window_layer, text_layer_get_layer(title_text_layer));
    
  loading_text_layer = text_layer_create(GRect(0, (bounds.size.h / 2) + 10, bounds.size.w, bounds.size.h));
  text_layer_set_text(loading_text_layer, "Cargando Paradas...");
#ifdef PBL_COLOR
  text_layer_set_text_color(loading_text_layer, GColorBlack);
  text_layer_set_background_color(loading_text_layer, BUSAL_COLOR);
#endif
  text_layer_set_font(loading_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(loading_text_layer, GTextAlignmentCenter); 
  layer_add_child(window_layer, text_layer_get_layer(loading_text_layer));
  
}

void loading_stops_window_destroy_text() {
  text_layer_destroy(title_text_layer);
  text_layer_destroy(loading_text_layer);
}


static void loading_stops_window_load(Window *window) {
  // Get the root layer
  Layer *window_layer = window_get_root_layer(window);
  loading_stops_window_set_text(window_layer);
}

static void loading_stops_window_unload(Window *window) {
  loading_stops_window_destroy_text();
}


void loading_stops_window_show() {
  // Create main Window
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = loading_stops_window_load,
    .unload = loading_stops_window_unload,
  });
  
  #ifdef PBL_COLOR
    window_set_background_color(s_main_window, BUSAL_COLOR);
  #endif
  window_stack_push(s_main_window, true);
}

void loading_stops_window_hide() {
  window_stack_remove(s_main_window, false);
  // Destroy main Window
  window_destroy(s_main_window);
}

