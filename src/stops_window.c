#include <pebble.h>

#include <stops_window.h>
#include <directions_window.h>

#define BUSAL_COLOR GColorFromHEX(0xee6e73)
#define STOP_NAME 51
#define STOP_NEXT_BUS 52
#define MSG_END 99
  
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

void stops_window_show() {
  // Create main Window
  s_stops_window = window_create();
  window_set_window_handlers(s_stops_window, (WindowHandlers) {
    .load = stops_window_load,
    .unload = stops_window_unload,
  });
  
  window_set_background_color(s_stops_window, BUSAL_COLOR);
  window_stack_push(s_stops_window, true);
}


////////////////////////////////////
//  MESSAGES COMUNICATION WITH JS
////////////////////////////////////


static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped! reason: %d", reason);
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Read first item
  Tuple *t_stop_name = dict_find(iterator, STOP_NAME);
  Tuple *t_stop_next_bus = dict_find(iterator, STOP_NEXT_BUS);  
  Tuple *t_end = dict_find(iterator, MSG_END);
  
  if (t_end == NULL){
    if (num_items < 50 && t_stop_name != NULL && t_stop_next_bus != NULL){
      APP_LOG(APP_LOG_LEVEL_INFO, "Stop name %s", t_stop_name->value->cstring);
      APP_LOG(APP_LOG_LEVEL_INFO, "Stop next bux %s", t_stop_next_bus->value->cstring);
      
      strcpy(stop_list[num_items].stop_name, t_stop_name->value->cstring);
      strcpy(stop_list[num_items].stop_next_bus, t_stop_next_bus->value->cstring);      
      
      num_items++;
    }
  } else {
    stops_window_show();
  }
    
}

void init_stops() {
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

void stops_window_hide() {
  APP_LOG(APP_LOG_LEVEL_INFO, "Destroy stops screen");

  // menu_layer_destroy(s_menu_layer);
  // window_destroy(s_stops_window);
}
