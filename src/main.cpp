#include "Apps/OEClockApp.h"

OEClockApp *app;

void setup()
{
  app = new OEClockApp();
  app->setup();
}

void loop()
{
  app->loop();
}
