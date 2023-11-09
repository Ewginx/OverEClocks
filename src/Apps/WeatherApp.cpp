#include "Apps/WeatherApp.h"

static WeatherApp *instance = NULL;

WeatherApp::WeatherApp(Weather *weather) {
    this->weather = weather;
    this->setup_weather_url();
}

void WeatherApp::send_weather_request(void *parameter) {
    WeatherApp *l_pThis = (WeatherApp *)parameter;
    for (;;) {
        if (WiFi.status() == WL_CONNECTED) {
            l_pThis->client.get(l_pThis->weather_url.c_str());
            int statusCode = l_pThis->client.responseStatusCode();
            Serial.print("Status code: ");
            Serial.println(statusCode);
            if (statusCode == 200) {
                String response = l_pThis->client.responseBody();
                l_pThis->deserialize_json_response(response);
                Serial.print("Response: ");
                Serial.println(response);
                Serial.printf("Wait %i minutes", WEATHER_API_POLLING_INTERVAL_MINUTES);
                Serial.println();
            }
        }

        vTaskDelay(WEATHER_API_POLLING_INTERVAL_MILLISECONDS / portTICK_PERIOD_MS);
    }
}
void WeatherApp::setup_weather_url() {
    this->weather_url += API_KEY;
    this->weather_url += "&q=";
    this->weather_url += "city";
    this->weather_url += "&aqi=no";
    this->weather_url += "&lang=";
    this->weather_url += "ru";
}
void WeatherApp::deserialize_json_response(String &response) {
    DynamicJsonDocument doc(4096);

    DeserializationError error = deserializeJson(doc, response);
    Serial.println("Starting to deserialize");

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    JsonObject location = doc["location"];
    // const char *location_name = location["name"]; // "city"
    // const char *location_region = location["region"];            // "city"
    // const char *location_country = location["country"]; // "Russia"
    this->set_city_and_country_code(location["name"], location["country"]);
    // float location_lat = location["lat"];                        // 48.5
    // float location_lon = location["lon"];                        // 135.1
    // const char *location_tz_id = location["tz_id"];              //
    // "Asia/Vladivostok" long location_localtime_epoch = location["localtime_epoch"];
    // // 1699182987 const char *location_localtime = location["localtime"];      //
    // "2023-11-05 21:16"

    JsonObject current = doc["current"];
    // int current_temp_c = current["temp_c"]; // -8
    // int current_is_day = current["is_day"]; // 0
    this->set_temperature(current["temp_c"]);
    JsonObject current_condition = current["condition"];
    // const char *current_condition_text = current_condition["text"]; // "Ясно"
    this->set_weather_condition(current_condition["text"]);
    // const char *current_condition_icon = current_condition["icon"];
    int current_condition_code = current_condition["code"]; // 1000

    // float current_wind_kph = current["wind_kph"]; // 11.2
    // int current_wind_degree = current["wind_degree"];   // 240
    // const char *current_wind_dir = current["wind_dir"]; // "WSW"
    this->set_wind(current["wind_kph"], current["wind_dir"]);
    // int current_pressure_mb = current["pressure_mb"];   // 1029
    this->set_pressure(current["pressure_mb"]);
    // int current_humidity = current["humidity"]; // 67
    this->set_humidity(current["humidity"]);
    // int current_cloud = current["cloud"];               // 0
    // float current_feelslike_c = current["feelslike_c"]; // -12.1
    this->set_feels_like(current["feelslike_c"]);
    // int current_vis_km = current["vis_km"];             // 10
    // int current_uv = current["uv"];                     // 1
    // float current_gust_kph = current["gust_kph"];       // 16.6

    // JsonObject forecast_forecastday_0 = doc["forecast"]["forecastday"][0];
    // const char *forecast_forecastday_0_date =
    //     forecast_forecastday_0["date"]; // "2023-11-05"
    // long forecast_forecastday_0_date_epoch =
    //     forecast_forecastday_0["date_epoch"]; // 1699142400

    JsonObject forecastday = doc["forecast"]["forecastday"][0]["day"];
    // float forecastday_maxtemp_c =
    //     forecastday["maxtemp_c"]; // -5.1
    // int forecastday_mintemp_c =
    //     forecastday["mintemp_c"]; // -14
    this->set_max_min_temperatures(forecastday["maxtemp_c"],
                                   forecastday["mintemp_c"]);
    // float forecastday_avgtemp_c =
    //     forecastday["avgtemp_c"]; // -9.6
    // float forecastday_maxwind_kph =
    //     forecastday["maxwind_kph"]; // 15.1
    // int forecastday_totalsnow_cm =
    //     forecastday["totalsnow_cm"]; // 0
    // int forecastday_avghumidity =
    //     forecastday["avghumidity"]; // 84
    // int forecastday_daily_will_it_rain =
    //     forecastday["daily_will_it_rain"];
    // int forecastday_daily_chance_of_rain =
    //     forecastday["daily_chance_of_rain"];
    // int forecastday_daily_will_it_snow =
    //     forecastday["daily_will_it_snow"];
    // int forecastday_daily_chance_of_snow =
    //     forecastday["daily_chance_of_snow"];
    this->set_precipitation_probability(
        forecastday["daily_chance_of_rain"],
        forecastday["daily_chance_of_snow"]);
    // const char *forecastday_condition_text =
    //     forecastday["condition"]["text"];
    // int forecastday_condition_code =
    //     forecastday["condition"]["code"]; // 1003

    // int forecastday_uv = forecastday["uv"]; // 2

    // for (JsonObject forecast_forecastday_0_hour_item :
    //      forecast_forecastday_0["hour"].as<JsonArray>()) {

    //     const char *forecast_forecastday_0_hour_item_time =
    //         forecast_forecastday_0_hour_item["time"];
    //     float forecast_forecastday_0_hour_item_temp_c =
    //         forecast_forecastday_0_hour_item["temp_c"]; // -12.8, ...
    // }
}
void WeatherApp::set_temperature(int temperature) {
    lv_label_set_text_fmt(weather->weatherTemperatureLabel, "%d°C", temperature);
}
void WeatherApp::set_feels_like(int temperature) {
    lv_label_set_text_fmt(weather->feelsLikeLabel, "Ощущается как: %d°C", temperature);
}
void WeatherApp::set_weather_condition(const char *conditions) {
    lv_label_set_text(weather->briefingLabel, conditions);
}
void WeatherApp::set_wind(double wind_speed, const char *wind_dir) {
    lv_label_set_text_fmt(weather->windLabel, WIND_SYMBOL " %.1f км/ч " SW_WIND_SYMBOL,
                          wind_speed);
}
void WeatherApp::set_humidity(int humidity) {
    lv_label_set_text_fmt(weather->weatherHumidityLabel, HUMIDITY_SYMBOL " %d%%",
                          humidity);
}
void WeatherApp::set_city_and_country_code(const char *city, const char *country_code) {
    lv_label_set_text_fmt(weather->weatherCityLabel, "%s, %s", city, country_code);
}
void WeatherApp::set_precipitation_probability(int rain_probability, int snow_probability) {
    lv_label_set_text_fmt(weather->weatherProbabilityLabel, SNOWFLAKE_SYMBOL " %d%%",
                          snow_probability);
}
void WeatherApp::set_max_min_temperatures(int max_temp, int min_temp) {
    lv_label_set_text_fmt(weather->weatherMaxTempLabel, TEMP_UP_SYMBOL " %d°C", max_temp);
    lv_label_set_text_fmt(weather->weatherMinTempLabel, TEMP_DOWN_SYMBOL " %d°C",
                          min_temp);
}
void WeatherApp::set_pressure(int pressure) {
    lv_label_set_text_fmt(weather->weatherPressureLabel, PRESSURE_SYMBOL " %d мбар",
                          pressure);
}
void WeatherApp::create_weather_task() {
    xTaskCreatePinnedToCore(
        this->send_weather_request, /* Function to implement the task */
        "request",                  /* Name of the task */
        10000,                      /* Stack size in words */
        this,                       /* Task input parameter */
        0,                          /* Priority of the task */
        &this->Weather_Task,        /* Task handle. */
        0);
}
WeatherApp::~WeatherApp() {}