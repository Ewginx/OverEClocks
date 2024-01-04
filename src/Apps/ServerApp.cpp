#include "ServerApp.h"

ServerApp::ServerApp(StateApp *state_app) : server(port) {
    this->_state_app = state_app;
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

    server.serveStatic("/", SPIFFS, "/").setCacheControl("max-age=604800");

    server.on("/settings", HTTP_GET,
              std::bind(&ServerApp::get_settings, this, std::placeholders::_1));
    server.begin();
    ElegantOTA.begin(&server);
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