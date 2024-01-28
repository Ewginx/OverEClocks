#pragma once
#include "StateApp.h"
#include <Arduino.h>
#include <WiFi.h>
#include <lvgl.h>

class WiFiApp {
  public:
    StateApp *_state_app;
    SemaphoreHandle_t &mutex;

    void connect_to_wifi();
    void handle_wifi_state(bool wifi_connected);
    void subscribe_to_wifi_disconnected_event();

    WiFiApp(StateApp *state_app, SemaphoreHandle_t &mutex);
    ~WiFiApp();
};
