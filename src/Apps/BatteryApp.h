#pragma once
#include "Config/Config.h"
#include "StateApp.h"
#include "lvgl.h"
#include <Arduino.h>

class BatteryApp {
  private:
    lv_timer_t *battery_level_timer = NULL;
    lv_timer_t *battery_charge_timer = NULL;
    float _voltage_samples[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    int _samples_count = 0;

  public:
    StateApp *_state_app;
    void measure_battery_level_and_send_msg();
    int calculate_battery_percentage(float battery_voltage);
    void check_charge_status_and_send_msg();
    uint8_t sigmoidal(float voltage, float minVoltage, float maxVoltage);
    void get_voltage_sample();
    float calculate_battery_voltage();

    void setup();
    BatteryApp(StateApp *state_app);
    ~BatteryApp();
};
