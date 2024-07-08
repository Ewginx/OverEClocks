#pragma once
#include "Config/Config.h"
#include "StateApp.h"
#include "lvgl.h"
#include <Arduino.h>
#include <DFMiniMp3.h>

class SoundApp;

typedef DFMiniMp3<HardwareSerial, DfMp3NoCallback, Mp3ChipOriginal, 900> DfMp3;

class SoundApp {
  public:
    StateApp *stateApp;

    void setup();

    void playAlarmSound();
    void loopAlarmSound();
    void playEasterEggSound();
    void playPlugInSound();
    void stopSound();
    void handlePlayerUsb();

    SoundApp(StateApp *stateApp);
    ~SoundApp();

  private:
    DfMp3 player;
    lv_timer_t *soundLoopTimer = NULL;

    uint8_t alarmTrackFolder = 1;
    uint8_t easterEggTrackFolder = 2;

    bool needToResetMp3Module = false;

    void setVolume();
    void playSoundFromFolderOnce(uint8_t folder, uint8_t track);

    void resetPlayer();
    bool canPlaySounds(bool soundOn);
    bool isPlaying();
    bool isIdling();
    bool isPlayerOffline();

    int getTrackCountInAlarmFolder();
    int getTrackCountInEasterEggFolder();
};
