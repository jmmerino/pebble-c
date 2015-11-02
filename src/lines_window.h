

typedef struct {
  char line_name[100];
  char line_num[100];
  char direction1[100];
  char direction2[100];
} BusLine;

BusLine current_line;
BusLine line_list[20];

////////////////////////////////////
//  *PUBLIC* METHODS
////////////////////////////////////

void init_lines();
void lines_window_hide();
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