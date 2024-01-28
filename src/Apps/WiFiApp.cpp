#include "WiFiApp.h"

static WiFiApp *instance = NULL;

extern "C" void reconnect_to_wifi_cb(void *subscriber, lv_msg_t *msg) {
    instance->connect_to_wifi();
}

void wifi_disconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
    Serial.print("Disconnected from WiFi access point. Reason: ");
    Serial.println(info.wifi_sta_disconnected.reason);
    if (instance->_state_app->wifi_state->wifi_connected) {
        instance->handle_wifi_state(false);
    }
    // WiFi.reconnect();
}

WiFiApp::WiFiApp(StateApp *state_app, SemaphoreHandle_t &mutex) : mutex(mutex) {
    instance = this;
    this->_state_app = state_app;
    lv_msg_subscribe(MSG_WIFI_RECONNECT, reconnect_to_wifi_cb, NULL);
}

void WiFiApp::connect_to_wifi() {
    Serial.print("Trying to connect to WiFi");
    IPAddress local_ip;
    IPAddress gateway_ip;
    IPAddress primaryDNS(8, 8, 8, 8);
    IPAddress secondaryDNS(8, 8, 4, 4);
    IPAddress subnet(255, 255, 0, 0);
    local_ip.fromString(this->_state_app->wifi_state->ip_address);
    gateway_ip.fromString(this->_state_app->wifi_state->gateway_address);
    if (WiFi.isConnected()) {
        WiFi.disconnect();
    }
    WiFi.config(local_ip, gateway_ip, subnet, primaryDNS, secondaryDNS);
    WiFi.begin(this->_state_app->wifi_state->ssid,
               this->_state_app->wifi_state->password);
    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED & attempt < 20) {
        delay(500);
        Serial.print(".");
        attempt++;
    }
    this->handle_wifi_state(WiFi.status() == WL_CONNECTED);
}

void WiFiApp::handle_wifi_state(bool wifi_connected) {
    this->_state_app->wifi_state->wifi_connected = wifi_connected;
    if (wifi_connected) {
        this->_state_app->wifi_state->current_ip_address = WiFi.localIP().toString();
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        IPAddress local_ip;
        local_ip.fromString(this->_state_app->wifi_state->ip_address);
        IPAddress subnet(255, 255, 255, 0);
        WiFi.disconnect();
        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(local_ip, local_ip, subnet);
        WiFi.softAP(this->_state_app->wifi_state->ap_login,
                    this->_state_app->wifi_state->ap_password);
        this->_state_app->wifi_state->current_ip_address = WiFi.softAPIP().toString();
        Serial.println("Unable to connect to WiFi network. Initiate AP mode.");
    }
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        lv_msg_send(MSG_UPDATE_IP_ADDRESS, NULL);
        lv_msg_send(MSG_UPDATE_WEATHER_GUI, NULL);
        lv_msg_send(MSG_UPDATE_WEATHER_TASK, NULL);
        lv_msg_send(MSG_UPDATE_WEATHER_TASK, NULL);
        lv_msg_send(MSG_UPDATE_WIFI_CONNECTION_ICON, NULL);
        xSemaphoreGive(mutex);
    }
}

void WiFiApp::subscribe_to_wifi_disconnected_event() {
    WiFi.onEvent(wifi_disconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
}

WiFiApp::~WiFiApp() {}