#pragma once
#include "Config/Config.h"
#include "GUI/DockPanel.h"
#include "StateApp.h"
#include <Arduino.h>
#include <SHT31.h>

class MicroclimateApp {
  public:
    void temperatureSensorTimerCallback();
    bool begin();
    MicroclimateApp(DockPanel *dockPanel, StateApp *stateApp);
    ~MicroclimateApp();

  private:
    StateApp *stateApp;
    SHT31 temperatureSensor;
    lv_timer_t *temperatureSensorTimer = NULL;
    DockPanel *dockPanel;

    float getTemperature();
    int getHumidity();
};
