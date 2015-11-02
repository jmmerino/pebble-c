#include <pebble.h>

#include <connection.h>
#include <lines_window.h>
#include <directions_window.h>
#include <splash_window.h>

#ifdef PBL_COLOR
#define BUSAL_COLOR GColorFromHEX(0xee6e73)
#endif
  
static Window *s_lines_window;
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
  menu_cell_basic_header_draw(ctx, cell_layer, "Lineas");
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Selected line: %s", line_list[cell_index->row].line_name);

  vibes_short_pulse();

  current_line = line_list[cell_index->row];

  directions_window_show();
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  if (cell_index->section == 0) {
      char line_text[150];
      snprintf(line_text, sizeof(line_text), "%s", line_list[cell_index->row].line_name);
      menu_cell_basic_draw(ctx, cell_layer, 
        line_list[cell_index->row].line_num, 
        line_text,
        NULL);
  }
}


////////////////////////////////
//  LINES WINDOW HANDLERS
////////////////////////////////

static void lines_window_load(Window *window) {
  // Destroy splash window
  splash_window_hide();  
    
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
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });
  
  // Bind the menu layer's click config provider to the window for interactivity
  // This is necesary to get the default menu events working
  menu_layer_set_click_config_onto_window(s_menu_layer, window);
  
  //Add the menu to the main window
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void lines_window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);
}

void lines_window_show(int num_items_param) {  
  num_items = num_items_param;  

  // Create main Window
  s_lines_window = window_create();
  window_set_window_handlers(s_lines_window, (WindowHandlers) {
    .load = lines_window_load,
    .unload = lines_window_unload,
  });
  
  #ifdef PBL_COLOR
  window_set_background_color(s_lines_window, BUSAL_COLOR);
  #endif
  window_stack_push(s_lines_window, true);
}


void init_lines() {
  
}

void lines_window_hide() {
  APP_LOG(APP_LOG_LEVEL_INFO, "Destroy lines screen");  
}
