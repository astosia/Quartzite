#include <pebble.h>
#include "main.h"
#include "weekday.h"
#include "effect_layer.h"
#include <pebble-fctx/fctx.h>
#include <pebble-fctx/fpath.h>
#include <pebble-fctx/ffont.h>

#define ROUND_VERTICAL_PADDING 15
#define ROTATION_SETTING_DEFAULT 0
#define ROTATION_SETTING_LEFT    0
#define ROTATION_SETTING_RIGHT   1

//Static and initial vars
static GFont
  FontDayOfTheWeekShortName, FontSunset, FontSunset12,FontMoonPhase, FontBTQTIcons, FontTemp, FontTempFore, FontWeatherIcons, FontWeatherCondition, FontWindDirection;
/*char
  sunsetstring[20],
  sunrisestring[20],
  sunsetstring12[20],
  sunrisestring12[20],
  tempstring[20],
  condstring[20],
  windstring[20],
  windavestring[20],
  iconnowstring[8],
  iconforestring[8],
  windiconnowstring[8],
  windiconavestring[8],
  templowstring[20],
  temphistring[20];*///,
//  moonstring[12];
//  fontstring[40];

FFont* time_font;
#ifdef PBL_MICROPHONE
FFont* time_font_opend;
FFont* time_font_copse;
#endif

static Window * s_window;

static Layer * s_canvas_background;
static Layer * s_canvas_top_section; //weather layer
static Layer * s_canvas_bottom_section;
static Layer * s_canvas_bt_icon;
static Layer * s_canvas_qt_icon;
//static Layer *s_step_layer;
Layer * time_area_layer;
Layer * date_area_layer;


static int s_hours, s_minutes, s_weekday, s_day, s_loop;//, s_countdown;//, s_battery_level;

static char* weather_conditions[] = {
    "\U0000F07B", // 'unknown': 0,
    "\U0000F00D", // 'clear': 1,
    "\U0000F00D", // 'sunny': 2,
    "\U0000F002", // 'partlycloudy': 3,
    "\U0000F041", // 'mostlycloudy': 4,
    "\U0000F00C", // 'mostlysunny': 5,
    "\U0000F002", // 'partlysunny': 6,
    "\U0000F013", // 'cloudy': 7,
    "\U0000F019", // 'rain': 8,
    "\U0000F01B", // 'snow': 9,
    "\U0000F01D", // 'tstorms': 10,
    "\U0000F0b5", // 'sleat': 11,
    "\U0000F00A", // 'flurries': 12,
    "\U0000F0b6", // 'hazy': 13,
    "\U0000F01D", // 'chancetstorms': 14,
    "\U0000F01B", // 'chancesnow': 15,
    "\U0000F0b5", // 'chancesleat': 16,
    "\U0000F008", // 'chancerain': 17,
    "\U0000F01B", // 'chanceflurries': 18,
    "\U0000F07B", // 'nt_unknown': 19,
    "\U0000F02E", // 'nt_clear': 20,
    "\U0000F02E", // 'nt_sunny': 21,
    "\U0000F083", // 'nt_partlycloudy': 22,
    "\U0000F086", // 'nt_mostlycloudy': 23,
    "\U0000F081", // 'nt_mostlysunny': 24,
    "\U0000F086", // 'nt_partlysunny': 25,
    "\U0000F013", // 'nt_cloudy': 26,
    "\U0000F019", // 'nt_rain': 27,
    "\U0000F01B", // 'nt_snow': 28,
    "\U0000F01D", // 'nt_tstorms': 29,
    "\U0000F0b5", // 'nt_sleat': 30,
    "\U0000F038", // 'nt_flurries': 31,
    "\U0000F04A", // 'nt_hazy': 32,
    "\U0000F01D", // 'nt_chancetstorms': 33,
    "\U0000F038", // 'nt_chancesnow': 34,
    "\U0000F0B3", // 'nt_chancesleat': 35,
    "\U0000F036", // 'nt_chancerain': 36,
    "\U0000F038", // 'nt_chanceflurries': 37,
    "\U0000F003", // 'fog': 38,
    "\U0000F04A", // 'nt_fog': 39,
    "\U0000F050", // 'strong wind': 40,
    "\U0000F015", // 'hail': 41,
    "\U0000F056", // 'tornado': 42,
};

static char* wind_direction[] = {
    "\U0000F044",//'North': 0,
    "\U0000F043", //'NNE': 1,
     "\U0000F043",//'NE': 2,
     "\U0000F043",//'ENE': 3,
     "\U0000F048",//'East': 4,
     "\U0000F087",//'ESE': 5,
     "\U0000F087",//'SE': 6,
     "\U0000F087",//'SSE': 7,
     "\U0000F058",//'South': 8,
     "\U0000F057",//'SSW': 9,
     "\U0000F057",//'SW': 10,
     "\U0000F057",//'WSW': 11,
     "\U0000F04D",//'West': 12,
     "\U0000F088",//'WNW': 13,
     "\U0000F088",//'NW': 14,
     "\U0000F088",//'NNW': 15,
     "\U0000F04B", //'Variable': 16
};

static char* moon_phase[] ={
  "\U0000F095",//'wi-moon-new':0,
  "\U0000F096",//'wi-moon-waxing-crescent-1',1,
  "\U0000F097",//'wi-moon-waxing-crescent-2',2,
  "\U0000F098",//'wi-moon-waxing-crescent-3',3,
  "\U0000F099",//'wi-moon-waxing-crescent-4',4,
  "\U0000F09A",//'wi-moon-waxing-crescent-5',5,
  "\U0000F09B",//'wi-moon-waxing-crescent-6',6,
  "\U0000F09C",//'wi-moon-first-quarter',7,
  "\U0000F09D",//'wi-moon-waxing-gibbous-1',8,
  "\U0000F09E",//'wi-moon-waxing-gibbous-2',9,
  "\U0000F09F",//'wi-moon-waxing-gibbous-3',10,
  "\U0000F0A0",//'wi-moon-waxing-gibbous-4',11,
  "\U0000F0A1",//'wi-moon-waxing-gibbous-5',12,
  "\U0000F0A2",//'wi-moon-waxing-gibbous-6',13,
  "\U0000F0A3",//'wi-moon-full',14,
  "\U0000F0A4",//'wi-moon-waning-gibbous-1',15,
  "\U0000F0A5",//'wi-moon-waning-gibbous-2',16,
  "\U0000F0A6",//'wi-moon-waning-gibbous-3',17,
  "\U0000F0A7",//'wi-moon-waning-gibbous-4',18,
  "\U0000F0A8",//'wi-moon-waning-gibbous-5',19,
  "\U0000F0A9",//'wi-moon-waning-gibbous-6',20,
  "\U0000F0AA",//'wi-moon-third-quarter',21,
  "\U0000F0AB",//'wi-moon-waning-crescent-1',22,
  "\U0000F0AC",//'wi-moon-waning-crescent-2',23,
  "\U0000F0AD",//'wi-moon-waning-crescent-3',24,
  "\U0000F0AE",//'wi-moon-waning-crescent-4',25,
  "\U0000F0AF",//'wi-moon-waning-crescent-5',26,
  "\U0000F0B0",//'wi-moon-waning-crescent-6',27,
  "\U0000F095",//'wi-moon-new',28,
};
//////Init Configuration///
//Init Clay
ClaySettings settings;
// Initialize the default settings
static void prv_default_settings(){
  settings.HourColorN = GColorBlack;
  settings.MinColorN = GColorBlack;
  settings.FrameColor1N = GColorWhite;
  settings.FrameColor2N = GColorWhite;
  settings.FrameColorN = GColorWhite;
  settings.Text1ColorN = GColorBlack;
  settings.Text2ColorN = GColorBlack;
  settings.Text3ColorN = GColorBlack;
  settings.Text4ColorN = GColorBlack;
  settings.Text5ColorN = GColorBlack;
  settings.Text6ColorN = GColorBlack;
  settings.Text7ColorN = GColorBlack;
  settings.Text8ColorN = GColorBlack;
  settings.Text9ColorN = GColorBlack;
  settings.Text10ColorN = GColorBlack;
  settings.FrameColor1 = GColorBlack;
  settings.FrameColor2 = GColorBlack;
  settings.FrameColor = GColorBlack;
  settings.Text1Color = GColorWhite;
  settings.Text2Color = GColorWhite;
  settings.Text3Color = GColorWhite;
  settings.Text4Color = GColorWhite;
  settings.Text5Color = GColorWhite;
  settings.Text6Color = GColorWhite;
  settings.Text7Color = GColorWhite;
  settings.Text8Color = GColorWhite;
  settings.Text9Color = GColorWhite;
  settings.Text10Color = GColorWhite;
  settings.HourColor = GColorWhite;
  settings.MinColor = GColorWhite;
  settings.WeatherUnit = 0;
  settings.WindUnit = 0;
//  settings.FontChoice = "steel";
  settings.UpSlider = 30;
  settings.NightTheme = false;
  settings.HealthOff = true;
  settings.AddZero12h = false;
  settings.RemoveZero24h = false;
}
int HourSunrise=700;
int HourSunset=2200;
int moonphase=0;
bool BTOn=true;
bool GPSOn=true;
bool IsNightNow=false;
bool showForecastWeather = false;
int s_countdown = 0;

static GColor ColorSelect(GColor ColorDay, GColor ColorNight){
  if (settings.NightTheme && IsNightNow && GPSOn ){
    return ColorNight;
  }
  else{
    return ColorDay;
  }
}

/*#ifdef PBL_MICROPHONE
static FFont* FontSelect(FFont* FontTime1, FFont* FontTime2, FFont* FontTime3){
  if (strcmp(fontstring,"steel") == 0){
    return FontTime1;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Steelfish");
  }
    else if(strcmp(fontstring,"opend") == 0){
      return FontTime2;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "OpenDyslexic");
    }
      else {
        return FontTime3;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Copse");
          }
  }
#endif*/
// Callback for js request
void request_watchjs(){
  //Starting loop at 0
  s_loop = 0;
  // Begin dictionary
  DictionaryIterator * iter;
  app_message_outbox_begin( & iter);
  // Add a key-value pair
  dict_write_uint8(iter, 0, 0);
  // Send the message!
  app_message_outbox_send();
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  // A tap event occured
  showForecastWeather = !showForecastWeather;
  layer_mark_dirty (s_canvas_top_section);
}

///BT Connection
static void bluetooth_callback(bool connected){
  BTOn = connected;
}

static void bluetooth_vibe_icon (bool connected) {

  layer_set_hidden(s_canvas_bt_icon, connected);

  if(!connected && !quiet_time_is_active()) {
    // Issue a vibrating alert
    vibes_double_pulse();
  }
}

///Show sound off icon if Quiet time is active
static void quiet_time_icon () {
  if(!quiet_time_is_active()) {
  layer_set_hidden(s_canvas_qt_icon,true);
  }
}

static void onreconnection(bool before, bool now){
  if (!before && now){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "BT reconnected, requesting weather at %d:%d", s_hours,s_minutes);
    request_watchjs();
  }
}


//Add in HEALTH to the watchface

static char s_current_steps_buffer[16];
static int s_step_count = 0;

// Is step data available?



bool step_data_is_available(){
    return HealthServiceAccessibilityMaskAvailable &
      health_service_metric_accessible(HealthMetricStepCount,
        time_start_of_today(), time(NULL));
      }



// Todays current step count
static void get_step_count() {
//  if (!settings.HealthOff) {
    s_step_count = (int)health_service_sum_today(HealthMetricStepCount);/* code */
//  }
}


static void display_step_count() {
  int thousands = s_step_count / 1000;
  int hundreds = (s_step_count % 1000)/100;
  int hundreds2 = (s_step_count % 1000);

//  layer_set_text_color(s_step_layer, ColorSelect(settings.Text1Color, settings.Text1ColorN));

  snprintf(s_current_steps_buffer, sizeof(s_current_steps_buffer),
   "%s", "");

  if(thousands > 0) {
      snprintf(s_current_steps_buffer, sizeof(s_current_steps_buffer),
     "%d.%d%s", thousands, hundreds, "k");
  } else {
    snprintf(s_current_steps_buffer, sizeof(s_current_steps_buffer),
      "%d", hundreds2);
  }
//  layer_set_text(s_step_layer, s_current_steps_buffer);

}


static void health_handler(HealthEventType event, void *context) {
    if(event != HealthEventSleepUpdate) {
      get_step_count();
      display_step_count();
    }
  }
//}

void layer_update_proc_background (Layer * back_layer, GContext * ctx){
  // Create Rects
  GRect bounds = layer_get_bounds(back_layer);

  GRect TopBand =
    PBL_IF_ROUND_ELSE(
      GRect(0, 0, bounds.size.w, 54),
      GRect(0, 0, bounds.size.w, 49));

 GRect MediumBand =
    PBL_IF_ROUND_ELSE(
      GRect(0, 0, bounds.size.w, bounds.size.w),
      GRect(0, 0, bounds.size.w, bounds.size.w));

 GRect BottomBand =
    PBL_IF_ROUND_ELSE(
      GRect(0, 129, bounds.size.w, 51),
      GRect(0, 122, bounds.size.w, 46));

      graphics_context_set_fill_color(ctx, ColorSelect(settings.FrameColor1, settings.FrameColor1N));
      graphics_fill_rect(ctx, MediumBand,0,GCornersAll);
      graphics_context_set_fill_color(ctx, ColorSelect(settings.FrameColor2, settings.FrameColor2N));
      graphics_fill_rect(ctx, TopBand,0,GCornersAll);
      graphics_context_set_fill_color(ctx, ColorSelect(settings.FrameColor, settings.FrameColorN));
      graphics_fill_rect(ctx, BottomBand,0,GCornersAll);
}

void update_time_area_layer(Layer *l, GContext* ctx7) {
  // check layer bounds
  GRect bounds = layer_get_unobstructed_bounds(l);

  #ifdef PBL_ROUND
 //   bounds = GRect(0, ROUND_VERTICAL_PADDING, bounds.size.w, bounds.size.h - ROUND_VERTICAL_PADDING * 2);
     bounds = GRect(0, 0,bounds.size.w, bounds.size.h);
  #else
     bounds = GRect(0,0,bounds.size.w,bounds.size.h);
  #endif

  // initialize FCTX, the fancy 3rd party drawing library that all the cool kids use
  FContext fctx;

  fctx_init_context(&fctx, ctx7);
  fctx_set_color_bias(&fctx, 0);
  fctx_set_fill_color(&fctx, ColorSelect(settings.HourColor, settings.HourColorN));

// calculate font size
  #ifdef PBL_ROUND
  int font_size = 82;//bounds.size.h * 1.15;
  #else
  int font_size = 80;//bounds.size.h * 1.15;
  #endif
// steelfish metrics
//  int v_padding = bounds.size.h / 16;
//  int h_padding = bounds.size.w / 16;
  int h_adjust = 0;
  int v_adjust = 0;

    #ifdef PBL_COLOR
      fctx_enable_aa(true);
    #endif

  // if it's a round watch, EVERYTHING CHANGES
  #ifdef PBL_ROUND
//    v_adjust = ROUND_VERTICAL_PADDING;
    v_adjust = 0;

  #else
    // for rectangular watches, adjust X position based on sidebar position
  //    h_adjust -= ACTION_BAR_WIDTH / 2 + 1;
   h_adjust = 0;
  #endif

  /*char* fontstring;
  char FontToDraw[20];

  fontstring = settings.FontChoice;

  if (strcmp(settings.FontChoice,"steel") == 0)
  {
  snprintf(FontToDraw, sizeof(FontToDraw), "%s",fontstring);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Steelfish2");
  }
  else if (strcmp(settings.FontChoice,"opend") == 0) {
    snprintf(FontToDraw, sizeof(FontToDraw), "%s",fontstring);
          APP_LOG(APP_LOG_LEVEL_DEBUG, "OpenDyslexic2");
  } else if (strcmp(settings.FontChoice,"copse") == 0)  {
    snprintf(FontToDraw, sizeof(FontToDraw), "%s",fontstring);
          APP_LOG(APP_LOG_LEVEL_DEBUG, "Copse2");
  } else {
    snprintf(FontToDraw, sizeof(FontToDraw), "%s",fontstring);
              APP_LOG(APP_LOG_LEVEL_DEBUG, "Steelfish by default2");
  }*/

  FPoint time_pos;
  fctx_begin_fill(&fctx);

  #ifndef PBL_MICROPHONE
  fctx_set_text_em_height(&fctx, time_font, font_size);
  #else
  if (strcmp(settings.FontChoice,"steel") == 0)
  {
  fctx_set_text_em_height(&fctx, time_font, font_size);
//        APP_LOG(APP_LOG_LEVEL_DEBUG, "Steelfish1");
  }
  else if(strcmp(settings.FontChoice,"opend") == 0)
  {
  fctx_set_text_em_height(&fctx, time_font_opend, font_size);
//      APP_LOG(APP_LOG_LEVEL_DEBUG, "OpenDyslexic1");
  }
  else if(strcmp(settings.FontChoice,"cop") == 0)
  {
  fctx_set_text_em_height(&fctx, time_font_copse, font_size);
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "Copse1");
  }
  else {
  fctx_set_text_em_height(&fctx, time_font, font_size);
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "Steelfish by default1");
  }
  //fctx_set_text_em_height(&fctx, FontToDraw, font_size);
  #endif

  fctx_set_color_bias(&fctx,0);


  /*int hourdraw;
  char hournow[3];
  if (clock_is_24h_style()){
    if (settings.RemoveZero24h) {
      hourdraw=s_hours;
      snprintf(hournow,sizeof(hournow),"%d",hourdraw);
    } else {
      hourdraw=s_hours;
      snprintf(hournow,sizeof(hournow),"%02d",hourdraw);
    }
    }
  else {
    if (s_hours==0 || s_hours==12){
      hourdraw=12;
    }
    else {
    if (settings.AddZero12h) {
      hourdraw=s_hours%12;
    snprintf(hournow, sizeof(hournow), "%02d", hourdraw);
    } else {
      hourdraw=s_hours%12;
    snprintf(hournow, sizeof(hournow), "%d", hourdraw);
    }
    }
  }

  int mindraw;
  mindraw = s_minutes;
  char minnow[3];
  snprintf(minnow, sizeof(minnow), "%02d", mindraw);

  char timedraw[6];
  snprintf(timedraw, sizeof(timedraw), "%s:%s", hournow,minnow);*/
  time_t temp = time(NULL);
  struct tm *time_now = localtime(&temp);

  char timedraw[8];
  if(clock_is_24h_style() && settings.RemoveZero24h){
      strftime(timedraw, sizeof(timedraw),"%k:%M",time_now);
  } else if (clock_is_24h_style() && !settings.RemoveZero24h) {
      strftime(timedraw, sizeof(timedraw),"%H:%M",time_now);
  } else if (settings.AddZero12h) {
    strftime(timedraw, sizeof(timedraw),"%I:%M",time_now);
  } else {
    strftime(timedraw, sizeof(timedraw),"%l:%M",time_now);
  }

  // draw hours
  time_pos.x = INT_TO_FIXED(PBL_IF_ROUND_ELSE(125, 108+3) + h_adjust);
  time_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(137 , 130)  + v_adjust);

  fctx_set_offset(&fctx, time_pos);

  #ifndef PBL_MICROPHONE
  fctx_draw_string(&fctx, timedraw, time_font, GTextAlignmentRight, FTextAnchorBottom);
  #else
  if (strcmp(settings.FontChoice,"steel") == 0)
  {
  fctx_draw_string(&fctx, timedraw, time_font, GTextAlignmentRight, FTextAnchorBottom);
//        APP_LOG(APP_LOG_LEVEL_DEBUG, "Steelfish2");
  }
  else if(strcmp(settings.FontChoice,"opend") == 0)
  {
  fctx_draw_string(&fctx, timedraw, time_font_opend, GTextAlignmentRight, FTextAnchorBottom);
//      APP_LOG(APP_LOG_LEVEL_DEBUG, "OpenDyslexic2");
  }
  else if(strcmp(settings.FontChoice,"cop") == 0)
  {
  fctx_draw_string(&fctx, timedraw, time_font_copse, GTextAlignmentRight, FTextAnchorBottom);
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "Copse2");
  }
  else {
  fctx_draw_string(&fctx, timedraw, time_font, GTextAlignmentRight, FTextAnchorBottom);
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "Steelfish by default2");
  }
  //fctx_draw_string(&fctx, timedraw, FontToDraw, GTextAlignmentRight, FTextAnchorBottom);
  #endif
  fctx_end_fill(&fctx);

  fctx_deinit_context(&fctx);
}


void update_date_area_layer(Layer *a, GContext* ctx8){

  // check layer bounds
  GRect bounds = layer_get_unobstructed_bounds(a);

  #ifdef PBL_ROUND
 //   bounds = GRect(0, ROUND_VERTICAL_PADDING, bounds.size.w, bounds.size.h - ROUND_VERTICAL_PADDING * 2);
     bounds = GRect(0, 0,bounds.size.w, bounds.size.h);
  #else
     bounds = GRect(0,0,bounds.size.w,bounds.size.h);
  #endif

  // initialize FCTX, the fancy 3rd party drawing library that all the cool kids use
  FContext fctx;

  fctx_init_context(&fctx, ctx8);
  fctx_set_color_bias(&fctx, 0);
  fctx_set_fill_color(&fctx, ColorSelect(settings.MinColor, settings.MinColorN));

// calculate font size

  #ifdef PBL_ROUND
  int font_size = 54;//bounds.size.h * 1.15;
  #else
  int font_size = 50;//bounds.size.h * 1.15;
  #endif
  int h_adjust = 0;
  int v_adjust = 0;

    #ifdef PBL_COLOR
      fctx_enable_aa(true);
    #endif

  // if it's a round watch, EVERYTHING CHANGES
  #ifdef PBL_ROUND
//    v_adjust = ROUND_VERTICAL_PADDING;
    v_adjust = 0;

  #else
    // for rectangular watches, adjust X position based on sidebar position
  //    h_adjust -= ACTION_BAR_WIDTH / 2 + 1;
   h_adjust = 0;
  #endif

  FPoint time_pos;
  fctx_begin_fill(&fctx);

  #ifndef PBL_MICROPHONE
  fctx_set_text_em_height(&fctx, time_font, font_size);
  #else
  if (strcmp(settings.FontChoice,"steel") == 0)
  {
  fctx_set_text_em_height(&fctx, time_font, font_size);
//        APP_LOG(APP_LOG_LEVEL_DEBUG, "Steelfish3");
  }
  else if(strcmp(settings.FontChoice,"opend") == 0)
  {
  fctx_set_text_em_height(&fctx, time_font_opend, font_size);
  //    APP_LOG(APP_LOG_LEVEL_DEBUG, "OpenDyslexic3");
  }
  else if(strcmp(settings.FontChoice,"cop") == 0)
  {
  fctx_set_text_em_height(&fctx, time_font_copse, font_size);
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "Copse3");
  }
  else {
  fctx_set_text_em_height(&fctx, time_font, font_size);
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "Steelfish by default3");
  }
//  fctx_set_text_em_height(&fctx, FontSelect(time_font, time_font_opend, time_font_copse), font_size);
  #endif

  fctx_set_color_bias(&fctx,0);

  fctx_set_fill_color(&fctx, ColorSelect(settings.MinColor, settings.MinColorN));

  int daydraw;
  daydraw = s_day;
  char daynow[8];
  snprintf(daynow, sizeof(daynow), "%d", daydraw);

  time_pos.x = INT_TO_FIXED(PBL_IF_ROUND_ELSE(150, 127) + h_adjust);
  time_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(133, 126)  + v_adjust);

  fctx_set_offset(&fctx, time_pos);

  #ifndef PBL_MICROPHONE
  fctx_draw_string(&fctx, daynow, time_font, GTextAlignmentCenter, FTextAnchorBottom);
  #else
  if (strcmp(settings.FontChoice,"steel") == 0)
  {
  fctx_draw_string(&fctx, daynow, time_font, GTextAlignmentCenter, FTextAnchorBottom);
  //      APP_LOG(APP_LOG_LEVEL_DEBUG, "Steelfish4");
  }
  else if(strcmp(settings.FontChoice,"opend") == 0)
  {
  fctx_draw_string(&fctx, daynow, time_font_opend, GTextAlignmentCenter, FTextAnchorBottom);
  //    APP_LOG(APP_LOG_LEVEL_DEBUG, "OpenDyslexic4");
  }
  else if(strcmp(settings.FontChoice,"cop") == 0)
  {
  fctx_draw_string(&fctx, daynow, time_font_copse, GTextAlignmentCenter, FTextAnchorBottom);
  //  APP_LOG(APP_LOG_LEVEL_DEBUG, "Copse4");
  }
  else {
  fctx_draw_string(&fctx, daynow, time_font, GTextAlignmentCenter, FTextAnchorBottom);
  //  APP_LOG(APP_LOG_LEVEL_DEBUG, "Steelfish by default4");
  }
//  fctx_draw_string(&fctx, daynow, FontSelect(time_font, time_font_opend, time_font_copse), GTextAlignmentCenter, FTextAnchorBottom);
  #endif

  fctx_end_fill(&fctx);

  fctx_deinit_context(&fctx);
}


static void layer_update_proc(Layer * layer, GContext * ctx){
//add in weather info
  GRect IconNowRect = //weather condition icon
       (PBL_IF_ROUND_ELSE(
       GRect(0-4, 12,110,32),
       GRect(0-4, 5, 62,50)));

  GRect WindKtsRect =  //windspeed number
       (PBL_IF_ROUND_ELSE(
       (GRect(8,-3,180,30-6)),
       (GRect(60-10,0-7,144-36-62+20,30-16))));

  GRect TempRect =  //temperature number
       (PBL_IF_ROUND_ELSE(
         (GRect(8,30-12,180,30)),
         (GRect(60-10,14,144-36-62+20,30))));

 GRect WindDirNowRect =  //wind direction icon
      (PBL_IF_ROUND_ELSE(
      (GRect(92,12,90,32)),
      (GRect(110,4,36,46))));

GRect BatteryRect =
    (PBL_IF_ROUND_ELSE(
    GRect(0,50,180,2),
    GRect(0,45,144,2)));

    //Battery
  int s_battery_level = battery_state_service_peek().charge_percent;

  int width_rect = (s_battery_level * 144) / 100;
  int width_round = (s_battery_level * 160) / 100;

  GRect BatteryFillRect =
    (PBL_IF_ROUND_ELSE(
    GRect(10,50,width_round,2),
    GRect(0,45,width_rect,2)));

  char battperc[20];
  snprintf(battperc, sizeof(battperc), "%d", s_battery_level);
  strcat(battperc, "%");

   //   char SunsetToDraw[20];
   //   char SunriseToDraw[20];
   //    snprintf(SunsetToDraw, sizeof(SunsetToDraw), "%s",sunsetstring);
   //    snprintf(SunriseToDraw, sizeof(SunriseToDraw), "%s",sunrisestring);
//////////////////////////////

   char CondToDraw[20];
   char TempToDraw[20];
   char SpeedToDraw[20];
   char DirectionToDraw[20];




  if (showForecastWeather)
      {
   snprintf(CondToDraw, sizeof(CondToDraw), "%s",settings.iconnowstring);
   snprintf(TempToDraw, sizeof(TempToDraw), "%s",settings.tempstring);
   snprintf(SpeedToDraw,sizeof(SpeedToDraw),"%s",settings.windstring);
   snprintf(DirectionToDraw,sizeof(DirectionToDraw),"%s",settings.windiconnowstring);
      }
  else
      {
  snprintf(CondToDraw, sizeof(CondToDraw), "%s",settings.iconforestring);
  snprintf(TempToDraw, sizeof(TempToDraw), "%s",settings.temphistring);
  snprintf(SpeedToDraw,sizeof(SpeedToDraw),"%s",settings.windavestring);
  snprintf(DirectionToDraw,sizeof(DirectionToDraw),"%s", settings.windiconavestring);
      }



  //Weathericon
  graphics_context_set_text_color(ctx,ColorSelect(settings.Text7Color,settings.Text7ColorN));
  graphics_draw_text(ctx, CondToDraw, FontWeatherCondition, IconNowRect, GTextOverflowModeFill, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);
  //Weather temp
  if (showForecastWeather)
      {
        graphics_context_set_text_color(ctx,ColorSelect(settings.Text8Color,settings.Text8ColorN));
        graphics_draw_text(ctx, TempToDraw, FontTemp, TempRect, GTextOverflowModeFill, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);
      }
  else
      {
        graphics_context_set_text_color(ctx,ColorSelect(settings.Text8Color,settings.Text8ColorN));
        graphics_draw_text(ctx, TempToDraw, FontTempFore, TempRect, GTextOverflowModeFill, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);
      }
  //Wind speed
  graphics_context_set_text_color(ctx,ColorSelect(settings.Text9Color,settings.Text9ColorN));
  graphics_draw_text(ctx, SpeedToDraw, FontTempFore, WindKtsRect, GTextOverflowModeFill, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);
  //Wind Direction
  graphics_context_set_text_color(ctx,ColorSelect(settings.Text10Color,settings.Text10ColorN));
  graphics_draw_text(ctx, DirectionToDraw, FontWindDirection, WindDirNowRect, GTextOverflowModeFill, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);

  // Draw the battery bar background
  graphics_context_set_fill_color(ctx, ColorSelect(settings.FrameColor2,settings.FrameColor2N));// GColorBlack);
  graphics_fill_rect(ctx, BatteryRect, 0, GCornerNone);

  // Draw the battery bar
  graphics_context_set_fill_color(ctx, ColorSelect(settings.Text6Color, settings.Text6ColorN));
  graphics_fill_rect(ctx,BatteryFillRect, 0, GCornerNone);
}

static void layer_update_proc_sunset(Layer * layer2, GContext * ctx2){
//add in steps, date, sunrise & sunset

  GRect DateRect =
   //  (0, offsetdate, bounds3.size.w, bounds1.size.h/4);
    (PBL_IF_ROUND_ELSE(
       GRect(126-1, 57-10, 48, 20),
       GRect(110, 53-11, 34, 20)));

 GRect SunsetRect =
       (PBL_IF_ROUND_ELSE(
        GRect(99-4,133-8,61,14),
         GRect(54+2, 151-10, 54,20)));

  GRect SunriseRect =
       (PBL_IF_ROUND_ELSE(
         GRect(20+6,133-8,61,14),
         GRect(0, 151-10, 54,20)));

   GRect MoonRect =
   (PBL_IF_ROUND_ELSE(
     GRect(90-9,132-5,18,18),
     GRect(108, 168-20-8, 36,20)));

   GRect StepsRect =
         (PBL_IF_ROUND_ELSE(
             GRect(0, 156-8, 180, 40),
             GRect(0, 116+2, 142, 40)));


   //sunsettime variable by clock setting
    char SunsetToDraw[20];
   if (clock_is_24h_style()){
       snprintf(SunsetToDraw, sizeof(SunsetToDraw), "%s",settings.sunsetstring);
     }
   else {
       snprintf(SunsetToDraw, sizeof(SunsetToDraw), "%s",settings.sunsetstring12);
     }

     char SunriseToDraw[20];
    if (clock_is_24h_style()){
        snprintf(SunriseToDraw, sizeof(SunriseToDraw), "%s",settings.sunrisestring);
      }
    else {
        snprintf(SunriseToDraw, sizeof(SunriseToDraw), "%s",settings.sunrisestring12);
      }

    APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather data captured at time %d",settings.Weathertimecapture);


    char MoonToDraw[20];
    snprintf(MoonToDraw, sizeof(MoonToDraw), "%s",settings.moonstring);

    char StepsToDraw[20];
    snprintf(StepsToDraw, sizeof(StepsToDraw), "%s",s_current_steps_buffer);

     //Date
    // Local language
    const char * sys_locale = i18n_get_system_locale();
    char datedraw[10];
    fetchwday(s_weekday, sys_locale, datedraw);
    char datenow[10];
    snprintf(datenow, sizeof(datenow), "%s", datedraw);

   GRect SunsetIconRect =
    (PBL_IF_ROUND_ELSE(
      GRect(113,150-3,24,24),
      GRect(70,126,26,26)));

    GRect SunriseIconRect =
       (PBL_IF_ROUND_ELSE(
         GRect(42,150-3,24,24),
         GRect(15-4,126+1,26,26)));

   char SunsetIconToShow[20];

  snprintf(SunsetIconToShow, sizeof(SunsetIconToShow),  "%s", "\U0000F052");

  char SunriseIconToShow[20];

  snprintf(SunriseIconToShow, sizeof(SunriseIconToShow),  "%s",  "\U0000F051");

 graphics_context_set_text_color(ctx2, ColorSelect(settings.Text2Color, settings.Text2ColorN));
 graphics_draw_text(ctx2, SunsetIconToShow, FontWeatherIcons, SunsetIconRect, GTextOverflowModeFill,PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);
 graphics_draw_text(ctx2, SunriseIconToShow, FontWeatherIcons, SunriseIconRect, GTextOverflowModeFill,PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);

 graphics_context_set_text_color(ctx2, ColorSelect(settings.Text2Color, settings.Text2ColorN));
 if (clock_is_24h_style()){
   graphics_draw_text(ctx2, SunsetToDraw, FontSunset, SunsetRect, GTextOverflowModeWordWrap, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);
   graphics_draw_text(ctx2, SunriseToDraw, FontSunset, SunriseRect, GTextOverflowModeWordWrap, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);
   }
 else {
   graphics_draw_text(ctx2, SunsetToDraw, FontSunset12, SunsetRect, GTextOverflowModeWordWrap, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);
   graphics_draw_text(ctx2, SunriseToDraw, FontSunset12, SunriseRect, GTextOverflowModeWordWrap, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);
   }
 graphics_context_set_text_color(ctx2,ColorSelect(settings.Text4Color,settings.Text4ColorN));
 graphics_draw_text(ctx2, MoonToDraw, FontMoonPhase, MoonRect, GTextOverflowModeFill, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);

 graphics_context_set_text_color(ctx2, ColorSelect(settings.Text3Color, settings.Text3ColorN));
 graphics_draw_text(ctx2, datenow, FontDayOfTheWeekShortName, DateRect, GTextOverflowModeWordWrap, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);

 graphics_context_set_text_color(ctx2,ColorSelect(settings.Text1Color,settings.Text1ColorN));
 graphics_draw_text(ctx2, StepsToDraw, FontSunset, StepsRect, GTextOverflowModeFill, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentRight), NULL);

}

static void layer_update_proc_bt(Layer * layer3, GContext * ctx3){
   GRect BTIconRect =
    (PBL_IF_ROUND_ELSE(
      GRect(146,129,16,20),
      GRect(8,130,108,20)));

 onreconnection(BTOn, connection_service_peek_pebble_app_connection());
 bluetooth_callback(connection_service_peek_pebble_app_connection());

 graphics_context_set_text_color(ctx3, ColorSelect(settings.Text5Color, settings.Text5ColorN));
 graphics_draw_text(ctx3, "z", FontBTQTIcons, BTIconRect, GTextOverflowModeFill,GTextAlignmentCenter, NULL);

}

static void layer_update_proc_qt(Layer * layer4, GContext * ctx4){
  GRect QTIconRect =
    (PBL_IF_ROUND_ELSE(
      GRect(10,129,32,20),
      GRect(0-8,130,108,20)));

 quiet_time_icon();

 graphics_context_set_text_color(ctx4, ColorSelect(settings.Text5Color, settings.Text5ColorN));
 graphics_draw_text(ctx4, "\U0000E061", FontBTQTIcons, QTIconRect, GTextOverflowModeFill,GTextAlignmentCenter, NULL);

}


/////////////////////////////////////////
////Init: Handle Settings and Weather////
/////////////////////////////////////////
// Read settings from persistent storage
static void prv_load_settings(){
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, & settings, sizeof(settings));
}
// Save the settings to persistent storage
static void prv_save_settings(){
  persist_write_data(SETTINGS_KEY, & settings, sizeof(settings));

}
// Handle the response from AppMessage
static void prv_inbox_received_handler(DictionaryIterator * iter, void * context){
  s_loop = s_loop + 1;
  if (s_loop == 1){
      strcpy(settings.iconnowstring, "\U0000F03e");
      strcpy(settings.iconforestring, "\U0000F03e");
  }
  //  Colours

     Tuple * fr1_color_t = dict_find(iter, MESSAGE_KEY_FrameColor1);
  if (fr1_color_t){
    settings.FrameColor1 = GColorFromHEX(fr1_color_t-> value -> int32);
  }
  Tuple * nfr1_color_t = dict_find(iter, MESSAGE_KEY_FrameColor1N);
  if (nfr1_color_t){
    settings.FrameColor1N = GColorFromHEX(nfr1_color_t-> value -> int32);
  }

  Tuple * sd1_color_t = dict_find(iter, MESSAGE_KEY_FrameColor2);
  if (sd1_color_t){
    settings.FrameColor2 = GColorFromHEX(sd1_color_t-> value -> int32);
  }
  Tuple * nsd1_color_t = dict_find(iter, MESSAGE_KEY_FrameColor2N);
  if (nsd1_color_t){
    settings.FrameColor2N = GColorFromHEX(nsd1_color_t-> value -> int32);
  }
   Tuple * sd2_color_t = dict_find(iter, MESSAGE_KEY_FrameColor);
  if (sd2_color_t){
    settings.FrameColor = GColorFromHEX(sd2_color_t-> value -> int32);
  }
  Tuple * nsd2_color_t = dict_find(iter, MESSAGE_KEY_FrameColorN);
  if (nsd2_color_t){
    settings.FrameColorN = GColorFromHEX(nsd2_color_t-> value -> int32);
  }
  ////////////
  Tuple * tx1_color_t = dict_find(iter, MESSAGE_KEY_Text1Color);
  if (tx1_color_t){
    settings.Text1Color = GColorFromHEX(tx1_color_t-> value -> int32);
  }
  Tuple * ntx1_color_t = dict_find(iter, MESSAGE_KEY_Text1ColorN);
  if (ntx1_color_t){
    settings.Text1ColorN = GColorFromHEX(ntx1_color_t-> value -> int32);
  }
  ///////////////////////////////
  Tuple * hr_color_t = dict_find(iter, MESSAGE_KEY_HourColor);
  if (hr_color_t){
    settings.HourColor = GColorFromHEX(hr_color_t-> value -> int32);
  }
  Tuple * nthr_color_t = dict_find(iter, MESSAGE_KEY_HourColorN);
  if (nthr_color_t){
    settings.HourColorN = GColorFromHEX(nthr_color_t-> value -> int32);
  }
  Tuple * min_color_t = dict_find(iter, MESSAGE_KEY_MinColor);
  if (min_color_t){
    settings.MinColor = GColorFromHEX(min_color_t-> value -> int32);
  }
  Tuple * ntmin_color_t = dict_find(iter, MESSAGE_KEY_MinColorN);
  if (ntmin_color_t){
    settings.MinColorN = GColorFromHEX(ntmin_color_t-> value -> int32);
  }
  ///////////////////////////////
  Tuple * tx2_color_t = dict_find(iter, MESSAGE_KEY_Text2Color);
  if (tx2_color_t){
    settings.Text2Color = GColorFromHEX(tx2_color_t-> value -> int32);
  }
  Tuple * ntx2_color_t = dict_find(iter, MESSAGE_KEY_Text2ColorN);
  if (ntx2_color_t){
    settings.Text2ColorN = GColorFromHEX(ntx2_color_t-> value -> int32);
  }
   Tuple * tx3_color_t = dict_find(iter, MESSAGE_KEY_Text3Color);
  if (tx3_color_t){
    settings.Text3Color = GColorFromHEX(tx3_color_t-> value -> int32);
  }
  Tuple * ntx3_color_t = dict_find(iter, MESSAGE_KEY_Text3ColorN);
  if (ntx3_color_t){
    settings.Text3ColorN = GColorFromHEX(ntx3_color_t-> value -> int32);
  }
  Tuple * tx4_color_t = dict_find(iter,MESSAGE_KEY_Text4Color);
  if (tx4_color_t){
    settings.Text4Color = GColorFromHEX(tx4_color_t-> value -> int32);
    }
  Tuple * ntx4_color_t = dict_find(iter, MESSAGE_KEY_Text4ColorN);
  if(ntx4_color_t){
    settings.Text4ColorN = GColorFromHEX(ntx4_color_t-> value -> int32);
  }
  Tuple * tx5_color_t = dict_find(iter,MESSAGE_KEY_Text5Color);
  if (tx5_color_t){
    settings.Text5Color = GColorFromHEX(tx5_color_t-> value -> int32);
    }
  Tuple * ntx5_color_t = dict_find(iter, MESSAGE_KEY_Text5ColorN);
  if(ntx5_color_t){
    settings.Text5ColorN = GColorFromHEX(ntx5_color_t-> value -> int32);
  }
   Tuple * tx6_color_t = dict_find(iter,MESSAGE_KEY_Text6Color);
  if (tx6_color_t){
    settings.Text6Color = GColorFromHEX(tx6_color_t-> value -> int32);
    }
  Tuple * ntx6_color_t = dict_find(iter, MESSAGE_KEY_Text6ColorN);
  if(ntx6_color_t){
    settings.Text6ColorN = GColorFromHEX(ntx6_color_t-> value -> int32);
  }
  if (tx6_color_t){
    settings.Text6Color = GColorFromHEX(tx6_color_t-> value -> int32);
    }

  Tuple * tx7_color_t = dict_find(iter,MESSAGE_KEY_Text7Color);
 if (tx7_color_t){
   settings.Text7Color = GColorFromHEX(tx7_color_t-> value -> int32);
   }
 Tuple * ntx7_color_t = dict_find(iter, MESSAGE_KEY_Text7ColorN);
 if(ntx7_color_t){
   settings.Text7ColorN = GColorFromHEX(ntx7_color_t-> value -> int32);
 }
 if (tx7_color_t){
   settings.Text7Color = GColorFromHEX(tx7_color_t-> value -> int32);
   }

 Tuple * tx8_color_t = dict_find(iter,MESSAGE_KEY_Text8Color);
if (tx8_color_t){
  settings.Text8Color = GColorFromHEX(tx8_color_t-> value -> int32);
  }
Tuple * ntx8_color_t = dict_find(iter, MESSAGE_KEY_Text8ColorN);
if(ntx8_color_t){
  settings.Text8ColorN = GColorFromHEX(ntx8_color_t-> value -> int32);
}
if (tx8_color_t){
  settings.Text8Color = GColorFromHEX(tx8_color_t-> value -> int32);
  }

Tuple * tx9_color_t = dict_find(iter,MESSAGE_KEY_Text9Color);
if (tx9_color_t){
 settings.Text9Color = GColorFromHEX(tx9_color_t-> value -> int32);
 }
Tuple * ntx9_color_t = dict_find(iter, MESSAGE_KEY_Text9ColorN);
if(ntx9_color_t){
 settings.Text9ColorN = GColorFromHEX(ntx9_color_t-> value -> int32);
}
if (tx9_color_t){
 settings.Text9Color = GColorFromHEX(tx9_color_t-> value -> int32);
 }

Tuple * tx10_color_t = dict_find(iter,MESSAGE_KEY_Text10Color);
if (tx10_color_t){
settings.Text10Color = GColorFromHEX(tx10_color_t-> value -> int32);
}
Tuple * ntx10_color_t = dict_find(iter, MESSAGE_KEY_Text10ColorN);
if(ntx10_color_t){
settings.Text10ColorN = GColorFromHEX(ntx10_color_t-> value -> int32);
}
if (tx10_color_t){
settings.Text10Color = GColorFromHEX(tx10_color_t-> value -> int32);
}
// Weather conditions
//Tuple * weather_t = dict_find(iter, MESSAGE_KEY_Weathertime);
//if (weather_t){
//  settings.Weathertimecapture = (int) weather_t -> value -> int32;
//   snprintf(settings.weathertimecapture, sizeof(settings.weathertimecapture), "%s", weather_t -> value -> cstring);
//  APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather data captured at time %d",settings.Weathertimecapture);
//}
//  if (s_countdown == 0 || s_countdown == 5)
//  {
/*  Tuple * wcond_t = dict_find(iter, MESSAGE_KEY_WeatherCond);
  if (wcond_t){
  snprintf(settings.condstring, sizeof(settings.condstring), "%s", wcond_t -> value -> cstring);
}*/

  Tuple * wtemp_t = dict_find(iter, MESSAGE_KEY_WeatherTemp);
  if (wtemp_t){
  snprintf(settings.tempstring, sizeof(settings.tempstring), "%s", wtemp_t -> value -> cstring);
  }
  Tuple * wwind_t = dict_find(iter, MESSAGE_KEY_WeatherWind);
  if (wwind_t){
  snprintf(settings.windstring, sizeof(settings.windstring), "%s", wwind_t -> value -> cstring);
  }
  Tuple * iconwinddirnow_tuple = dict_find(iter, MESSAGE_KEY_WindIconNow);
  if (iconwinddirnow_tuple){
  snprintf(settings.windiconnowstring,sizeof(settings.windiconnowstring),"%s",wind_direction[(int)iconwinddirnow_tuple->value->int32]);
      //  snprintf(forecast_result,sizeof(forecast_result),"%s","\U0000F002");
  }
  Tuple * iconnow_tuple = dict_find(iter, MESSAGE_KEY_IconNow);
    //////////Add in icons and forecast hi/lo temp////////////
    if (iconnow_tuple){
      snprintf(settings.iconnowstring,sizeof(settings.iconnowstring),"%s",weather_conditions[(int)iconnow_tuple->value->int32]);
  }

  Tuple * wwindave_t = dict_find(iter, MESSAGE_KEY_WindFore);
  if (wwindave_t){
    snprintf(settings.windavestring, sizeof(settings.windavestring), "%s", wwindave_t -> value -> cstring);
  }

    //Hour Sunrise and Sunset
  Tuple * sunrise_t = dict_find(iter, MESSAGE_KEY_HourSunrise);
  if (sunrise_t){
    HourSunrise = (int) sunrise_t -> value -> int32;
  }
  Tuple * sunset_t = dict_find(iter, MESSAGE_KEY_HourSunset);
  if (sunset_t){
    HourSunset = (int) sunset_t -> value -> int32;
  }

  Tuple * sunset_dt = dict_find(iter, MESSAGE_KEY_WEATHER_SUNSET_KEY);
  if (sunset_dt){
     snprintf(settings.sunsetstring, sizeof(settings.sunsetstring), "%s", sunset_dt -> value -> cstring);
  }
   Tuple * sunrise_dt = dict_find(iter, MESSAGE_KEY_WEATHER_SUNRISE_KEY);
  if (sunrise_dt){
     snprintf(settings.sunrisestring, sizeof(settings.sunrisestring), "%s", sunrise_dt -> value -> cstring);
  }

  //12hr version of sunset & sunrise
  Tuple * sunset12_dt = dict_find(iter, MESSAGE_KEY_WEATHER_SUNSET_KEY_12H);
  if (sunset12_dt){
     snprintf(settings.sunsetstring12, sizeof(settings.sunsetstring12), "%s", sunset12_dt -> value -> cstring);
  }
   Tuple * sunrise12_dt = dict_find(iter, MESSAGE_KEY_WEATHER_SUNRISE_KEY_12H);
  if (sunrise12_dt){
     snprintf(settings.sunrisestring12, sizeof(settings.sunrisestring12), "%s", sunrise12_dt -> value -> cstring);
  }


  Tuple * iconfore_tuple = dict_find(iter, MESSAGE_KEY_IconFore);
  if (iconfore_tuple){
    snprintf(settings.iconforestring,sizeof(settings.iconforestring),"%s",weather_conditions[(int)iconfore_tuple->value->int32]);
    //  snprintf(forecast_result,sizeof(forecast_result),"%s","\U0000F002");
  }

   Tuple * iconwinddirave_tuple = dict_find(iter, MESSAGE_KEY_WindIconAve);
  if (iconwinddirave_tuple){
    snprintf(settings.windiconavestring,sizeof(settings.windiconavestring),"%s",wind_direction[(int)iconwinddirave_tuple->value->int32]);
    //  snprintf(forecast_result,sizeof(forecast_result),"%s","\U0000F002");
  }

  Tuple * wforetemp_t = dict_find(iter, MESSAGE_KEY_TempFore);
  if (wforetemp_t){
    snprintf(settings.temphistring, sizeof(settings.temphistring), "%s", wforetemp_t -> value -> cstring);
  }

  Tuple * wforetemplow_t = dict_find(iter, MESSAGE_KEY_TempForeLow);
  if (wforetemplow_t){
    snprintf(settings.templowstring, sizeof(settings.templowstring), "%s", wforetemplow_t -> value -> cstring);
  }

  Tuple * moon_tuple = dict_find(iter, MESSAGE_KEY_MoonPhase);
  if (moon_tuple){
    snprintf(settings.moonstring,sizeof(settings.moonstring),"%s",moon_phase[(int)moon_tuple->value->int32]);
    //  snprintf(forecast_result,sizeof(forecast_result),"%s","\U0000F002");
  }

  Tuple * weather_t = dict_find(iter, MESSAGE_KEY_Weathertime);
  if (weather_t){
    settings.Weathertimecapture = (int) weather_t -> value -> int32;
  //   snprintf(settings.weathertimecapture, sizeof(settings.weathertimecapture), "%s", weather_t -> value -> cstring);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather data captured at time %d",settings.Weathertimecapture);
  }
//}
  //  APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather update time is %s", settings.FontChoice);

  Tuple * font_tuple = dict_find(iter, MESSAGE_KEY_FontChoice);
  if (font_tuple){
      snprintf(settings.FontChoice,sizeof(settings.FontChoice),"%s",font_tuple->value->cstring);
      APP_LOG(APP_LOG_LEVEL_DEBUG, "font seting is %s", settings.FontChoice);
  //    fontstring = settings.FontChoice;
  //    APP_LOG(APP_LOG_LEVEL_DEBUG, "fontstring is %s", fontstring);
    }

  Tuple * frequpdate = dict_find(iter, MESSAGE_KEY_UpSlider);
  if (frequpdate){
    settings.UpSlider = (int) frequpdate -> value -> int32;
    //Restart the counter
    s_countdown = settings.UpSlider;
  }

  Tuple * disntheme_t = dict_find(iter, MESSAGE_KEY_NightTheme);
  if (disntheme_t){
    if (disntheme_t -> value -> int32 == 0){
      settings.NightTheme = false;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "NTHeme off");
    } else {
    settings.NightTheme = true;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "NTHeme on");
      }
    }

  Tuple * health_t = dict_find(iter, MESSAGE_KEY_HealthOff);
  if (health_t){
    if (health_t -> value -> int32 == 0){
      settings.HealthOff = false;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Health on");
      get_step_count();
      display_step_count();
    } else {
      settings.HealthOff = true;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Health off");
      snprintf(s_current_steps_buffer, sizeof(s_current_steps_buffer),
       "%s", "");
    }
  }

  Tuple * addzero12_t = dict_find(iter, MESSAGE_KEY_AddZero12h);
  if (addzero12_t){
    if (addzero12_t -> value -> int32 == 0){
      settings.AddZero12h = false;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Add Zero 12h off");
    } else {
    settings.AddZero12h = true;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Add Zero 12h on");
      }
    }

  Tuple * remzero24_t = dict_find(iter, MESSAGE_KEY_RemoveZero24h);
  if (remzero24_t){
    if (remzero24_t -> value -> int32 == 0){
      settings.RemoveZero24h = false;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Remove Zero 24h off");
    } else {
    settings.RemoveZero24h = true;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Remove Zero 24h off");
      }
    }

  layer_mark_dirty(s_canvas_top_section);
  layer_mark_dirty(time_area_layer);
  layer_mark_dirty(date_area_layer);
  layer_mark_dirty(s_canvas_bottom_section);
  layer_mark_dirty(s_canvas_bt_icon);
  layer_mark_dirty(s_canvas_qt_icon);

  prv_save_settings();
}



//Load main layer
static void window_load(Window * window){
  Layer * window_layer = window_get_root_layer(window);
  GRect bounds4 = layer_get_bounds(window_layer);

  s_canvas_background = layer_create(bounds4);
    layer_set_update_proc(s_canvas_background, layer_update_proc_background);
    layer_add_child(window_layer, s_canvas_background);

  time_area_layer = layer_create(bounds4);
     layer_add_child(window_layer, time_area_layer);
     layer_set_update_proc(time_area_layer, update_time_area_layer);

  date_area_layer = layer_create(bounds4);
     layer_add_child(window_layer, date_area_layer);
     layer_set_update_proc(date_area_layer, update_date_area_layer);

  s_canvas_bottom_section = layer_create(bounds4);
    layer_set_update_proc (s_canvas_bottom_section, layer_update_proc_sunset);
    layer_add_child(window_layer, s_canvas_bottom_section);

  s_canvas_bt_icon = layer_create(bounds4);
    layer_set_update_proc (s_canvas_bt_icon, layer_update_proc_bt);
    layer_add_child(window_layer, s_canvas_bt_icon);

  s_canvas_qt_icon = layer_create(bounds4);
    layer_set_update_proc (s_canvas_qt_icon, layer_update_proc_qt);
    layer_add_child(window_layer, s_canvas_qt_icon);


/*  s_step_layer = text_layer_create (PBL_IF_ROUND_ELSE(
      GRect(0, 156-8, 180, 40),
      GRect(0, 116+2, 142, 40)));
      text_layer_set_background_color(s_step_layer, GColorClear);
      text_layer_set_font(s_step_layer,
                        FontTempFore);
      text_layer_set_text_alignment(s_step_layer, (PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentRight)));
      layer_add_child(window_layer, text_layer_get_layer(s_step_layer));
*/

  s_canvas_top_section = layer_create(bounds4);
      layer_set_update_proc(s_canvas_top_section, layer_update_proc);
      layer_add_child(window_layer, s_canvas_top_section);
  }


static void window_unload(Window * window){
  layer_destroy (s_canvas_background);
  //text_layer_destroy (s_step_layer);
  layer_destroy(s_canvas_top_section);
  layer_destroy(time_area_layer);
  layer_destroy(date_area_layer);
  layer_destroy(s_canvas_bottom_section);
  layer_destroy(s_canvas_bt_icon);
  layer_destroy(s_canvas_qt_icon);
  window_destroy(s_window);
  fonts_unload_custom_font(FontMoonPhase);
  fonts_unload_custom_font(FontBTQTIcons);
  fonts_unload_custom_font(FontWeatherIcons);
  fonts_unload_custom_font(FontWeatherCondition);
  fonts_unload_custom_font(FontWindDirection);
  ffont_destroy(time_font);
  #ifdef PBL_MICROPHONE
  ffont_destroy(time_font_opend);
  ffont_destroy(time_font_copse);
  #endif
}
void main_window_push(){
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers){
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_window, true);

//   update_time();
}
void main_window_update(int hours, int minutes, int weekday, int day){
  s_hours = hours;
  s_minutes = minutes;
  s_day = day;
  s_weekday = weekday;

//  layer_mark_dirty(s_canvas_background);
//  layer_mark_dirty(s_canvas);
//  layer_mark_dirty(time_area_layer);
//  layer_mark_dirty(date_area_layer);
  layer_mark_dirty(s_canvas_bottom_section);
//  layer_mark_dirty(s_canvas_bt_icon);
//  layer_mark_dirty(s_canvas_qt_icon);

}

static void tick_handler(struct tm * time_now, TimeUnits changed){

  main_window_update(time_now -> tm_hour, time_now -> tm_min, time_now -> tm_wday, time_now -> tm_mday);
  //update_time();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Tick at %d", time_now -> tm_min);
  //s_loop = 0;
  if (s_countdown == 0){
    //Reset
    s_countdown = settings.UpSlider;
  } else{
    s_countdown = s_countdown - 1;
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Countdown to update %d", s_countdown);


  // Evaluate if is day or night
  int nowthehouris = s_hours * 100 + s_minutes;
  if (HourSunrise <= nowthehouris && nowthehouris <= HourSunset){
    IsNightNow = false;
  } else{
    IsNightNow = true;
  }
  // Extra catch on sunrise and sunset
  if (nowthehouris == HourSunrise || nowthehouris == HourSunset){
    s_countdown = 1;
  }
  if (GPSOn && settings.NightTheme){
    //Extra catch around 1159 to gather information of today
    if (nowthehouris == 1159 && s_countdown > 5){
      s_countdown = 1;
    };
    // Change Color of background
    layer_mark_dirty(s_canvas_background);
    layer_mark_dirty(s_canvas_top_section);
    layer_mark_dirty(time_area_layer);
    layer_mark_dirty(date_area_layer);
    layer_mark_dirty(s_canvas_bottom_section);
    layer_mark_dirty(s_canvas_bt_icon);
    layer_mark_dirty(s_canvas_qt_icon);

  }
  // Get weather update every requested minutes and extra request 5 minutes earlier
  if (s_countdown == 0 || s_countdown == 5){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "countdown is 0, so updated weather at %d", time_now -> tm_min);
      request_watchjs();
  }
  //If GPS was off request weather every 15 minutes
  if (!GPSOn){
          if (s_countdown % 15 == 0){
          APP_LOG(APP_LOG_LEVEL_DEBUG, "Attempt to request GPS on %d", time_now -> tm_min);
          request_watchjs();
        }

    }
 }

static void init(){

  prv_load_settings();

  // Listen for AppMessages
  //Starting loop at 0
  s_loop = 0;
  //s_countdown = settings.UpSlider;
  //Clean vars

//  strcpy(settings.windiconnowstring, "\U0000F04B");
//  strcpy(settings.windiconavestring, "\U0000F04B");

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  s_hours=t->tm_hour;
  s_minutes=t->tm_min;
  s_day=t->tm_mday;
  s_weekday=t->tm_wday;
  //Register and open
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(512, 512);
  // Load Fonts
   // allocate fonts
  time_font =  ffont_create_from_resource(RESOURCE_ID_FONT_STEELFISHRG);
  #ifdef PBL_MICROPHONE
  time_font_opend =  ffont_create_from_resource(RESOURCE_ID_FONT_ZEP);
  time_font_copse =  ffont_create_from_resource(RESOURCE_ID_FONT_COPSE);
  #endif
  FontDayOfTheWeekShortName = fonts_get_system_font(PBL_IF_ROUND_ELSE(FONT_KEY_GOTHIC_28_BOLD,FONT_KEY_GOTHIC_24_BOLD));
  FontSunset = fonts_get_system_font(PBL_IF_ROUND_ELSE(FONT_KEY_GOTHIC_18_BOLD,FONT_KEY_GOTHIC_24_BOLD));
  FontSunset12 = fonts_get_system_font(PBL_IF_ROUND_ELSE(FONT_KEY_GOTHIC_18,FONT_KEY_GOTHIC_24));
  FontMoonPhase = fonts_load_custom_font(resource_get_handle(PBL_IF_ROUND_ELSE(RESOURCE_ID_FONT_WEATHERICONS_22,RESOURCE_ID_FONT_WEATHERICONS_26)));
  FontBTQTIcons = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DRIPICONS_16));
  FontTemp = PBL_IF_ROUND_ELSE(fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD),fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  FontTempFore = PBL_IF_ROUND_ELSE(fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  FontWeatherIcons = fonts_load_custom_font(resource_get_handle(PBL_IF_ROUND_ELSE(RESOURCE_ID_FONT_WEATHERICONS_16,RESOURCE_ID_FONT_WEATHERICONS_18)));
  FontWeatherCondition = fonts_load_custom_font(resource_get_handle(PBL_IF_ROUND_ELSE(RESOURCE_ID_FONT_WEATHERICONS_26,RESOURCE_ID_FONT_WEATHERICONS_30)));
  FontWindDirection = fonts_load_custom_font(resource_get_handle(PBL_IF_ROUND_ELSE(RESOURCE_ID_FONT_WEATHERICONS_32,RESOURCE_ID_FONT_WEATHERICONS_32)));

  main_window_push();
  // Register with Event Services
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
//  battery_callback(battery_state_service_peek());

  if (!settings.HealthOff && step_data_is_available())  {
      health_service_events_subscribe(health_handler,NULL);
      APP_LOG(APP_LOG_LEVEL_DEBUG, "health is on, and steps data is subscribed");
//          health_service_events_subscribe(NULL,NULL);
    }
  //#endif

  connection_service_subscribe((ConnectionHandlers){
    .pebble_app_connection_handler = bluetooth_vibe_icon
  });
  bluetooth_vibe_icon(connection_service_peek_pebble_app_connection());
  accel_tap_service_subscribe(accel_tap_handler);
}
static void deinit(){
  tick_timer_service_unsubscribe();
  app_message_deregister_callbacks();
  battery_state_service_unsubscribe();
  connection_service_unsubscribe();
  health_service_events_unsubscribe();
  accel_tap_service_unsubscribe();
}
int main(){
  init();
  app_event_loop();
  deinit();
}
