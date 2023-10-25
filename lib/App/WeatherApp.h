#pragma once
#include "Arduino.h"
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <HttpClient.h>
#include <WiFiClient.h>
#include"Config.h"

class WeatherApp
{
private:
    short int port = 80;
    String weather_url = "jsonplaceholder.typicode.com";
    TaskHandle_t Weather_Task;
public:
    WiFiClient wifi;
    HttpClient client = HttpClient(wifi, weather_url, port);

    void create_weather_task();
    static void send_weather_request(void *parameter);

    WeatherApp();
    ~WeatherApp();
};


