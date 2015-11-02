
////////////////////////////////////
//  PUBLIC COMUNICATION WITH JS
////////////////////////////////////

void init_connection();
void connection_get_bus_stops();

////////////////////////////////////
//  MESSAGES COMUNICATION WITH JS
////////////////////////////////////

static void inbox_dropped_callback(AppMessageResult reason, void *context);
static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
static void outbox_sent_callback(DictionaryIterator *iterator, void *context);
static void inbox_received_callback(DictionaryIterator *iterator, void *context);
static void send_message_to_phone(int key, char *value);
