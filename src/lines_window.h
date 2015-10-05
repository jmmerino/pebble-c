#include <pebble.h>
#define BUSAL_COLOR GColorFromHEX(0xee6e73)
#define LINE_NAME 0
#define LINE_NUM 1
#define MSG_END 99
#define NUM_FIRST_MENU_ITEMS 3
#define NUM_MENU_SECTIONS 1
  
static Window *s_lines_window;
static SimpleMenuLayer *s_simple_menu_layer;
static SimpleMenuItem s_first_menu_items[NUM_FIRST_MENU_ITEMS];
static SimpleMenuSection s_menu_sections[NUM_MENU_SECTIONS];

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped! reason: %d", reason);
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void lines_window_load(Window *window) {
  // Get the root layer
  Layer *window_layer = window_get_root_layer(window);
  
  // Although we already defined NUM_FIRST_MENU_ITEMS, you can define
  // an int as such to easily change the order of menu items later
  int num_a_items = 0;

  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "First Item",
    //.callback = menu_select_callback,
  };
  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "Second Item",
    .subtitle = "Here's a subtitle",
    //.callback = menu_select_callback,
  };
  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "Third Item",
    .subtitle = "This has an icon",
    //.callback = menu_select_callback
  };
  
  s_menu_sections[0] = (SimpleMenuSection) {
    .num_items = NUM_FIRST_MENU_ITEMS,
    .items = s_first_menu_items,
  };

  GRect bounds = layer_get_frame(window_layer);

  s_simple_menu_layer = simple_menu_layer_create(bounds, window, s_menu_sections, NUM_MENU_SECTIONS, NULL);

  layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer));
}

static void lines_window_unload(Window *window) {
  simple_menu_layer_destroy(s_simple_menu_layer);
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

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
    case LINE_NAME:
      APP_LOG(APP_LOG_LEVEL_INFO, "Line %s", t->value->cstring);
      break;
    case LINE_NUM:
      APP_LOG(APP_LOG_LEVEL_INFO, "Line number: %s", t->value->cstring);
      break;
    case MSG_END:
      lines_window_show();
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }

    // Look for next item
    t = dict_read_next(iterator);
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
