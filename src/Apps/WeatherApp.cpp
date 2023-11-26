#include "Apps/WeatherApp.h"

static WeatherApp *instance = NULL;
void weather_enabled_cb(void *subscriber, lv_msg_t *msg) {
    Serial.println("Get msg in WeatherApp with payload:");
    const bool *payload = static_cast<const bool *>(lv_msg_get_payload(msg));
    Serial.println(*payload);
}
WeatherApp::WeatherApp(Weather *weather, SemaphoreHandle_t &mutex) {
    instance = this;
    this->_mutex = mutex;
    this->weather = weather;
    this->setup_weather_url();
    lv_msg_subscribe(MSG_WEATHER_ENABLED, weather_enabled_cb, NULL);
}

void WeatherApp::send_weather_request(void *parameter) {
    WeatherApp *l_pThis = (WeatherApp *)parameter;
    for (;;) {
        if (WiFi.status() == WL_CONNECTED & l_pThis->url_is_ready) {
            l_pThis->client.get(l_pThis->weather_url.c_str());
            int statusCode = l_pThis->client.responseStatusCode();
            Serial.print("Status code: ");
            Serial.println(statusCode);
            if (statusCode == 200) {
                String response = l_pThis->client.responseBody();
                xSemaphoreTake(l_pThis->_mutex, portMAX_DELAY);
                l_pThis->deserialize_json_response(response);
                xSemaphoreGive(l_pThis->_mutex);
                Serial.print("Response: ");
                Serial.println(response);
                Serial.printf("Waiting %i minutes for the next request",
                              WEATHER_API_POLLING_INTERVAL_MINUTES);
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
    this->url_is_ready = true;
}
void WeatherApp::enable_weather(bool enable) { this->_weather_api_enabled = enable; }
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
    this->set_city_and_country_code(location["name"].as<const char *>(),
                                    location["country"].as<const char *>());
    // location["lat"];                        // 48.5
    // location["lon"];                        // 135.1

    JsonObject current_weather = doc["current"];
    // current_weather["is_day"]; // 0
    this->set_temperature(current_weather["temp_c"].as<int>());
    JsonObject current_condition = current_weather["condition"];
    this->set_weather_condition(current_condition["text"].as<const char *>());
    // int current_condition_code = current_condition["code"].as<int>(); // 1000

    // current_weather["wind_dir"];  // SW
    this->set_wind(current_weather["wind_kph"].as<int>(),
                   current_weather["wind_degree"].as<double>());
    this->set_pressure(current_weather["pressure_mb"].as<int>());
    this->set_humidity(current_weather["humidity"].as<int>());
    // current_weather["cloud"];  // 0
    this->set_feels_like(current_weather["feelslike_c"].as<double>());
    // current_weather["vis_km"];  // 10
    // current_weather["uv"];  // 1
    // current_weather["gust_kph"];   // 16.6

    JsonObject forecastday = doc["forecast"]["forecastday"][0]["day"];
    this->set_max_min_temperatures(forecastday["maxtemp_c"].as<int>(),
                                   forecastday["mintemp_c"].as<int>());
    //  forecastday["avgtemp_c"]; // -9.6
    //  forecastday["maxwind_kph"]; // 15.1
    //  forecastday["totalsnow_cm"]; // 0
    //  forecastday["avghumidity"]; // 84
    //  forecastday["daily_will_it_rain"];
    //  forecastday["daily_chance_of_rain"];
    //  forecastday["daily_will_it_snow"];
    //  forecastday["daily_chance_of_snow"];
    this->set_precipitation_probability(forecastday["daily_chance_of_rain"].as<int>(),
                                        forecastday["daily_chance_of_snow"].as<int>(),
                                        current_weather["temp_c"].as<int>());
    // forecastday["condition"]["code"]; // 1003
    // forecastday["uv"]; // 2
    this->set_daily_temperatures(
        doc["forecast"]["forecastday"][0]["hour"][2]["temp_c"].as<double>(),
        doc["forecast"]["forecastday"][0]["hour"][7]["temp_c"].as<double>(),
        doc["forecast"]["forecastday"][0]["hour"][13]["temp_c"].as<double>(),
        doc["forecast"]["forecastday"][0]["hour"][19]["temp_c"].as<double>());
}
void WeatherApp::set_temperature(int temperature) {
    lv_label_set_text_fmt(weather->weatherTemperatureLabel, "%d°C", temperature);
}
void WeatherApp::set_feels_like(double temperature) {
    lv_label_set_text_fmt(weather->feelsLikeLabel, "%s %0.1f°C",
                          weather_translation[feels_like], temperature);
}
void WeatherApp::set_weather_condition(const char *conditions) {
    lv_label_set_text(weather->briefingLabel, conditions);
}
void WeatherApp::set_wind(double wind_speed, double wind_dir) {
    char wind_symbol[4];
    if (337.5 < wind_dir | wind_dir < 22.5) {
        strcpy(wind_symbol, N_WIND_SYMBOL);
    } else if (292.5 <= wind_dir & wind_dir <= 337.5) {
        strcpy(wind_symbol, NW_WIND_SYMBOL);
    } else if (247.5 < wind_dir & wind_dir < 292.5) {
        strcpy(wind_symbol, W_WIND_SYMBOL);
    } else if (202.5 <= wind_dir & wind_dir <= 247.5) {
        strcpy(wind_symbol, SW_WIND_SYMBOL);
    } else if (157.5 < wind_dir & wind_dir < 202.5) {
        strcpy(wind_symbol, S_WIND_SYMBOL);
    } else if (112.5 <= wind_dir & wind_dir <= 157.5) {
        strcpy(wind_symbol, SE_WIND_SYMBOL);
    } else if (67.5 < wind_dir & wind_dir < 112.5) {
        strcpy(wind_symbol, E_WIND_SYMBOL);
    } else if (22.5 <= wind_dir & wind_dir <= 67.5) {
        strcpy(wind_symbol, NE_WIND_SYMBOL);
    }
    lv_label_set_text_fmt(weather->windLabel, WIND_SYMBOL " %.1f %s %s", wind_speed,
                          weather_translation[wind_speed_uom], wind_symbol);
}
void WeatherApp::set_humidity(int humidity) {
    lv_label_set_text_fmt(weather->weatherHumidityLabel, HUMIDITY_SYMBOL " %d%%",
                          humidity);
}
void WeatherApp::set_city_and_country_code(const char *city, const char *country_code) {
    lv_label_set_text_fmt(weather->weatherCityLabel, "%s, %s", city, country_code);
}
void WeatherApp::set_precipitation_probability(int rain_probability, int snow_probability,
                                               int temp) {
    if (temp < 0) {
        lv_label_set_text_fmt(weather->weatherProbabilityLabel, SNOWFLAKE_SYMBOL " %d%%",
                              snow_probability);
    } else {
        lv_label_set_text_fmt(weather->weatherProbabilityLabel, WATER_DROP_SYMBOL " %d%%",
                              rain_probability);
    }
}
void WeatherApp::set_max_min_temperatures(int max_temp, int min_temp) {
    lv_label_set_text_fmt(weather->weatherMaxTempLabel, TEMP_UP_SYMBOL " %d°C", max_temp);
    lv_label_set_text_fmt(weather->weatherMinTempLabel, TEMP_DOWN_SYMBOL " %d°C",
                          min_temp);
}
void WeatherApp::set_daily_temperatures(double night_temp, double morning_temp,
                                        double afternoon_temp, double evening_temp) {
    lv_label_set_text_fmt(weather->weatherFirstTempLabel, "%0.1f°C", night_temp);
    lv_label_set_text_fmt(weather->weatherSecondTempLabel, "%0.1f°C", morning_temp);

    lv_label_set_text_fmt(weather->weatherThirdTempLabel, "%0.1f°C", afternoon_temp);

    lv_label_set_text_fmt(weather->weatherFourthTempLabel, "%0.1f°C", evening_temp);
}
void WeatherApp::set_pressure(int pressure) {
    lv_label_set_text_fmt(weather->weatherPressureLabel, PRESSURE_SYMBOL " %d %s",
                          pressure, weather_translation[pressure_uom]);
}
void WeatherApp::set_weather_img(int code) {
    if (code == 1000)
        lv_img_set_src(weather->weatherImage, &img_day_clear_png);
    else {
        lv_img_set_src(weather->weatherImage, &img_day_rain_png);
    }
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