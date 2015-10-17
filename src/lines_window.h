#include <pebble.h>

typedef struct {
  char line_name[100];
  char line_num[100];
} BusLine;

////////////////////////////////////
//  *PUBLIC* METHODS
////////////////////////////////////

void init_lines();
void destroy_lines();
void lines_window_show();

////////////////////
//  MENU HANDLERS  
////////////////////

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data);
static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);
static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);
static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data);
static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data);
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data);

////////////////////////////////
//  LINES WINDOW HANDLERS
////////////////////////////////

static void lines_window_load(Window *window);
static void lines_window_unload(Window *window);

////////////////////////////////////
//  MESSAGES COMUNICATION WITH JS
////////////////////////////////////

static void inbox_dropped_callback(AppMessageResult reason, void *context);
static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
static void outbox_sent_callback(DictionaryIterator *iterator, void *context);
static void inbox_received_callback(DictionaryIterator *iterator, void *context);
