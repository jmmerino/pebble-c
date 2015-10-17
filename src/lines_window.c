#include <pebble.h>
#include <lines_window.h>
#include <splash_window.h>

#define BUSAL_COLOR GColorFromHEX(0xee6e73)
#define LINE_NAME 0
#define LINE_NUM 1
#define MSG_END 2
#define NUM_FIRST_MENU_ITEMS 3
#define NUM_MENU_SECTIONS 1
  
static Window *s_lines_window;
static MenuLayer *s_menu_layer;
static int num_items = 0;

BusLine line_list[20];


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
  // Determine which section we're working with
  menu_cell_basic_header_draw(ctx, cell_layer, "Lineas");
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  vibes_short_pulse();
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
  
  APP_LOG(APP_LOG_LEVEL_ERROR, "Free memory: %d", heap_bytes_free());
  APP_LOG(APP_LOG_LEVEL_ERROR, "Used memory: %d", heap_bytes_used());
    
  splash_window_hide();
  
  APP_LOG(APP_LOG_LEVEL_ERROR, "Free memory: %d", heap_bytes_free());
  APP_LOG(APP_LOG_LEVEL_ERROR, "Used memory: %d", heap_bytes_used());
    
  // Get the root layer
  Layer *window_layer = window_get_root_layer(window);

  GRect bounds = layer_get_frame(window_layer);

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
  menu_layer_set_click_config_onto_window(s_menu_layer, window);
  
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void lines_window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);
}

void lines_window_show() {
  // Create main Window
  s_lines_window = window_create();
  window_set_window_handlers(s_lines_window, (WindowHandlers) {
    .load = lines_window_load,
    .unload = lines_window_unload,
  });
  
  window_set_background_color(s_lines_window, BUSAL_COLOR);
  window_stack_push(s_lines_window, true);
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
  Tuple *t_line_name = dict_find(iterator, LINE_NAME);
  Tuple *t_line_num = dict_find(iterator, LINE_NUM);
  Tuple *t_end = dict_find(iterator, MSG_END);
  
  if (t_end == NULL){
    if (num_items < 50 && t_line_name != NULL && t_line_num != NULL){
      APP_LOG(APP_LOG_LEVEL_INFO, "Line name %s", t_line_name->value->cstring);
      APP_LOG(APP_LOG_LEVEL_INFO, "Line num %s", t_line_num->value->cstring);
      
      strcpy(line_list[num_items].line_name, t_line_name->value->cstring);
      strcpy(line_list[num_items].line_num, t_line_num->value->cstring);
      
      num_items++;
    }
  } else {
    lines_window_show();
  }
    
}

void init_lines() {
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

void destroy_lines() {
  APP_LOG(APP_LOG_LEVEL_INFO, "Destroy lines screen");
}
