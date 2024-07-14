#pragma once
#include "Config/Config.h"
#include "StateApp.h"
#include "lvgl.h"
#include <Arduino.h>

class BatteryApp {
  public:
    StateApp *stateApp;

    void setup();
    void measureBatteryLevelAndSendMsg();
    int calculateBatteryPercentage(float batteryVoltage);
    void checkChargeStatusAndSendMsg();
    int sigmoidal(float voltage, float minVoltage, float maxVoltage);
    void getVoltageSample();
    float calculateBatteryVoltage();

    BatteryApp(StateApp *stateApp);
    ~BatteryApp();

  private:
    lv_timer_t *batteryLevelTimer = NULL;
    lv_timer_t *batteryChargingTimer = NULL;
    float voltageSamples[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    int voltageSamplesCount = 0;
};
