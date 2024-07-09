#pragma once
#include "BrightnessApp.h"
#include "Config/Config.h"
#include "MicroclimateApp.h"
#include "StateApp.h"
#include "lvgl.h"
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <Update.h>

class ServerApp {
  public:
    StateApp *stateApp;

    void setup();
    void run();

    void websocketTimerCallback(lv_timer_t *timer);

    void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                 void *arg, uint8_t *data, size_t len);

    ServerApp(StateApp *stateApp, BrightnessApp *brightnessApp);
    ~ServerApp();

  private:
    uint16_t port = 80;

    AsyncWebServer server;

    AsyncWebSocket websocket;

    lv_timer_t *_websocketNotifyTimer = NULL;

    BrightnessApp *brightnessApp;

    MicroclimateApp *microclimateApp;

    bool espShouldReboot = false;

    void setupIndexHandler();
    void setupSettingsHandlers();
    void setupRedirectHandlers();
    void setupSetTimeHandler();
    void setupFirmwareUpdateHandler();
    void setupFilesystemUpdateHandler();
    void setupWeatherImagesUploadHandlers();
    void setupGIFUploadHandler();
    void setupWebUiUploadHandler();
    void setupAnalogClockImagesUploadHandler();

    void saveTimeSettings(JsonVariant &json);
    void saveWeatherSettings(JsonVariant &json);
    void saveThemeSettings(JsonVariant &json);
    void saveBrightnessSettings(JsonVariant &json);
    void saveWiFiSettings(JsonVariant &json);
    void saveAlarmClockSettings(JsonVariant &json);
    void saveRGBSettings(JsonVariant &json);
    void saveSoundSettings(JsonVariant &json);

    void getSettings(AsyncWebServerRequest *request);

    void setTime(JsonVariant &json);

    void handleUpload(AsyncWebServerRequest *request, String filename, size_t index,
                      uint8_t *data, size_t len, bool final, const char *path);

    String getInfoForWS();
};
