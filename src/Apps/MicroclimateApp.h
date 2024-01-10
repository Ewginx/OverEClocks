#pragma once
#include "Config/Config.h"
#include "GUI/DockPanel.h"
#include <Adafruit_BME280.h>
#include <Arduino.h>

class MicroclimateApp {
  private:
    Adafruit_BME280 _bme_sensor;
    lv_timer_t *_bme_timer = NULL;
    DockPanel *_dock_panel;

  public:
    float get_temperature();
    int get_humidity();
    void bme_timer_cb(lv_timer_t *timer);
    bool begin();
    MicroclimateApp(DockPanel *dock_panel);
    ~MicroclimateApp();
};
