#include <pebble.h>

#include <connection.h>
#include <stops_window.h>
#include <lines_window.h>
#include <directions_window.h>
#include <loading_stops_window.h>

#define BUSAL_COLOR GColorFromHEX(0xee6e73)
  
static Window *s_stops_window;
static MenuLayer *s_menu_layer;
static int num_items = 0;


////////////////////
//  MENU HANDLERS  
////////////////////

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return 1; // Only one dimension menu
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return num_items;
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {  
  menu_cell_basic_header_draw(ctx, cell_layer, "Paradas");
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  if (cell_index->section == 0) {
      char stop_text[150];
      snprintf(stop_text, sizeof(stop_text), "%s", stop_list[cell_index->row].stop_name);
      menu_cell_basic_draw(ctx, cell_layer,         
        stop_text,
        stop_list[cell_index->row].stop_next_bus,
        NULL);
  }
}


////////////////////////////////
//  STOPS WINDOW HANDLERS
////////////////////////////////

static void stops_window_load(Window *window) {   
  // Destroy loading_stops window
  loading_stops_window_hide();  

  // Get the root layer
  Layer *window_layer = window_get_root_layer(window);

  // Get the bounds from the current window layer
  GRect bounds = layer_get_frame(window_layer);

  //Create the menú
  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback
  });
  
  // Bind the menu layer's click config provider to the window for interactivity
  // This is necesary to get the default menu events working
  menu_layer_set_click_config_onto_window(s_menu_layer, window);
  
  //Add the menu to the main window
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void stops_window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);
}

void stops_window_show(int num_items_param) {
  num_items = num_items_param;
  
  // Create main Window
  s_stops_window = window_create();
  window_set_window_handlers(s_stops_window, (WindowHandlers) {
    .load = stops_window_load,
    .unload = stops_window_unload,
  });
  
  window_set_background_color(s_stops_window, BUSAL_COLOR);
  window_stack_push(s_stops_window, true);
}


void init_stops(char *direction) {
  connection_get_bus_stops(current_line.line_num, direction);
}

void stops_window_hide() {
  APP_LOG(APP_LOG_LEVEL_INFO, "Destroy stops screen");

  // menu_layer_destroy(s_menu_layer);
  // window_destroy(s_stops_window);
}
