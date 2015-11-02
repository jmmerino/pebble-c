#include <pebble.h>

#include "connection.h"
#include "lines_window.h"
#include "stops_window.h"
#include "splash_window.h"

#define MSG_END_LINE 10
#define LINE_NAME 11
#define LINE_NUM 12
#define LINE_DIRECTION1 13
#define LINE_DIRECTION2 14

#define MSG_END_BUS_STOP 20
#define STOP_NAME 21
#define STOP_NEXT_BUS 22

#define JS_CONNECTION_READY 1000
#define JS_GET_LINES 1001
#define JS_LINE_NUM 1002
#define JS_LINE_DIRECTION 1003

static int num_items = 0;

////////////////////////////////////
//  MESSAGES COMUNICATION WITH JS
////////////////////////////////////

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped! reason: %d", reason);
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {

  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed. Code: %d", reason);
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {

  // Get the first pair
  Tuple *t = dict_read_first(iterator);

  Tuple *t_line_name = dict_find(iterator, LINE_NAME);
  Tuple *t_line_num = dict_find(iterator, LINE_NUM);
  Tuple *t_line_direction1 = dict_find(iterator, LINE_DIRECTION1);
  Tuple *t_line_direction2 = dict_find(iterator, LINE_DIRECTION2);

  Tuple *t_stop_name = dict_find(iterator, STOP_NAME);
  Tuple *t_stop_next_bus = dict_find(iterator, STOP_NEXT_BUS);

  APP_LOG(APP_LOG_LEVEL_INFO, "Key received: %d", (int)t->key);

  switch(t->key){
    case JS_CONNECTION_READY:
      APP_LOG(APP_LOG_LEVEL_INFO, "JS loaded and ready");
      num_items = 0;
      send_message_to_phone(JS_GET_LINES, "1");
      break;

    case MSG_END_LINE:
      APP_LOG(APP_LOG_LEVEL_INFO, "Show lines window, %d", num_items);      
      lines_window_show(num_items);    
      break;

    case MSG_END_BUS_STOP:
      APP_LOG(APP_LOG_LEVEL_INFO, "Show stops window, %d", num_items);    
      stops_window_show(num_items);    
      break;

    case LINE_NAME:
    case LINE_NUM:
    case LINE_DIRECTION1:
    case LINE_DIRECTION2:              
      if (num_items < 50 && t_line_name != NULL && t_line_num != NULL){
        APP_LOG(APP_LOG_LEVEL_INFO, "Line name %s", t_line_name->value->cstring);
        APP_LOG(APP_LOG_LEVEL_INFO, "Line num %s", t_line_num->value->cstring);
        
        strcpy(line_list[num_items].line_name, t_line_name->value->cstring);
        strcpy(line_list[num_items].line_num, t_line_num->value->cstring);
        strcpy(line_list[num_items].direction1, t_line_direction1->value->cstring);
        strcpy(line_list[num_items].direction2, t_line_direction2->value->cstring);
        
        num_items++;
      }
      break;

    case STOP_NAME:  
    case STOP_NEXT_BUS:    
      if (num_items < 50 && t_stop_name != NULL && t_stop_next_bus != NULL){
        APP_LOG(APP_LOG_LEVEL_INFO, "Stop name %s", t_stop_name->value->cstring);
        APP_LOG(APP_LOG_LEVEL_INFO, "Stop next bus %s", t_stop_next_bus->value->cstring);
        
        strcpy(stop_list[num_items].stop_name, t_stop_name->value->cstring);
        strcpy(stop_list[num_items].stop_next_bus, t_stop_next_bus->value->cstring);        
        
        num_items++;
      }
      break;

  }
    
}

static void send_message_to_phone(int key, char *value) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Sending message to phone");

  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  if (iter == NULL) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "null iter");    
  } else {
    Tuplet tuple = TupletCString(key, value);
    dict_write_tuplet(iter, &tuple);
    dict_write_end(iter);

    app_message_outbox_send();
  }

    
}

void init_connection() {
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

void connection_get_bus_stops(char *line_num, char *direction) {
  char *params = "";
  strcat(params, line_num);
  strcat(params, ":");
  strcat(params, direction);

  num_items = 0;
  send_message_to_phone(JS_LINE_NUM, params);  
}