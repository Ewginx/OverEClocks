#pragma once
#include "Config/Config.h"
#include "StateApp.h"
#include "lvgl.h"
#include <Arduino.h>
#include <DFMiniMp3.h>

class SoundApp;

typedef DFMiniMp3<HardwareSerial, DfMp3NoCallback, Mp3ChipOriginal, 900> DfMp3;

class SoundApp {
  private:
    DfMp3 player;
    lv_timer_t *sound_loop_timer = NULL;
    short int alarm_track_folder = 1;
    short int easter_egg_track_folder = 2;
    bool need_to_reset = false;

  public:
    StateApp *_state_app;
    void play_alarm_sound();
    void loop_alarm_sound();
    void set_volume();
    void play_ee_sound();
    void play_plug_in_sound();
    void play_sound_from_folder_once(int folder, int track);
    bool is_playing();
    bool is_idling();
    void stop_sound();
    void reset_player();
    bool can_play_sounds(bool sound_on);
    int get_track_count_in_alarm_folder();
    int get_track_count_in_ee_folder();

    void handle_player_usb();

    void setup_player();
    bool is_player_offline();
    SoundApp(StateApp *state_app);
    ~SoundApp();
};
