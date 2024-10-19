#pragma once
#include "StateApp.h"
#include <Arduino.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <lvgl.h>

class WiFiApp {
  public:
    int wifi_reconnect_attempts = 4;

    void setup();
    bool isWiFiConnected();
    void connectToWiFi();
    void handleWiFiState(bool wifiConnected);

    WiFiApp(StateApp *state_app, SemaphoreHandle_t &mutex);
    ~WiFiApp();

  private:
    StateApp *stateApp;
    SemaphoreHandle_t &mutex;

    void setupMDNS();
    void initiateAPMode();
    void notifyOtherApps();
    void subscribeToWiFiDisconnectedEvent();
};
