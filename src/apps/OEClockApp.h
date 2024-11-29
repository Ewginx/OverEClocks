#pragma once
#include "BatteryApp.h"
#include "BrightnessApp.h"
#include "ButtonApp.h"
#include "Config/Config.h"
#include "Display/Display.h"
#include "Filesystem/lv_fs_littlefs.h"
#include "Filesystem/lv_fs_sdcard.h"
#include "GUI/GuiApp.h"
#include "MicroclimateApp.h"
#include "RGBApp.h"
#include "ServerApp.h"
#include "SoundApp.h"
#include "StateApp.h"
#include "TimeApp.h"
#include "WeatherApp.h"
#include "WiFiApp.h"
#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>

class OEClockApp {
  public:
    StateApp *stateApp;
    Display *display;
    GuiApp *guiApp;
    ServerApp *serverApp;
    WiFiApp *wifiApp;
    RGBApp *rgbApp;

    void setup();
    void loop();
    OEClockApp();
    ~OEClockApp();

  private:
    TimeApp *timeApp;
    WeatherApp *weatherApp;
    BrightnessApp *brightnessApp;
    MicroclimateApp *microclimateApp;
    SoundApp *soundApp;
    ButtonApp *buttonApp;
    BatteryApp *batteryApp;

    void initI2CApps();
    void initGUI();
    void initApps();
    void printHeapInformation();
    TaskHandle_t createLoadingScreenTask();
    void deleteLoadingScreenTask(TaskHandle_t updateDisplayTask);
};
