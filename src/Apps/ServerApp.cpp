#include "ServerApp.h"

static ServerApp *instance = NULL;

extern "C" void websocket_timer_cb_wrapper(lv_timer_t *timer) {
    instance->websocket_timer_cb(timer);
}

void onEventWrapper(AsyncWebSocket *server, AsyncWebSocketClient *client,
                    AwsEventType type, void *arg, uint8_t *data, size_t len) {
    instance->onEvent(server, client, type, arg, data, len);
}

ServerApp::ServerApp(StateApp *state_app, BrightnessApp *brightness_app,
                     MicroclimateApp *microclimate_app)
    : server(port), websocket("/ws") {
    instance = this;
    this->_state_app = state_app;
    this->_brightness_app = brightness_app;
    this->_microclimate_app = microclimate_app;
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    //           { request->send(200, "text/plain", "Hi! Welcome to OEClock."); });
}
void ServerApp::setup() {
    const char *http_username = "admin";
    const char *http_password = "admin";
    server.on("/", HTTP_GET,
              [http_username, http_password](AsyncWebServerRequest *request) {
                  if (!request->authenticate(http_username, http_password))
                      return request->requestAuthentication();
                  AsyncWebServerResponse *response =
                      request->beginResponse(SPIFFS, "/index.html.gz", "text/html");
                  response->addHeader("Content-Encoding", "gzip");
                  request->send(response);
              });
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

    server.on("/settings", HTTP_GET,
              std::bind(&ServerApp::get_settings, this, std::placeholders::_1));

    AsyncCallbackJsonWebHandler *time_settings_handler = new AsyncCallbackJsonWebHandler(
        "/settings/time", [this](AsyncWebServerRequest *request, JsonVariant &json) {
            this->save_time_settings(json);
            request->send(200);
        });
    server.addHandler(time_settings_handler);

    AsyncCallbackJsonWebHandler *weather_settings_handler =
        new AsyncCallbackJsonWebHandler(
            "/settings/weather",
            [this](AsyncWebServerRequest *request, JsonVariant &json) {
                this->save_weather_settings(json);
                request->send(200);
            });
    server.addHandler(weather_settings_handler);

    AsyncCallbackJsonWebHandler *theme_settings_handler = new AsyncCallbackJsonWebHandler(
        "/settings/theme", [this](AsyncWebServerRequest *request, JsonVariant &json) {
            this->save_theme_settings(json);
            request->send(200);
        });
    server.addHandler(theme_settings_handler);

    AsyncCallbackJsonWebHandler *brightness_settings_handler =
        new AsyncCallbackJsonWebHandler(
            "/settings/brightness",
            [this](AsyncWebServerRequest *request, JsonVariant &json) {
                this->save_brightness_settings(json);
                request->send(200);
            });
    server.addHandler(brightness_settings_handler);

    AsyncCallbackJsonWebHandler *wifi_settings_handler = new AsyncCallbackJsonWebHandler(
        "/settings/wifi", [this](AsyncWebServerRequest *request, JsonVariant &json) {
            this->save_wifi_settings(json);
            request->send(200);
        });
    server.addHandler(wifi_settings_handler);

    AsyncCallbackJsonWebHandler *set_time_handler = new AsyncCallbackJsonWebHandler(
        "/time", [this](AsyncWebServerRequest *request, JsonVariant &json) {
            this->set_time(json);
            request->send(200);
        });
    server.addHandler(set_time_handler);

    server.serveStatic("/", SPIFFS, "/").setCacheControl("max-age=604800");

    websocket.onEvent(onEventWrapper);

    server.addHandler(&websocket);
    server.begin();
    ElegantOTA.begin(&server);
}
void ServerApp::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
                        AwsEventType type, void *arg, uint8_t *data, size_t len) {
    switch (type) {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(),
                      client->remoteIP().toString().c_str());
        if (this->_websocket_notify_timer == NULL) {
            this->_websocket_notify_timer =
                lv_timer_create(websocket_timer_cb_wrapper, 600, NULL);
        }
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        if (this->_websocket_notify_timer != NULL & this->websocket.count() == 0) {
            lv_timer_del(this->_websocket_notify_timer);
            this->_websocket_notify_timer = NULL;
        }
        break;
    case WS_EVT_DATA:
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

void ServerApp::set_time(JsonVariant &json) {
    JsonObject &&time_json = json.as<JsonObject>();
    struct timeval tv;
    // tv.tv_sec = time_json["time"].as<unsigned long>();
    // settimeofday(&tv, NULL);
    Serial.println(time_json["time"].as<unsigned long>());
    // struct tm timeinfo;
    // time_t now;
    // time(&now);
    // localtime_r(&now, &timeinfo);
    // char s[51];
    // strftime(s, 6,
    //          "%H"
    //          ":"
    //          "%M",
    //          &timeinfo);
    // Serial.println(s);
}

void ServerApp::websocket_timer_cb(lv_timer_t *timer) {
    this->websocket.textAll(this->getSensorReadings());
}
String ServerApp::getSensorReadings() {
    String sensors_readings;
    StaticJsonDocument<64> doc;
    doc["temperature"] = this->_microclimate_app->get_temperature();
    doc["humidity"] = this->_microclimate_app->get_humidity();
    doc["lx"] = this->_brightness_app->get_light_level();
    doc["battery_level"] = 84;
    serializeJson(doc, sensors_readings);
    return sensors_readings;
}

void ServerApp::save_time_settings(JsonVariant &json) {
    JsonObject &&time_json = json.as<JsonObject>();
    this->_state_app->save_timezone(time_json["timezone_posix"].as<const char *>());
    this->_state_app->save_digital_main_screen(
        time_json["digital_main_screen"].as<bool>());
}

void ServerApp::save_weather_settings(JsonVariant &json) {
    JsonObject &&weather_json = json.as<JsonObject>();
    this->_state_app->save_weather_enabled(weather_json["weather_enabled"].as<bool>());
    this->_state_app->save_api_key(weather_json["api_key"].as<const char *>());
    this->_state_app->save_city(weather_json["city"].as<const char *>());
    this->_state_app->save_language(weather_json["language"].as<const char *>());
    this->_state_app->save_request_period(weather_json["request_period"].as<int>());
    lv_msg_send(MSG_WEATHER_ENABLED,
                static_cast<const void *>(&this->_state_app->weather_enabled));
}
void ServerApp::save_theme_settings(JsonVariant &json) {
    JsonObject &&theme_json = json.as<JsonObject>();
    this->_state_app->save_dark_theme_enabled(
        theme_json["dark_theme_enabled"].as<bool>());
    this->_state_app->save_light_colors(
        theme_json["light_background_color"].as<const char *>(),
        theme_json["light_second_color"].as<const char *>());
    this->_state_app->save_dark_colors(
        theme_json["dark_background_color"].as<const char *>(),
        theme_json["dark_second_color"].as<const char *>());
    lv_msg_send(MSG_CHANGE_THEME,
                static_cast<const void *>(&this->_state_app->dark_theme_enabled));
    //implement recolor for themes
}
void ServerApp::save_brightness_settings(JsonVariant &json) {
    JsonObject &&brightness_json = json.as<JsonObject>();
    this->_state_app->save_auto_brightness_enabled(
        brightness_json["auto_brightness"].as<bool>());
    this->_state_app->save_auto_theme_change(
        brightness_json["auto_theme_change"].as<bool>());
    this->_state_app->save_brightness_threshold(brightness_json["threshold"].as<int>());
    this->_state_app->save_brightness_level(
        brightness_json["brightness_level"].as<int>());
    lv_msg_send(MSG_BRIGHTNESS_CHANGED, NULL);
    lv_msg_send(MSG_AUTO_BRIGHTNESS, NULL);
}
void ServerApp::save_wifi_settings(JsonVariant &json) {
    JsonObject &&wifi_json = json.as<JsonObject>();
    this->_state_app->save_ssid(wifi_json["ssid"].as<const char *>());
    this->_state_app->save_password(wifi_json["password"].as<const char *>());
    Serial.println(wifi_json["ip_address"].as<const char *>());
    Serial.println(wifi_json["gateway"].as<const char *>());
    this->_state_app->save_sta_login(wifi_json["sta_login"].as<const char *>());
    this->_state_app->save_sta_password(wifi_json["sta_password"].as<const char *>());
}

void ServerApp::get_settings(AsyncWebServerRequest *request) {
    Serial.println("Request on settings");
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    StaticJsonDocument<512> doc;
    doc["ssid"] = this->_state_app->ssid;
    doc["password"] = this->_state_app->password;
    doc["ip_address"] = WiFi.localIP();
    doc["gateway"] = WiFi.gatewayIP();
    doc["sta_login"] = this->_state_app->sta_login;
    doc["sta_password"] = this->_state_app->sta_password;
    doc["auto_brightness"] = this->_state_app->auto_brightness;
    doc["auto_theme_change"] = false;
    doc["threshold"] = this->_state_app->threshold;
    doc["brightness_level"] = this->_state_app->brightness_level;
    doc["timezone_posix"] = this->_state_app->timezone_posix;
    doc["digital_main_screen"] = this->_state_app->digital_main_screen;
    doc["weather_enabled"] = this->_state_app->weather_enabled;
    doc["api_key"] = this->_state_app->api_key;
    doc["city"] = this->_state_app->city;
    doc["language"] = this->_state_app->language;
    doc["request_period"] = this->_state_app->request_period;
    doc["dark_theme_enabled"] = this->_state_app->dark_theme_enabled;
    doc["light_background_color"] = this->_state_app->light_background_color;
    doc["light_second_color"] = this->_state_app->light_second_color;
    doc["dark_background_color"] = this->_state_app->dark_background_color;
    doc["dark_second_color"] = this->_state_app->dark_second_color;
    serializeJson(doc, *response);
    request->send(response);
}
void ServerApp::run() { ElegantOTA.loop(); }
ServerApp::~ServerApp() {}