#include "WeatherApp.h"

static WeatherApp *instance = NULL;
void weather_enabled_cb(void *subscriber, lv_msg_t *msg) {
    // const bool *payload = static_cast<const bool *>(lv_msg_get_payload(msg));
    // instance->update_weather_task_state(*payload);
    instance->update_weather_task_state();
}
void weather_url_update_cb(void *subscriber, lv_msg_t *msg) {
    instance->setup_weather_url();
}
void weather_update_cb(void *subscriber, lv_msg_t *msg) { instance->update_weather(); }
WeatherApp::WeatherApp(Weather *weather, StateApp *state_app) {
    instance = this;
    this->_state_app = state_app;
    this->weather = weather;
    lv_msg_subscribe(MSG_WEATHER_UPDATE, weather_update_cb, NULL);
    lv_msg_subscribe(MSG_WEATHER_ENABLED, weather_enabled_cb, NULL);
    lv_msg_subscribe(MSG_WEATHER_CITY_CHANGED, weather_url_update_cb, NULL);
    lv_msg_subscribe(MSG_WEATHER_LANGUAGE_CHANGED, weather_url_update_cb, NULL);
}

void WeatherApp::get_weather(void *parameter) {
    WeatherApp *l_pThis = (WeatherApp *)parameter;
    for (;;) {
        if (WiFi.status() == WL_CONNECTED & l_pThis->_weather_running) {
            int i = 0;
            for (i; i < 3; i++) {
                int statusCode = l_pThis->send_weather_request();
                Serial.print("Status code: ");
                Serial.println(statusCode);
                if (statusCode == 200) {
                    String response = l_pThis->client.responseBody();
                    l_pThis->deserialize_json_response(response);
                    // Serial.print("Response: ");
                    // Serial.println(response);
                    Serial.printf("Waiting %i minutes for the next request",
                                  l_pThis->_state_app->request_period / 60000);
                    Serial.println();
                    break;
                } else {
                    Serial.printf("Try %d of 3 \n", i + 1);
                    Serial.println(l_pThis->client.responseBody());
                    vTaskDelay(300 / portTICK_PERIOD_MS);
                }
            }
            if (i == 3) {
                l_pThis->suspend_task_on_error();
            }
        }
        vTaskDelay(l_pThis->_state_app->request_period / portTICK_PERIOD_MS);
    }
}
void WeatherApp::setup_weather_url() {
    this->encode_city();
    this->_weather_url.clear();
    this->_weather_url += this->_api_url;
    this->_weather_url += "?key=";
    this->_weather_url += this->_state_app->api_key;
    this->_weather_url += "&q=";
    this->_weather_url += this->_state_app->city_encoded;
    this->_weather_url += "&aqi=no";
    this->_weather_url += "&lang=";
    this->_weather_url += this->_state_app->language;
}

int WeatherApp::send_weather_request() {
    this->client.get(this->_weather_url.c_str());
    return this->client.responseStatusCode();
}
void WeatherApp::encode_city() {
    this->_state_app->city_encoded = this->url_encode(this->_state_app->city.c_str());
}
void WeatherApp::update_weather_task_state() {

    if (this->_state_app->weather_enabled & this->_state_app->wifi_connected) {
        if (!this->_weather_running) {
            vTaskResume(this->_weather_task);
        } else {
            Serial.println("Task already resumed!");
        }
    } else {
        if (this->_weather_running) {
            this->weather->set_no_data_values();
            vTaskSuspend(this->_weather_task);
        } else {
            Serial.println("Task already suspended!");
        }
    }
    this->_weather_running =
        this->_state_app->wifi_connected ? this->_state_app->weather_enabled : false;
}
void WeatherApp::suspend_task_on_error() {
    this->_state_app->weather_enabled = false;
    this->update_weather_task_state();
    lv_msg_send(MSG_UPDATE_WEATHER_GUI, NULL);
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
    this->set_city_and_country_code(location["name"].as<const char *>(),
                                    location["country"].as<const char *>());
    // location["lat"];                        // 48.5
    // location["lon"];                        // 135.1

    JsonObject current_weather = doc["current"];
    // current_weather["is_day"]; // 0
    this->set_temperature(current_weather["temp_c"].as<int>());
    JsonObject current_condition = current_weather["condition"];
    this->set_weather_condition(current_condition["text"].as<const char *>());
    // int current_condition_code = ; // 1000
    this->set_weather_img(current_condition["icon"].as<const char *>());
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
void WeatherApp::set_weather_img(const char *url) {
    char path[24];
    if (url[35] == 'n') {
        char code[4];
        code[0] = url[41];
        code[1] = url[42];
        code[2] = url[43];
        code[3] = '\0';
        int image_code = this->get_mapped_image_code(atoi(code));
        char folder[] = "night";
        sprintf(path, "F:/icons/%s/%d.bin", folder, image_code);

    } else {
        char code[4];
        code[0] = url[39];
        code[1] = url[40];
        code[2] = url[41];
        code[3] = '\0';
        int image_code = this->get_mapped_image_code(atoi(code));
        char folder[] = "day";
        sprintf(path, "F:/icons/%s/%d.bin", folder, image_code);
    }
    lv_img_set_src(this->weather->weatherImage, path);
    // cdn.weatherapi.com/weather/64x64/day/368.png
    // cdn.weatherapi.com/weather/64x64/night/368.png
    // S:/icons/night/113.bin
}
int WeatherApp::get_mapped_image_code(int code) {
    if (code == 113 || code == 119 || code == 116 || code == 122) {
        return code;
    }
    if (code == 176 || code == 266) {
        return 176;
    }
    if (code == 143 || code == 248 || code == 260) {
        return 143;
    }
    if (code == 182 || code == 362 || code == 365) {
        return 182;
    }
    if (code == 185 || code == 317 || code == 320) {
        return 185;
    }
    if (code == 179 || code == 323 || code == 329 || code == 335 || code == 368 ||
        code == 371) {
        return 179;
    }
    if (code == 200 || code == 392) {
        return 200;
    }
    if (code == 227 || code == 230) {
        return 227;
    }
    if (code == 263 || code == 281 || code == 284) {
        return 263;
    }
    if (code == 293 || code == 299) {
        return 293;
    }
    if (code == 296 || code == 302) {
        return 296;
    }
    if (code == 305 || code == 356) {
        return 305;
    }
    if (code == 308 || code == 311 || code == 314) {
        return 308;
    }
    if (code == 326 || code == 332 || code == 338) {
        return 326;
    }
    if (code == 350) {
        return 350;
    }
    if (code == 353) {
        return 353;
    }
    if (code == 359 || code == 386) {
        return 359;
    }
    if (code == 362 || code == 365) {
        return 362;
    }
    if (code == 374 || code == 377) {
        return 374;
    }
    if (code == 389 || code == 395) {
        return 389;
    }
    return code;
}
void WeatherApp::update_weather() {
    if (this->_weather_running) {
        this->setup_weather_url();
        Serial.println("Update weather");
        vTaskSuspend(this->_weather_task);
        vTaskResume(this->_weather_task);
    }
}
String WeatherApp::url_encode(const char *str) {
    String encodedMsg = "";
    const char *hex = "0123456789ABCDEF";
    while (*str != '\0') {
        if (('a' <= *str && *str <= 'z') || ('A' <= *str && *str <= 'Z') ||
            ('0' <= *str && *str <= '9') || *str == '-' || *str == '_' || *str == '.' ||
            *str == '~') {
            encodedMsg += *str;
        } else {
            encodedMsg += '%';
            encodedMsg += hex[(unsigned char)*str >> 4];
            encodedMsg += hex[*str & 0xf];
        }
        str++;
    }
    return encodedMsg;
}
void WeatherApp::create_weather_task() {
    xTaskCreatePinnedToCore(this->get_weather,    /* Function to implement the task */
                            "request",            /* Name of the task */
                            10000,                /* Stack size in words */
                            this,                 /* Task input parameter */
                            0,                    /* Priority of the task */
                            &this->_weather_task, /* Task handle. */
                            0);
    vTaskSuspend(this->_weather_task);
}
WeatherApp::~WeatherApp() {}
