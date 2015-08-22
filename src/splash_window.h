#include <pebble.h>
  
static TextLayer *title_text_layer;
static TextLayer *loading_text_layer;

void splash_window_set_text(Layer *window_layer) {
    
  // Get the bounds of the window for sizing the text layer
  GRect bounds = layer_get_bounds(window_layer);

  // Create and Add to layer hierarchy:
  title_text_layer = text_layer_create(GRect(0, (bounds.size.h / 2) - 50, bounds.size.w, bounds.size.h));
  text_layer_set_text(title_text_layer, "BuSal");
  text_layer_set_text_color(title_text_layer, GColorWhite);
  text_layer_set_background_color(title_text_layer, BUSAL_COLOR);
  text_layer_set_font(title_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(title_text_layer, GTextAlignmentCenter); 
  layer_add_child(window_layer, text_layer_get_layer(title_text_layer));
  
  loading_text_layer = text_layer_create(GRect(0, (bounds.size.h / 2) + 10, bounds.size.w, bounds.size.h));
  text_layer_set_text(loading_text_layer, "Cargando líneas...");
  text_layer_set_text_color(loading_text_layer, GColorWhite);
  text_layer_set_background_color(loading_text_layer, BUSAL_COLOR);
  text_layer_set_font(loading_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(loading_text_layer, GTextAlignmentCenter); 
  layer_add_child(window_layer, text_layer_get_layer(loading_text_layer));
  
}

void splash_window_destroy_text() {
  text_layer_destroy(title_text_layer);
  text_layer_destroy(loading_text_layer);
}
