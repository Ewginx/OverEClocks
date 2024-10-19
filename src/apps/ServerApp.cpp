#include "ServerApp.h"

static ServerApp *instance = NULL;

extern "C" void websocketTimerCallbackWrapper(lv_timer_t *timer) {
    instance->websocketTimerCallback(timer);
}

void ServerApp::websocketTimerCallback(lv_timer_t *timer) {
    this->websocket.textAll(this->getInfoForWS());
    this->websocket.cleanupClients();
}

String ServerApp::getInfoForWS() {
    String sensors_readings;
    DynamicJsonDocument doc(128);
    doc["temperature"] = this->stateApp->microclimateState->indoorTemperature;
    doc["humidity"] = this->stateApp->microclimateState->indoorHumidity;
    doc["lx"] = this->brightnessApp->getLightLevel();
    doc["battery_level"] = this->stateApp->batteryState->batteryLevel;
    doc["battery_voltage"] = this->stateApp->batteryState->batteryVoltage;
    doc["max_free_block"] = ESP.getMaxAllocHeap() / 1024;
    doc["free_heap"] = ESP.getFreeHeap() / 1024;
    doc["used_space"] = ((unsigned int)LittleFS.usedBytes()) / 1024;
    serializeJson(doc, sensors_readings);
    return sensors_readings;
}

void onEventWrapper(AsyncWebSocket *server, AsyncWebSocketClient *client,
                    AwsEventType type, void *arg, uint8_t *data, size_t len) {
    instance->onEvent(server, client, type, arg, data, len);
}

void ServerApp::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
                        AwsEventType type, void *arg, uint8_t *data, size_t len) {
    switch (type) {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(),
                      client->remoteIP().toString().c_str());
        if (this->_websocketNotifyTimer == NULL) {
            this->_websocketNotifyTimer =
                lv_timer_create(websocketTimerCallbackWrapper, 3000, NULL);
        }
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        if (this->_websocketNotifyTimer != NULL & this->websocket.count() == 0) {
            lv_timer_del(this->_websocketNotifyTimer);
            this->_websocketNotifyTimer = NULL;
        }
        break;
    case WS_EVT_DATA:
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

ServerApp::ServerApp(StateApp *stateApp, BrightnessApp *brightnessApp)
    : server(port), websocket("/ws") {

    instance = this;

    this->stateApp = stateApp;
    this->brightnessApp = brightnessApp;
}

void ServerApp::setup() {
    this->setupIndexHandler();
    this->setupRedirectHandlers();

    server.on("/settings", HTTP_GET,
              std::bind(&ServerApp::getSettings, this, std::placeholders::_1));

    this->setupSettingsHandlers();

    this->setupSetTimeHandler();
    this->setupFirmwareUpdateHandler();
    this->setupFilesystemUpdateHandler();

    this->setupWeatherImagesUploadHandlers();
    this->setupGIFUploadHandler();
    this->setupWebUiUploadHandler();
    this->setupAnalogClockImagesUploadHandler();

    server.serveStatic("/", LittleFS, "/").setCacheControl("max-age=604800");

    websocket.onEvent(onEventWrapper);
    server.addHandler(&websocket);

    Update.onProgress([](size_t current, size_t final) {
        Serial.printf("Progress: %u%%\n", (current * 100) / final);
    });

    server.begin();
}

void ServerApp::setupIndexHandler() {
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if (!request->authenticate(this->stateApp->wifiState->apLogin.c_str(),
                                   this->stateApp->wifiState->apPassword.c_str()))
            return request->requestAuthentication();
        AsyncWebServerResponse *response =
            request->beginResponse(LittleFS, "/index.html.gz", "text/html");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });
}

void ServerApp::setupRedirectHandlers() {
    server.on("/wifi", HTTP_GET,
              [](AsyncWebServerRequest *request) { request->redirect("/"); });
    server.on("/weather", HTTP_GET,
              [](AsyncWebServerRequest *request) { request->redirect("/"); });
    server.on("/brightness", HTTP_GET,
              [](AsyncWebServerRequest *request) { request->redirect("/"); });
    server.on("/theme", HTTP_GET,
              [](AsyncWebServerRequest *request) { request->redirect("/"); });
    server.on("/time", HTTP_GET,
              [](AsyncWebServerRequest *request) { request->redirect("/"); });
    server.on("/debug", HTTP_GET,
              [](AsyncWebServerRequest *request) { request->redirect("/"); });
    server.on("/alarm_clock", HTTP_GET,
              [](AsyncWebServerRequest *request) { request->redirect("/"); });
    server.on("/rgb", HTTP_GET,
              [](AsyncWebServerRequest *request) { request->redirect("/"); });
    server.on("/sound", HTTP_GET,
              [](AsyncWebServerRequest *request) { request->redirect("/"); });
    server.on("/ota", HTTP_GET,
              [](AsyncWebServerRequest *request) { request->redirect("/"); });
}

void ServerApp::getSettings(AsyncWebServerRequest *request) {
    Serial.println("Request on settings");
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument doc(1024);
    doc["ssid"] = this->stateApp->wifiState->ssid.c_str();
    doc["password"] = this->stateApp->wifiState->password.c_str();
    doc["ip_address"] = this->stateApp->wifiState->ipAddress.c_str();
    doc["gateway"] = this->stateApp->wifiState->gatewayAddress.c_str();
    doc["ap_login"] = this->stateApp->wifiState->apLogin.c_str();
    doc["ap_password"] = this->stateApp->wifiState->apPassword.c_str();

    doc["auto_brightness"] = this->stateApp->displayState->autoBrightness;
    doc["auto_theme_change"] = this->stateApp->displayState->autoThemeChange;
    doc["threshold"] = this->stateApp->displayState->threshold;
    doc["brightness_level"] = this->stateApp->displayState->brightnessLevel;

    doc["timezone_posix"] = this->stateApp->timeState->timezonePosix.c_str();
    doc["digital_main_screen"] = this->stateApp->displayState->mainScreenIsDigital;

    doc["weather_enabled"] = this->stateApp->weatherState->weatherEnabled;
    doc["api_key"] = this->stateApp->weatherState->apiKey.c_str();
    doc["city"] = this->stateApp->weatherState->city.c_str();
    doc["language"] = this->stateApp->weatherState->language.c_str();
    doc["request_period"] = this->stateApp->weatherState->requestPeriod;

    doc["dark_theme_enabled"] = this->stateApp->themeState->darkThemeEnabled;
    doc["light_primary_color"] = this->stateApp->themeState->lightPrimaryColor;
    doc["light_second_color"] = this->stateApp->themeState->lightSecondColor;
    doc["light_screen_color"] = this->stateApp->themeState->lightScreenColor;
    doc["light_card_color"] = this->stateApp->themeState->lightCardColor;
    doc["light_text_color"] = this->stateApp->themeState->lightTextColor;
    doc["light_grey_color"] = this->stateApp->themeState->lightGreyColor;

    doc["dark_primary_color"] = this->stateApp->themeState->darkPrimaryColor;
    doc["dark_second_color"] = this->stateApp->themeState->darkSecondColor;
    doc["dark_screen_color"] = this->stateApp->themeState->darkScreenColor;
    doc["dark_card_color"] = this->stateApp->themeState->darkCardColor;
    doc["dark_text_color"] = this->stateApp->themeState->darkTextColor;
    doc["dark_grey_color"] = this->stateApp->themeState->darkGreyColor;

    doc["weekdays_time"] = this->stateApp->alarmState->weekdaysTime.c_str();
    doc["weekends_time"] = this->stateApp->alarmState->weekendsTime.c_str();
    doc["one_off_time"] = this->stateApp->alarmState->oneOffTime.c_str();
    doc["weekdays_enabled"] = this->stateApp->alarmState->weekdaysSwitchEnabled;
    doc["weekends_enabled"] = this->stateApp->alarmState->weekendsSwitchEnabled;
    doc["one_off_enabled"] = this->stateApp->alarmState->oneOffSwitchEnabled;

    doc["fs_space"] = ((unsigned int)LittleFS.totalBytes()) / 1024;

    doc["rgb_enabled"] = this->stateApp->rgbState->enabled;
    doc["rgb_mode"] = this->stateApp->rgbState->effect;
    doc["first_rgb_color"] = this->stateApp->rgbState->firstRgbColor;
    doc["second_rgb_color"] = this->stateApp->rgbState->secondRgbColor;
    doc["third_rgb_color"] = this->stateApp->rgbState->thirdRgbColor;
    doc["rgb_delay"] = this->stateApp->rgbState->delay;
    doc["rgb_brightness"] = this->stateApp->rgbState->brightness;
    doc["rgb_night"] = this->stateApp->rgbState->turnOffAtNight;

    doc["sound_on"] = this->stateApp->soundState->soundOn;
    doc["ee_sound_on"] = this->stateApp->soundState->eeSoundOn;
    doc["plug_sound_on"] = this->stateApp->soundState->plugInSoundOn;
    doc["volume_level"] = this->stateApp->soundState->volumeLevel;
    doc["alarm_track"] = this->stateApp->soundState->alarmTrackNumber;
    doc["ee_track"] = this->stateApp->soundState->eeTrackNumber;
    doc["plug_track"] = this->stateApp->soundState->plugInTrackNumber;
    doc["enable_player_usb"] = this->stateApp->soundState->enablePlayerUsb;

    serializeJson(doc, *response);
    request->send(response);
}

void ServerApp::setupSettingsHandlers() {
    AsyncCallbackJsonWebHandler *time_settings_handler = new AsyncCallbackJsonWebHandler(
        "/settings/time", [this](AsyncWebServerRequest *request, JsonVariant &json) {
            this->saveTimeSettings(json);
            request->send(200);
        });
    server.addHandler(time_settings_handler);

    AsyncCallbackJsonWebHandler *weather_settings_handler =
        new AsyncCallbackJsonWebHandler(
            "/settings/weather",
            [this](AsyncWebServerRequest *request, JsonVariant &json) {
                this->saveWeatherSettings(json);
                request->send(200);
            });
    server.addHandler(weather_settings_handler);

    AsyncCallbackJsonWebHandler *theme_settings_handler = new AsyncCallbackJsonWebHandler(
        "/settings/theme", [this](AsyncWebServerRequest *request, JsonVariant &json) {
            this->saveThemeSettings(json);
            request->send(200);
        });
    server.addHandler(theme_settings_handler);

    AsyncCallbackJsonWebHandler *brightness_settings_handler =
        new AsyncCallbackJsonWebHandler(
            "/settings/brightness",
            [this](AsyncWebServerRequest *request, JsonVariant &json) {
                this->saveBrightnessSettings(json);
                request->send(200);
            });
    server.addHandler(brightness_settings_handler);

    AsyncCallbackJsonWebHandler *wifi_settings_handler = new AsyncCallbackJsonWebHandler(
        "/settings/wifi", [this](AsyncWebServerRequest *request, JsonVariant &json) {
            this->saveWiFiSettings(json);
            request->send(200);
        });
    server.addHandler(wifi_settings_handler);

    AsyncCallbackJsonWebHandler *alarm_clock_settings_handler =
        new AsyncCallbackJsonWebHandler(
            "/settings/alarm_clock",
            [this](AsyncWebServerRequest *request, JsonVariant &json) {
                this->saveAlarmClockSettings(json);
                request->send(200);
            });
    server.addHandler(alarm_clock_settings_handler);

    AsyncCallbackJsonWebHandler *rgb_settings_handler = new AsyncCallbackJsonWebHandler(
        "/settings/rgb", [this](AsyncWebServerRequest *request, JsonVariant &json) {
            this->saveRGBSettings(json);
            lv_msg_send(MSG_RGB_STATE_CHANGED, NULL);
            request->send(200);
        });
    server.addHandler(rgb_settings_handler);

    AsyncCallbackJsonWebHandler *save_sound_settings_handler =
        new AsyncCallbackJsonWebHandler(
            "/settings/sound", [this](AsyncWebServerRequest *request, JsonVariant &json) {
                this->saveSoundSettings(json);
                lv_msg_send(MSG_HANDLE_PLAYER_USB, NULL);
                request->send(200);
            });
    server.addHandler(save_sound_settings_handler);
}

void ServerApp::saveTimeSettings(JsonVariant &json) {
    JsonObject &&time_json = json.as<JsonObject>();
    this->stateApp->timeState->saveTimezone(
        time_json["timezone_posix"].as<const char *>());
    this->stateApp->displayState->saveMainScreenIsDigital(
        time_json["digital_main_screen"].as<bool>());
    lv_msg_send(MSG_UPDATE_TZ, NULL);
}

void ServerApp::saveWeatherSettings(JsonVariant &json) {
    JsonObject &&weather_json = json.as<JsonObject>();
    this->stateApp->weatherState->saveWeatherEnabled(
        weather_json["weather_enabled"].as<bool>());
    this->stateApp->weatherState->saveApiKey(
        weather_json["api_key"].as<const char *>());
    this->stateApp->weatherState->saveCity(weather_json["city"].as<const char *>());
    this->stateApp->weatherState->saveLanguage(
        weather_json["language"].as<const char *>());
    this->stateApp->weatherState->saveRequestPeriod(
        weather_json["request_period"].as<int>());
    lv_msg_send(MSG_UPDATE_WEATHER_GUI, NULL);
    lv_msg_send(
        MSG_UPDATE_WEATHER_TASK,
        static_cast<const void *>(&this->stateApp->weatherState->weatherEnabled));
    if (this->stateApp->weatherState->weatherEnabled) {
        lv_msg_send(MSG_WEATHER_UPDATE, NULL);
    }
}

void ServerApp::saveThemeSettings(JsonVariant &json) {
    JsonObject &&theme_json = json.as<JsonObject>();
    this->stateApp->themeState->saveDarkThemeEnabled(
        theme_json["dark_theme_enabled"].as<bool>());
    ThemeStruct light_theme;
    light_theme.primaryColor = theme_json["light_primary_color"].as<int>();
    light_theme.secondColor = theme_json["light_second_color"].as<int>();
    light_theme.screenColor = theme_json["light_screen_color"].as<int>();
    light_theme.textColor = theme_json["light_text_color"].as<int>();
    light_theme.cardColor = theme_json["light_card_color"].as<int>();
    light_theme.greyColor = theme_json["light_grey_color"].as<int>();
    this->stateApp->themeState->saveLightColors(light_theme);

    ThemeStruct dark_theme;
    dark_theme.primaryColor = theme_json["dark_primary_color"].as<int>();
    dark_theme.secondColor = theme_json["dark_second_color"].as<int>();
    dark_theme.screenColor = theme_json["dark_screen_color"].as<int>();
    dark_theme.textColor = theme_json["dark_text_color"].as<int>();
    dark_theme.cardColor = theme_json["dark_card_color"].as<int>();
    dark_theme.greyColor = theme_json["dark_grey_color"].as<int>();
    this->stateApp->themeState->saveDarkColors(dark_theme);
    lv_msg_send(MSG_CHANGE_THEME, static_cast<const void *>(
                                      &this->stateApp->themeState->darkThemeEnabled));
}

void ServerApp::saveBrightnessSettings(JsonVariant &json) {
    JsonObject &&brightness_json = json.as<JsonObject>();
    this->stateApp->displayState->saveAutoBrightnessEnabled(
        brightness_json["auto_brightness"].as<bool>());
    this->stateApp->displayState->saveAutoThemeChange(
        brightness_json["auto_theme_change"].as<bool>());
    this->stateApp->displayState->saveAutoBrightnessThreshold(
        brightness_json["threshold"].as<int>());
    this->stateApp->displayState->saveBrightnessLevel(
        brightness_json["brightness_level"].as<int>());
    lv_msg_send(MSG_BRIGHTNESS_CHANGED, NULL);
    lv_msg_send(MSG_AUTO_BRIGHTNESS, NULL);
}

void ServerApp::saveWiFiSettings(JsonVariant &json) {
    JsonObject &&wifi_json = json.as<JsonObject>();
    this->stateApp->wifiState->saveSsid(wifi_json["ssid"].as<const char *>());
    this->stateApp->wifiState->savePassword(wifi_json["password"].as<const char *>());
    this->stateApp->wifiState->saveIpAndGatewayAddresses(
        wifi_json["ip_address"].as<const char *>(),
        wifi_json["gateway"].as<const char *>());
    this->stateApp->wifiState->saveApLogin(wifi_json["ap_login"].as<const char *>());
    this->stateApp->wifiState->saveApPassword(
        wifi_json["ap_password"].as<const char *>());
}

void ServerApp::saveAlarmClockSettings(JsonVariant &json) {
    JsonObject &&alarm_clock_json = json.as<JsonObject>();
    this->stateApp->alarmState->saveAlarmTimes(
        alarm_clock_json["weekdays_time"].as<const char *>(),
        alarm_clock_json["weekends_time"].as<const char *>(),
        alarm_clock_json["one_off_time"].as<const char *>());
    this->stateApp->alarmState->saveAlarmSwitchStates(
        alarm_clock_json["weekdays_enabled"].as<bool>(),
        alarm_clock_json["weekends_enabled"].as<bool>(),
        alarm_clock_json["one_off_enabled"].as<bool>());
    lv_msg_send(MSG_UPDATE_ALARM_GUI, NULL);
}

void ServerApp::saveRGBSettings(JsonVariant &json) {
    JsonObject &&rgb_json = json.as<JsonObject>();
    this->stateApp->rgbState->saveIsEnabled(rgb_json["rgb_enabled"].as<bool>());
    this->stateApp->rgbState->saveEffect(rgb_json["rgb_mode"].as<int>());
    this->stateApp->rgbState->saveColors(rgb_json["first_rgb_color"].as<int>(),
                                              rgb_json["second_rgb_color"].as<int>(),
                                              rgb_json["third_rgb_color"].as<int>());
    this->stateApp->rgbState->saveDelay(rgb_json["rgb_delay"].as<int>());
    this->stateApp->rgbState->saveRgbBrightness(rgb_json["rgb_brightness"].as<int>());
    this->stateApp->rgbState->saveTurnOffAtNight(rgb_json["rgb_night"].as<bool>());
}

void ServerApp::saveSoundSettings(JsonVariant &json) {
    JsonObject &&sound_json = json.as<JsonObject>();
    this->stateApp->soundState->saveAlarmTrackNumber(sound_json["alarm_track"].as<int>());
    this->stateApp->soundState->saveEETrackNumber(sound_json["ee_track"].as<int>());
    this->stateApp->soundState->savePlugInTrackNumber(sound_json["plug_track"].as<int>());
    this->stateApp->soundState->saveVolumeLevel(sound_json["volume_level"].as<int>());
    this->stateApp->soundState->saveSoundOn(sound_json["sound_on"].as<bool>());
    this->stateApp->soundState->saveEESoundOn(
        sound_json["ee_sound_on"].as<bool>());
    this->stateApp->soundState->savePlugInSoundOn(
        sound_json["plug_sound_on"].as<bool>());
    this->stateApp->soundState->enablePlayerUsb =
        sound_json["enable_player_usb"].as<bool>();
}

void ServerApp::setupSetTimeHandler() {
    AsyncCallbackJsonWebHandler *set_time_handler = new AsyncCallbackJsonWebHandler(
        "/time", [this](AsyncWebServerRequest *request, JsonVariant &json) {
            this->setTime(json);
            request->send(200);
        });
    server.addHandler(set_time_handler);
}

void ServerApp::setTime(JsonVariant &json) {
    if (this->stateApp->wifiState->wifiIsConnected) {
        return;
    }
    JsonObject &&time_json = json.as<JsonObject>();
    struct timeval tv;
    tv.tv_sec = time_json["time"].as<unsigned long>();
    tv.tv_usec = 0;
    Serial.println(time_json["time"].as<unsigned long>());
    settimeofday(&tv, NULL);
    this->stateApp->timeState->timeIsSet = true;
    lv_msg_send(MSG_UPDATE_TZ, NULL);
    lv_msg_send(MSG_UPDATE_TIME_TIMER, NULL);
}

void ServerApp::setupFirmwareUpdateHandler() {
    server.on(
        "/update_fw", HTTP_POST,
        [this](AsyncWebServerRequest *request) {
            this->espShouldReboot = !Update.hasError();
            AsyncResponseStream *response =
                request->beginResponseStream("application/json");
            response->print(this->espShouldReboot ? "{\"update\" : true}"
                                                  : "{\"update\" : false}");
            request->send(response);
        },
        [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data,
           size_t len, bool final) {
            if (!index) {
                Serial.printf("Update Start: %s\n", filename.c_str());
                if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH)) {
                    Update.printError(Serial);
                }
            }
            if (!Update.hasError()) {
                if (Update.write(data, len) != len) {
                    Update.printError(Serial);
                }
            }
            if (final) {
                if (Update.end(true)) {
                    Serial.printf("Update Success: %uB\n", index + len);
                } else {
                    Update.printError(Serial);
                }
            }
        });
}

void ServerApp::setupFilesystemUpdateHandler() {
    server.on(
        "/update_fs", HTTP_POST,
        [this](AsyncWebServerRequest *request) {
            this->espShouldReboot = !Update.hasError();
            AsyncResponseStream *response =
                request->beginResponseStream("application/json");
            response->print(this->espShouldReboot ? "{\"update\" : true}"
                                                  : "{\"update\" : false}");
            request->send(response);
        },
        [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data,
           size_t len, bool final) {
            if (!index) {
                Serial.printf("Update Start: %s\n", filename.c_str());
                if (!Update.begin(LittleFS.totalBytes(), U_SPIFFS)) {
                    Update.printError(Serial);
                }
            }
            if (!Update.hasError()) {
                if (Update.write(data, len) != len) {
                    Update.printError(Serial);
                }
            }
            if (final) {
                if (Update.end(true)) {
                    Serial.printf("Update Success: %uB\n", index + len);
                } else {
                    Update.printError(Serial);
                }
            }
        });
}

void ServerApp::setupWeatherImagesUploadHandlers() {
    server.on(
        "/weather_images_day", HTTP_POST,
        [this](AsyncWebServerRequest *request) {
            Serial.println("Get weather images for day");
        },
        [this](AsyncWebServerRequest *request, String filename, size_t index,
               uint8_t *data, size_t len, bool final) {
            this->handleUpload(request, filename, index, data, len, final,
                               "/weather/day/");
        });

    server.on(
        "/weather_images_night", HTTP_POST,
        [this](AsyncWebServerRequest *request) {
            Serial.println("Get weather images for night");
        },
        [this](AsyncWebServerRequest *request, String filename, size_t index,
               uint8_t *data, size_t len, bool final) {
            this->handleUpload(request, filename, index, data, len, final,
                               "/weather/night/");
        });
}

void ServerApp::handleUpload(AsyncWebServerRequest *request, String filename,
                             size_t index, uint8_t *data, size_t len, bool final,
                             const char *path) {
    if (!index) {
        Serial.printf("UploadStart: %s\n", filename.c_str());
        request->_tempFile = LittleFS.open(path + filename, "w");
    }
    if (len) {
        request->_tempFile.write(data, len);
    }
    if (final) {
        request->_tempFile.close();
        Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index + len);
    }
}

void ServerApp::setupGIFUploadHandler() {
    server.on(
        "/gif", HTTP_POST,
        [this](AsyncWebServerRequest *request) {
            Serial.println("Get GIF file");
            lv_msg_send(MSG_UPDATE_GIF_SRC, NULL);
        },
        [this](AsyncWebServerRequest *request, String filename, size_t index,
               uint8_t *data, size_t len, bool final) {
            this->handleUpload(request, filename, index, data, len, final, "/gif/");
        });
}

void ServerApp::setupWebUiUploadHandler() {
    server.on(
        "/frontend", HTTP_POST,
        [this](AsyncWebServerRequest *request) {
            Serial.println("Get index.html.gz file");
        },
        [this](AsyncWebServerRequest *request, String filename, size_t index,
               uint8_t *data, size_t len, bool final) {
            this->handleUpload(request, filename, index, data, len, final, "/");
        });
}

void ServerApp::setupAnalogClockImagesUploadHandler() {
    server.on(
        "/clock_images", HTTP_POST,
        [this](AsyncWebServerRequest *request) {
            Serial.println("Get analog clock images files");
            lv_msg_send(MSG_UPDATE_CLOCK_IMG_SRC, NULL);
        },
        [this](AsyncWebServerRequest *request, String filename, size_t index,
               uint8_t *data, size_t len, bool final) {
            this->handleUpload(request, filename, index, data, len, final,
                               "/analog_clock/");
        });
}

void ServerApp::run() {
    if (espShouldReboot) {
        Serial.println("ESP32 reboot ...");
        delay(100);
        ESP.restart();
    }
}
ServerApp::~ServerApp() {}