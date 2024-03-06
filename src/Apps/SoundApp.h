#pragma once
#include "Config/Config.h"
#include "StateApp.h"
#include "lvgl.h"
#include <Arduino.h>
#include <DFMiniMp3.h>

typedef DFMiniMp3<HardwareSerial> DfMp3;

class SoundApp {
  private:
    DfMp3 player;
    lv_timer_t *sound_loop_timer = NULL;
    short int alarm_track_folder = 1;
    short int easter_egg_track_folder = 2;

  public:
    StateApp *_state_app;
    void play_alarm_sound();
    void loop_alarm_sound();
    void set_volume();
    void play_ee();

    void play_sound_once(int folder, int track);
    bool is_playing();
    bool is_idling();
    void stop_sound();

    int get_track_count_in_alarm_folder();
    int get_track_count_in_ee_folder();

    void setup_player();

    SoundApp(StateApp *state_app);
    ~SoundApp();
};
