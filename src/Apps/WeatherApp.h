#pragma once
#include "Arduino.h"
#include "Config/Config.h"
#include "GUI/Weather.h"
#include <ArduinoJson.h>
#include <HttpClient.h>
#include <WiFi.h>
#include <WiFiClient.h>


class WeatherApp {
  private:
    short int port = 80;
    String weather_url =
        "/v1/forecast.json"; // ?key={API_KEY}&q={CITY}&aqi=no&lang={COUNTRY_CODE}
    Weather *weather;
    TaskHandle_t Weather_Task;
    bool url_is_ready = false;
    bool _weather_api_enabled = false;
    SemaphoreHandle_t _mutex;
  public:
    WiFiClient wifi;
    HttpClient client = HttpClient(wifi, "api.weatherapi.com", port);
    // HttpClient client = HttpClient(wifi, "jsonplaceholder.typicode.com", port);

    void setup_weather_url();
    void enable_weather(bool enable=true);
    void deserialize_json_response(String &response);
    void set_temperature(int temperature);
    void set_feels_like(double temperature);
    void set_weather_condition(const char *conditions);
    void set_wind(double wind_speed, double wind_dir);
    void set_humidity(int humidity);
    void set_pressure(int pressure);
    void set_precipitation_probability(int rain_probability, int snow_probability,
                                       int temp);
    void set_max_min_temperatures(int max_temp, int min_temp);
    void set_daily_temperatures(double night_temp, double morning_temp,
                                double afternoon_temp, double evening_temp);
    // void set_visibility(char *visibility);
    // void set_uv(char *uv);
    void set_city_and_country_code(const char *city, const char *country_code);
    // void set_hour_forecast();
    void set_weather_img(int code);
    void create_weather_task();
    static void send_weather_request(void *parameter);

    WeatherApp(Weather *weather, SemaphoreHandle_t &mutex);
    ~WeatherApp();
};
