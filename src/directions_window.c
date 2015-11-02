#include <pebble.h>
#include "directions_window.h"
#include "lines_window.h"
#include "stops_window.h"
#include "loading_stops_window.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_icon_down_arrow;
static GFont s_res_gothic_14;
static GBitmap *s_res_icon_up_arrow;
static GFont s_res_gothic_18_bold;
static ActionBarLayer *s_actionbarlayer_1;
static TextLayer *s_label_direction1_layer;
static TextLayer *s_label_direction2_layer;
static TextLayer *s_textlayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_icon_down_arrow = gbitmap_create_with_resource(RESOURCE_ID_ICON_DOWN_ARROW);
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  s_res_icon_up_arrow = gbitmap_create_with_resource(RESOURCE_ID_ICON_UP_ARROW);
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  // s_actionbarlayer_1
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorBlack);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_icon_up_arrow);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_icon_down_arrow);
  // Set the click config provider:
  action_bar_layer_set_click_config_provider(s_actionbarlayer_1, click_config_provider);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
  
  // s_label_direction1_layer
  s_label_direction1_layer = text_layer_create(GRect(9, 69, 105, 31));
  text_layer_set_text(s_label_direction1_layer, current_line.direction1);
  text_layer_set_font(s_label_direction1_layer, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_label_direction1_layer);
  
  // s_label_direction2_layer
  s_label_direction2_layer = text_layer_create(GRect(10, 114, 105, 31));
  text_layer_set_text(s_label_direction2_layer, current_line.direction2);
  text_layer_set_font(s_label_direction2_layer, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_label_direction2_layer);
  
  // s_textlayer_1
  APP_LOG(APP_LOG_LEVEL_INFO, "Current line: %s", current_line.line_num);  

  s_textlayer_1 = text_layer_create(GRect(5, 10, 100, 39));
  text_layer_set_text(s_textlayer_1, current_line.line_num);
  text_layer_set_font(s_textlayer_1, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
}

static void direction1_click_handler(ClickRecognizerRef recognizer, Window *window) {
    loading_stops_window_show();
    init_stops("1");
}

static void direction2_click_handler(ClickRecognizerRef recognizer, Window *window) {
    loading_stops_window_show();
    init_stops("2");
}


static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) direction1_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler) direction2_click_handler);  
}

static void destroy_ui(void) {
  window_destroy(s_window);
  action_bar_layer_destroy(s_actionbarlayer_1);
  text_layer_destroy(s_label_direction1_layer);
  text_layer_destroy(s_label_direction2_layer);
  text_layer_destroy(s_textlayer_1);
  gbitmap_destroy(s_res_icon_down_arrow);
  gbitmap_destroy(s_res_icon_up_arrow);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void directions_window_show() {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void directions_window_hide(void) {
  window_stack_remove(s_window, true);
}
