#pragma once
#include "Config/Config.h"
#include "StateApp.h"
#include "lvgl.h"
#include <Arduino.h>

class ButtonApp {
  public:
    void measureTimeButtonIsPressed();
    ButtonApp(StateApp *stateApp);
    ~ButtonApp();

  private:
    StateApp *stateApp;
    lv_timer_t *buttonTimer = NULL;

    int measureDelay = 300;
    int elapsedTime = 0;
    bool wasPressed = false;
    void longPress();
    void shortPress();
};
