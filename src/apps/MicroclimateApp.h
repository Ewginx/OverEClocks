#pragma once
#include "Config/Config.h"
#include "GUI/DockPanel.h"
#include "StateApp.h"
#include <Arduino.h>
#include <SHT31.h>
#include <SensirionI2cSht4x.h>

#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

class MicroclimateApp {
  public:
    void temperatureSensorTimerCallback();
    bool begin();
    MicroclimateApp(DockPanel *dockPanel, StateApp *stateApp);
    ~MicroclimateApp();

  private:
    StateApp *stateApp;
    SensirionI2cSht4x temperatureSensor;
    lv_timer_t *temperatureSensorTimer = NULL;
    DockPanel *dockPanel;

    char errorMessage[64];
    int16_t error;
  
    void getSensorReadings();
    // float getTemperature();
    // int getHumidity();
};
