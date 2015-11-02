

typedef struct {
  char stop_name[100];
  char stop_next_bus[100];  
} BusStop;

BusStop stop_list[30];

////////////////////////////////////
//  *PUBLIC* METHODS
////////////////////////////////////

void init_stops();
void stops_window_hide();
void stops_window_show();

////////////////////
//  MENU HANDLERS  
////////////////////

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data);
static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);
static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);
static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data);
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data);

////////////////////////////////
//  STOPS WINDOW HANDLERS
////////////////////////////////
static void stops_window_load(Window *window);
static void stops_window_unload(Window *window);

////////////////////////////////////
//  MESSAGES COMUNICATION WITH JS
////////////////////////////////////

static void inbox_dropped_callback(AppMessageResult reason, void *context);
static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
static void outbox_sent_callback(DictionaryIterator *iterator, void *context);
static void inbox_received_callback(DictionaryIterator *iterator, void *context);
