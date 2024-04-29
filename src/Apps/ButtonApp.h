#pragma once
#include "Config/Config.h"
#include "StateApp.h"
#include "lvgl.h"
#include <Arduino.h>


class ButtonApp {
  private:
    int measure_delay = 300;
    int elapsed_time = 0;
    bool was_pressed = false;
    lv_timer_t *button_timer = NULL;

  public:
    StateApp *_state_app;
    void measure_time_button_press();
    ButtonApp(StateApp *state_app);
    ~ButtonApp();
};
