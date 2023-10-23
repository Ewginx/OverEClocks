#include "OEClockApp.h"

OEClockApp *app;
// #define ONE_MINUTE_MS (60 * 1000)
// #define ONE_HOUR_MS (60 * 60 * 1000)
// #define TWELVE_HOUR_MS (12 * 60 * 60 * 1000)


// LGFX tft;
// #if LV_USE_LOG != 0
// /* Serial debugging */
// void my_print(const char * buf)
// {
//     Serial.printf(buf);
//     Serial.flush();
// }
// #endif

void setup() {
  app = new OEClockApp();
  app->setup();
  // #if LV_USE_LOG != 0
  //     lv_log_register_print_cb( my_print ); /* register print function for debugging */
  // #endif
}

void loop()
{
  app->loop();
}

