#pragma once
#include "Config/Config.h"
#include <DFPlayerMini_Fast.h>
#include "StateApp.h"
#include "lvgl.h"
#include <Arduino.h>


class SoundApp {
  private:
    DFPlayerMini_Fast  player;
    lv_timer_t *sound_loop_timer = NULL;

  public:

    StateApp *_state_app;
    void play_alarm_sound();
    void loop_alarm_sound();
    void stop_sound();
    void play_ee();
    int get_file_counts();

    void setup_player();

    SoundApp(StateApp *state_app);
    ~SoundApp();
};
