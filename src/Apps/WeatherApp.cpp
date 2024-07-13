#include "WeatherApp.h"

static WeatherApp *instance = NULL;

void updateTaskCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    instance->updateTaskState();
}
void updateWeatherUrlCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    instance->assembleUrl();
}
void restartTaskCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    instance->restartTask();
}

void WeatherApp::restartTask() {
    if (weatherRunning) {
        this->assembleUrl();
        vTaskSuspend(this->weatherTask);
        vTaskResume(this->weatherTask);
    }
}

WeatherApp::WeatherApp(Weather *weather, StateApp *stateApp) {
    instance = this;
    this->stateApp = stateApp;
    this->weather = weather;
    lv_msg_subscribe(MSG_WEATHER_UPDATE, restartTaskCallbackWrapper, NULL);
    lv_msg_subscribe(MSG_UPDATE_WEATHER_TASK, updateTaskCallbackWrapper, NULL);
    lv_msg_subscribe(MSG_WEATHER_CITY_CHANGED, updateWeatherUrlCallbackWrapper, NULL);
    lv_msg_subscribe(MSG_WEATHER_LANGUAGE_CHANGED, updateWeatherUrlCallbackWrapper, NULL);
}

void WeatherApp::setup() {
    this->assembleUrl();
    this->createTask();
}

void WeatherApp::assembleUrl() {
    this->encodeCity();
    weatherUrl.clear();
    weatherUrl += apiUrl;
    weatherUrl += "?key=";
    weatherUrl += stateApp->weather_state->api_key;
    weatherUrl += "&q=";
    weatherUrl += stateApp->weather_state->city_encoded;
    weatherUrl += "&aqi=no";
    weatherUrl += "&lang=";
    weatherUrl += stateApp->weather_state->language;
}

void WeatherApp::createTask() {
    xTaskCreatePinnedToCore(this->updateWeather, /* Function to implement the task */
                            "request",           /* Name of the task */
                            4096, /* Stack size in bytes */
                            NULL,                           /* Task input parameter */
                            0,                              /* Priority of the task */
                            &this->weatherTask,             /* Task handle. */
                            0);
    vTaskSuspend(this->weatherTask);
}

void WeatherApp::encodeCity() {
    stateApp->weather_state->city_encoded =
        this->urlEncode(stateApp->weather_state->city.c_str());
}

String WeatherApp::urlEncode(const char *str) {
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

void WeatherApp::updateWeather(void *parameter) {
    for (;;) {
        if (WiFi.status() == WL_CONNECTED & instance->weatherRunning) {
            uint8_t retry = 0;
            for (retry; retry < 3; retry++) {
                int statusCode = instance->sendGetRequest();
                Serial.print("Status code: ");
                Serial.println(statusCode);
                if (statusCode == 200) {
                    String response = instance->client.responseBody();
                    instance->deserializeJsonResponse(response);
                    Serial.printf("Waiting %i minutes for the next request\n",
                                  instance->stateApp->weather_state->request_period /
                                      60000);
                    break;
                } else {
                    Serial.printf("Try %d of 3 \n", retry++);
                    Serial.println(instance->weatherUrl);
                    Serial.println(instance->client.responseBody());
                    vTaskDelay(300 / portTICK_PERIOD_MS);
                }
            }
            if (retry == 3) {
                instance->suspendTaskOnError();
            }
        }
        vTaskDelay(instance->stateApp->weather_state->request_period /
                   portTICK_PERIOD_MS);
    }
}

int WeatherApp::sendGetRequest() {
    this->client.get(weatherUrl.c_str());
    return this->client.responseStatusCode();
}

void WeatherApp::deserializeJsonResponse(String &response) {
    DynamicJsonDocument doc(1536);
    StaticJsonDocument<368> filter;
    filter["location"]["name"] = true;
    filter["location"]["country"] = true;
    filter["current"]["temp_c"] = true;
    filter["current"]["wind_kph"] = true;
    filter["current"]["wind_degree"] = true;
    filter["current"]["pressure_mb"] = true;
    filter["current"]["humidity"] = true;
    filter["current"]["feelslike_c"] = true;
    filter["current"]["condition"] = true;
    filter["current"]["condition"]["text"] = true;
    filter["current"]["condition"]["icon"] = true;
    filter["forecast"]["forecastday"][0]["day"]["maxtemp_c"] = true;
    filter["forecast"]["forecastday"][0]["day"]["mintemp_c"] = true;
    filter["forecast"]["forecastday"][0]["day"]["daily_chance_of_rain"] = true;
    filter["forecast"]["forecastday"][0]["day"]["daily_chance_of_snow"] = true;
    filter["forecast"]["forecastday"][0]["hour"][0]["temp_c"] = true;

    DeserializationError error =
        deserializeJson(doc, response, DeserializationOption::Filter(filter));

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    Serial.println("Updating weather");

    JsonObject location = doc["location"];
    this->setCityAndCountryCode(location["name"].as<const char *>(),
                                location["country"].as<const char *>());

    JsonObject current_weather = doc["current"];
    this->setTemperature(current_weather["temp_c"].as<int>());
    JsonObject current_condition = current_weather["condition"];
    this->setWeatherCondition(current_condition["text"].as<const char *>());
    this->setWeatherImg(current_condition["icon"].as<const char *>());
    this->setWind(current_weather["wind_kph"].as<int>(),
                  current_weather["wind_degree"].as<float>());
    this->setPressure(current_weather["pressure_mb"].as<int>());
    this->setHumidity(current_weather["humidity"].as<int>());
    this->setFeelsLike(current_weather["feelslike_c"].as<float>());

    JsonObject forecastday = doc["forecast"]["forecastday"][0]["day"];
    this->setMaxMinTemperatures(forecastday["maxtemp_c"].as<int>(),
                                forecastday["mintemp_c"].as<int>());
    this->setPrecipitationProbability(forecastday["daily_chance_of_rain"].as<int>(),
                                      forecastday["daily_chance_of_snow"].as<int>(),
                                      current_weather["temp_c"].as<int>());
    this->setHourlyTemperatures(
        doc["forecast"]["forecastday"][0]["hour"][2]["temp_c"].as<float>(),
        doc["forecast"]["forecastday"][0]["hour"][7]["temp_c"].as<float>(),
        doc["forecast"]["forecastday"][0]["hour"][13]["temp_c"].as<float>(),
        doc["forecast"]["forecastday"][0]["hour"][19]["temp_c"].as<float>());
}

void WeatherApp::suspendTaskOnError() {
    stateApp->weather_state->weather_enabled = false;
    this->updateTaskState();
    lv_msg_send(MSG_UPDATE_WEATHER_GUI, NULL);
}

void WeatherApp::updateTaskState() {
    if (stateApp->weather_state->weather_enabled & stateApp->wifi_state->wifi_connected) {
        if (!weatherRunning) {
            vTaskResume(this->weatherTask);
        } else {
            Serial.println("Task already resumed!");
        }
    } else {
        if (weatherRunning) {
            weather->set_no_data_values();
            vTaskSuspend(this->weatherTask);
        } else {
            Serial.println("Task already suspended!");
        }
    }
    weatherRunning = stateApp->wifi_state->wifi_connected
                         ? stateApp->weather_state->weather_enabled
                         : false;
}

void WeatherApp::setTemperature(int temperature) {
    lv_label_set_text_fmt(weather->weatherTemperatureLabel, "%d°C", temperature);
}

void WeatherApp::setFeelsLike(float temperature) {
    lv_label_set_text_fmt(weather->feelsLikeLabel, "%s %0.1f°C",
                          weather_translation[feels_like], temperature);
}

void WeatherApp::setWeatherCondition(const char *conditions) {
    lv_label_set_text(weather->briefingLabel, conditions);
}

void WeatherApp::setWind(float wind_speed, float wind_dir) {
    char wind_symbol[4];
    if (337.5f < wind_dir | wind_dir < 22.5f) {
        strcpy(wind_symbol, N_WIND_SYMBOL);
    } else if (292.5f <= wind_dir & wind_dir <= 337.5f) {
        strcpy(wind_symbol, NW_WIND_SYMBOL);
    } else if (247.5f < wind_dir & wind_dir < 292.5f) {
        strcpy(wind_symbol, W_WIND_SYMBOL);
    } else if (202.5f <= wind_dir & wind_dir <= 247.5f) {
        strcpy(wind_symbol, SW_WIND_SYMBOL);
    } else if (157.5f < wind_dir & wind_dir < 202.5f) {
        strcpy(wind_symbol, S_WIND_SYMBOL);
    } else if (112.5f <= wind_dir & wind_dir <= 157.5f) {
        strcpy(wind_symbol, SE_WIND_SYMBOL);
    } else if (67.5f < wind_dir & wind_dir < 112.5f) {
        strcpy(wind_symbol, E_WIND_SYMBOL);
    } else if (22.5f <= wind_dir & wind_dir <= 67.5f) {
        strcpy(wind_symbol, NE_WIND_SYMBOL);
    }
    lv_label_set_text_fmt(weather->windLabel, WIND_SYMBOL " %.1f %s %s", wind_speed,
                          weather_translation[wind_speed_uom], wind_symbol);
}

void WeatherApp::setHumidity(int humidity) {
    lv_label_set_text_fmt(weather->weatherHumidityLabel, HUMIDITY_SYMBOL " %d%%",
                          humidity);
}

void WeatherApp::setCityAndCountryCode(const char *city, const char *country_code) {
    lv_label_set_text_fmt(weather->weatherCityLabel, "%s, %s", city, country_code);
}

void WeatherApp::setPrecipitationProbability(int rain_probability, int snow_probability,
                                             int temperature) {
    if (temperature < 0) {
        lv_label_set_text_fmt(weather->weatherProbabilityLabel, SNOWFLAKE_SYMBOL " %d%%",
                              snow_probability);
    } else {
        lv_label_set_text_fmt(weather->weatherProbabilityLabel, WATER_DROP_SYMBOL " %d%%",
                              rain_probability);
    }
}

void WeatherApp::setMaxMinTemperatures(int maxTemperature, int minTemperature) {
    lv_label_set_text_fmt(weather->weatherMaxTempLabel, TEMP_UP_SYMBOL " %d°C",
                          maxTemperature);
    lv_label_set_text_fmt(weather->weatherMinTempLabel, TEMP_DOWN_SYMBOL " %d°C",
                          minTemperature);
}

void WeatherApp::setHourlyTemperatures(float night_temp, float morning_temp,
                                       float afternoon_temp, float evening_temp) {
    lv_label_set_text_fmt(weather->weatherFirstTempLabel, "%0.1f°C", night_temp);
    lv_label_set_text_fmt(weather->weatherSecondTempLabel, "%0.1f°C", morning_temp);

    lv_label_set_text_fmt(weather->weatherThirdTempLabel, "%0.1f°C", afternoon_temp);

    lv_label_set_text_fmt(weather->weatherFourthTempLabel, "%0.1f°C", evening_temp);
}

void WeatherApp::setPressure(int pressure) {
    lv_label_set_text_fmt(weather->weatherPressureLabel, PRESSURE_SYMBOL " %d %s",
                          pressure, weather_translation[pressure_uom]);
}

void WeatherApp::setWeatherImg(const char *url) {
    Serial.println("url");
    Serial.println(url);
    char path[26];
    if (url[35] == 'n') {
        char code[4];
        code[0] = url[41];
        code[1] = url[42];
        code[2] = url[43];
        code[3] = '\0';
        int image_code = this->getMappedImageCode(atoi(code));
        char folder[] = "night";
        sprintf(path, "F:/weather/%s/%d.bin", folder, image_code);

    } else {
        char code[4];
        code[0] = url[39];
        code[1] = url[40];
        code[2] = url[41];
        code[3] = '\0';
        int image_code = this->getMappedImageCode(atoi(code));
        char folder[] = "day";
        sprintf(path, "F:/weather/%s/%d.bin", folder, image_code);
    }
    lv_img_set_src(weather->weatherImage, path);
    // //cdn.weatherapi.com/weather/64x64/day/368.png
    // //cdn.weatherapi.com/weather/64x64/night/368.png
    // F:/weather/night/113.bin
}

int WeatherApp::getMappedImageCode(int code) {
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
    if (code == 263 || code == 284) {
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
    if (code == 374 || code == 377) {
        return 374;
    }
    if (code == 389 || code == 395) {
        return 389;
    }
    return code;
}

WeatherApp::~WeatherApp() {}
