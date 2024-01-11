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
  private:
    short int port = 80;
    AsyncWebServer server;
    AsyncWebSocket websocket;
    lv_timer_t *_websocket_notify_timer = NULL;
    BrightnessApp *_brightness_app;
    MicroclimateApp *_microclimate_app;
    bool espShouldReboot = false;

  public:
    StateApp *_state_app;

    void get_settings(AsyncWebServerRequest *request);

    void save_time_settings(JsonVariant &json);
    void save_weather_settings(JsonVariant &json);
    void save_theme_settings(JsonVariant &json);
    void save_brightness_settings(JsonVariant &json);
    void save_wifi_settings(JsonVariant &json);
    void save_alarm_clock_settings(JsonVariant &json);

    void set_time(JsonVariant &json);

    void setup_settings_handlers();
    void setup_redirect_handlers();
    void setup_set_time_handler();
    void setup_fw_update_handler();
    void setup_fs_update_handler();

    void setup();
    void run();
    void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                 void *arg, uint8_t *data, size_t len);
    void websocket_timer_cb(lv_timer_t *timer);
    String getSensorReadings();

    ServerApp(StateApp *state_app, BrightnessApp *brightness_app,
              MicroclimateApp *microclimate_app);
    ~ServerApp();
};
