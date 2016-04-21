#include <pebble.h>

Window *window;
TextLayer *bg_layer;
TextLayer *time_layer;
TextLayer *date_layer;
TextLayer *dow_layer;
TextLayer *step_layer;

BitmapLayer *bt_layer;
GBitmap *bt_img;
BitmapLayer *battery_layer;
GBitmap *battery_img;

char time_buffer[] = "00:00";
char date_buffer[] = "00 September";
char long_dow_buffer[] = "000";
char dow_buffer[] = "00";
char step_buffer[] = "0000000000";

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  //Here we will update the watchface display
  //Format the buffer string using tick_time as the time source
  strftime(time_buffer, sizeof("00:00"), "%H:%M", tick_time);
  strftime(date_buffer, sizeof("00 September"), "%d %B", tick_time);
  strftime(long_dow_buffer, sizeof("000"), "%a", tick_time);
  strncpy(dow_buffer, long_dow_buffer, 2);
  snprintf(step_buffer, 10, "%d", (int)health_service_sum_today(HealthMetricStepCount));

  //Change the TextLayer text to show the new time!
  text_layer_set_text(time_layer, time_buffer);
  text_layer_set_text(date_layer, date_buffer);
  text_layer_set_text(dow_layer, dow_buffer);
  #if defined(PBL_HEALTH)
    snprintf(step_buffer, 10, "%d", (int)health_service_sum_today(HealthMetricStepCount));
    text_layer_set_text(step_layer, step_buffer);
  #endif
}


ResHandle get_battery_resource(int percent) {
  switch (percent) {
    case 0:
      battery_img = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_00);
      break;
    case 10:
      battery_img = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_10);
      break;
    case 20:
      battery_img = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_20);
      break;
    case 30:
      battery_img = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_30);
      break;
    case 40:
      battery_img = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_40);
      break;
    case 50:
      battery_img = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_50);
      break;
    case 60:
      battery_img = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_60);
      break;
    case 70:
      battery_img = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_70);
      break;
    case 80:
      battery_img = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_80);
      break;
    case 90:
      battery_img = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_90);
      break;
    case 100:
      battery_img = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_100);
      break;
  }
  return battery_img;
}

static void handle_battery(BatteryChargeState charge_state) {
  battery_img = get_battery_resource(charge_state.charge_percent);
  bitmap_layer_set_bitmap(battery_layer, battery_img);
}

void handle_bluetooth(bool connected) {
  layer_set_hidden(bitmap_layer_get_layer(bt_layer), !connected);
  vibes_double_pulse();
}

void add_bg_layer () {
  bg_layer = text_layer_create(GRect(0, 0, 144, 168));
  text_layer_set_background_color(bg_layer, GColorBlack);
  layer_add_child(window_get_root_layer(window), (Layer*) bg_layer);
}

void add_time_layer(ResHandle time_font) {
  time_layer = text_layer_create(GRect(0, 53, 144, 62));
  text_layer_set_background_color(time_layer, GColorBlack);
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_font(time_layer, fonts_load_custom_font(time_font));
  layer_add_child(window_get_root_layer(window), (Layer*) time_layer);
}

void add_date_layer(ResHandle text_font) {
  date_layer = text_layer_create(GRect(30, 148, 114, 20));
  text_layer_set_background_color(date_layer, GColorBlack);
  text_layer_set_text_color(date_layer, GColorWhite);
  text_layer_set_text_alignment(date_layer, GTextAlignmentRight);
  text_layer_set_font(date_layer, fonts_load_custom_font(text_font));
  layer_add_child(window_get_root_layer(window), (Layer*) date_layer);
}

void add_dow_layer(ResHandle text_font) {
  dow_layer = text_layer_create(GRect(0, 148, 30, 20));
  text_layer_set_background_color(dow_layer, GColorBlack);
  text_layer_set_text_color(dow_layer, GColorWhite);
  text_layer_set_text_alignment(dow_layer, GTextAlignmentLeft);
  text_layer_set_font(dow_layer, fonts_load_custom_font(text_font));
  layer_add_child(window_get_root_layer(window), (Layer*) dow_layer);
}

void add_battery_layer() {
  battery_layer = bitmap_layer_create(GRect(99, 0, 45, 20));
  layer_add_child(window_get_root_layer(window), (Layer*) battery_layer);
  BatteryChargeState charge_state = battery_state_service_peek();
  battery_img = get_battery_resource(charge_state.charge_percent);
  bitmap_layer_set_bitmap(battery_layer, battery_img);
  layer_add_child(window_get_root_layer(window), (Layer*) battery_layer);
}

void add_step_layer(ResHandle text_font) {
  step_layer = text_layer_create(GRect (0, 103, 144, 17));
  text_layer_set_background_color(step_layer, GColorBlack);
  text_layer_set_text_color(step_layer, GColorWhite);
  text_layer_set_text_alignment(step_layer, GTextAlignmentCenter);
  text_layer_set_font(step_layer, fonts_load_custom_font(text_font));
  #if defined(PBL_HEALTH) 
    layer_add_child(window_get_root_layer(window), (Layer*) step_layer);
  #endif
}

void add_bluetooth_layer() {
  bt_img = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_LOGO);
  bt_layer = bitmap_layer_create(GRect (0, 0, 20, 20));
  bitmap_layer_set_bitmap(bt_layer, bt_img);
  layer_add_child(window_get_root_layer(window), (Layer*) bt_layer);
}

void window_load(Window *window)
{
  ResHandle time_font = resource_get_handle(RESOURCE_ID_FONT_FUTURA_50);
  ResHandle text_font = resource_get_handle(RESOURCE_ID_FONT_UBUNTU_16);
  add_bg_layer();
  add_time_layer(time_font);
  add_date_layer(text_font);
  add_dow_layer(text_font);
  add_bluetooth_layer();
  add_battery_layer();
  add_step_layer(text_font);

  //Manually call the tick handler when the window is loading
  struct tm *t;
  time_t temp;
  temp = time(NULL);
  t = localtime(&temp);
  tick_handler(t, MINUTE_UNIT);
}

void window_unload(Window *window)
{
  //We will safely destroy the Window's elements here!
  gbitmap_destroy(bt_img);
  text_layer_destroy(bg_layer);
  text_layer_destroy(time_layer);
  text_layer_destroy(date_layer);
  text_layer_destroy(dow_layer);
  bitmap_layer_destroy(battery_layer);
  bitmap_layer_destroy(bt_layer);
  text_layer_destroy(step_layer);
}

void init()
{
  //Initialize the app elements here!
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler) tick_handler);
  battery_state_service_subscribe(&handle_battery);
  bluetooth_connection_service_subscribe(&handle_bluetooth);
  window_stack_push(window, true);
}
 
void deinit()
{
  //De-initialize elements here to save memory!
  window_destroy(window);
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
}

int main(void)
{
  init();
  app_event_loop();
  deinit();
}

