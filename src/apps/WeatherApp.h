#pragma once
#include "Config/Config.h"
#include "GUI/Weather.h"
#include "StateApp.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HttpClient.h>
#include <WiFi.h>
#include <WiFiClient.h>

class WeatherApp {

  public:
    WiFiClient wifi;
    HttpClient client = HttpClient(wifi, "api.weatherapi.com");

    void setup();

    void assembleUrl();

    void restartTask();

    int sendGetRequest();

    void updateTaskState();

    void suspendTaskOnError();

    static void updateWeather(void *parameter);

    WeatherApp(Weather *weather, StateApp *stateApp);
    ~WeatherApp();

  private:
    String apiUrl = "/v1/forecast.json";
    String weatherUrl;

    Weather *weather;
    StateApp *stateApp;

    TaskHandle_t weatherTask;

    bool weatherRunning = false;

    void encodeCity();
    String urlEncode(const char *str);

    void deserializeJsonResponse(String &response);

    void setTemperature(int temperature);
    void setFeelsLike(float temperature);
    void setWeatherCondition(const char *conditions);
    void setWind(float windSpeed, float windDirection);
    void setHumidity(int humidity);
    void setPressure(int pressure);
    void setPrecipitationProbability(int rainProbability, int snowProbability,
                                       int temp);
    void setMaxMinTemperatures(int maxTemperature, int minTemperature);
    void setHourlyTemperatures(float nightTemperature, float morningTemperature,
                                float afternoonTemperature, float eveningTemperature);
    void setCityAndCountryCode(const char *city, const char *countryCode);
    void setWeatherImg(const char *url);

    int getMappedImageCode(int code);

    void createTask();
};
