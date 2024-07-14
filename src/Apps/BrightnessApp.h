#pragma once
#include "Config/Config.h"
#include "Display/Display.h"
#include "GUI/Settings.h"
#include "StateApp.h"
#include <Arduino.h>
#include <hp_BH1750.h>

class BrightnessApp {
  public:
    StateApp *stateApp;

    void setup();
    bool begin();
    void setAutoBrightnessTimer(bool autoBrightness);
    void lightSensorTimerCallback();
    void setDisplayBrightness(uint8_t brightness);
    int getLightLevel();

    BrightnessApp(Display *display, Settings *settings, StateApp *stateApp);
    ~BrightnessApp();

  private:
    hp_BH1750 lightSensor;
    lv_timer_t *lightSensorTimer = NULL;
    lv_anim_t brightnessLevelAnimation;
    Display *display;
    Settings *settings;

    void switchTheme(uint8_t brightness);
    int mapLightLevel(int lightLevel);
    void changeBrightnessSmoothly(int newLightLevel, int oldLightLevel);
};
