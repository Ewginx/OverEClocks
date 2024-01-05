#pragma once
#include "BrightnessApp.h"
#include "Config/Config.h"
#include "MicroclimateApp.h"
#include "SPIFFS.h"
#include "StateApp.h"
#include "lvgl.h"
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>

class ServerApp {
  private:
    short int port = 80;
    AsyncWebServer server;
    AsyncWebSocket websocket;
    lv_timer_t *_websocket_notify_timer = NULL;
    BrightnessApp *_brightness_app;
    MicroclimateApp *_microclimate_app;
    
  public:
    StateApp *_state_app;

    void get_settings(AsyncWebServerRequest *request);

    void save_time_settings(JsonVariant &json);
    void save_weather_settings(JsonVariant &json);
    void save_theme_settings(JsonVariant &json);
    void save_brightness_settings(JsonVariant &json);
    void save_wifi_settings(JsonVariant &json);

    void set_time(JsonVariant &json);

    void setup();
    void run();
    void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                 void *arg, uint8_t *data, size_t len);
    void websocket_timer_cb(lv_timer_t *timer);
    String getSensorReadings();

    ServerApp(StateApp *state_app, BrightnessApp *brightness_app, MicroclimateApp *microclimate_app);
    ~ServerApp();
};
