#pragma once
#include "Config/Config.h"
#include "GUI/DockPanel.h"
#include "StateApp.h"
#include <Arduino.h>
#include <SHT31.h>


class MicroclimateApp {
  private:
    SHT31  _temp_sensor;
    lv_timer_t *_temp_sensor_timer = NULL;
    DockPanel *_dock_panel;

  public:
      StateApp *_state_app;
    float get_temperature();
    int get_humidity();
    void temp_sensor_timer_cb(lv_timer_t *timer);
    bool begin();
    MicroclimateApp(DockPanel *dock_panel, StateApp *state_app);
    ~MicroclimateApp();
};
