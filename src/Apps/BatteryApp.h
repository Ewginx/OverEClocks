#pragma once
#include "Config/Config.h"
#include "StateApp.h"
#include "lvgl.h"
#include <Arduino.h>

class BatteryApp {
  private:
    lv_timer_t *battery_level_timer = NULL;
    lv_timer_t *battery_charge_timer = NULL;

  public:
    StateApp *_state_app;
    void measure_battery_level_and_send_msg();
    int calculate_battery_percentage(int battery_adc);
    void check_charge_status_and_send_msg();
    uint8_t sigmoidal(uint16_t voltage, uint16_t minVoltage, uint16_t maxVoltage);

    void setup_battery_app();
    BatteryApp(StateApp *state_app);
    ~BatteryApp();
};
