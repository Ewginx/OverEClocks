#include "OEClockApp.h"

OEClockApp *app;

// #if LV_USE_LOG != 0
// /* Serial debugging */
// void my_print(const char * buf)
// {
//     Serial.printf(buf);
//     Serial.flush();
// }
// #endif

void setup()
{
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
