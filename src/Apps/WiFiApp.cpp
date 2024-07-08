#include "WiFiApp.h"

static WiFiApp *instance = NULL;

extern "C" void reconnectToWiFiCallback(void *subscriber, lv_msg_t *msg) {
    instance->connectToWiFi();
}

void wifiDisconnectedCallback(WiFiEvent_t event, WiFiEventInfo_t info) {
    Serial.print("Disconnected from WiFi access point. Reason: ");
    Serial.println(info.wifi_sta_disconnected.reason);
    if (instance->isWiFiConnected()) {
        instance->handleWiFiState(false);
    }
    // WiFi.reconnect();
}
bool WiFiApp::isWiFiConnected() { return this->stateApp->wifi_state->wifi_connected; }

WiFiApp::WiFiApp(StateApp *stateApp, SemaphoreHandle_t &mutex) : mutex(mutex) {
    instance = this;
    this->stateApp = stateApp;
    lv_msg_subscribe(MSG_WIFI_RECONNECT, reconnectToWiFiCallback, NULL);
}

void WiFiApp::setup() {
    this->connectToWiFi();
    this->subscribeToWiFiDisconnectedEvent();
}

void WiFiApp::connectToWiFi() {
    Serial.print("Trying to connect to WiFi");
    IPAddress localIP;
    IPAddress gatewayIP;
    IPAddress primaryDNS(8, 8, 8, 8);
    IPAddress secondaryDNS(8, 8, 4, 4);
    IPAddress subnet(255, 255, 0, 0);
    localIP.fromString(this->stateApp->wifi_state->ip_address);
    gatewayIP.fromString(this->stateApp->wifi_state->gateway_address);
    if (WiFi.isConnected()) {
        WiFi.disconnect();
    }
    WiFi.config(localIP, gatewayIP, subnet, primaryDNS, secondaryDNS);
    WiFi.begin(this->stateApp->wifi_state->ssid, this->stateApp->wifi_state->password);
    uint8_t attempt = 0;
    while (WiFi.status() != WL_CONNECTED & attempt < WIFI_CONNECT_ATTEMPTS) {
        delay(WIFI_CONNECT_DELAY);
        Serial.print(".");
        attempt++;
    }
    this->setupMDNS();
    WiFi.setSleep(true);
    this->handleWiFiState(WiFi.status() == WL_CONNECTED);
}

void WiFiApp::setupMDNS() {
    if (!MDNS.begin(MDNS_NAME)) {
        Serial.println("Error starting mDNS");
    } else {
        MDNS.addService("http", "tcp", 80);
    }
}

void WiFiApp::subscribeToWiFiDisconnectedEvent() {
    WiFi.onEvent(wifiDisconnectedCallback,
                 WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
}

void WiFiApp::handleWiFiState(bool wifiConnected) {
    this->stateApp->wifi_state->wifi_connected = wifiConnected;
    if (wifiConnected) {
        this->stateApp->wifi_state->current_ip_address = WiFi.localIP().toString();
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("Unable to connect to WiFi network. Initiate AP mode.");
        this->initiateAPMode();
    }
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        this->notifyOtherApps();
        xSemaphoreGive(mutex);
    }
}
void WiFiApp::initiateAPMode() {
    IPAddress localIP;
    localIP.fromString(this->stateApp->wifi_state->ip_address);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(localIP, localIP, subnet);
    WiFi.softAP(this->stateApp->wifi_state->ap_login,
                this->stateApp->wifi_state->ap_password);
    this->stateApp->wifi_state->current_ip_address = WiFi.softAPIP().toString();
}

void WiFiApp::notifyOtherApps() {
    lv_msg_send(MSG_UPDATE_IP_ADDRESS, NULL);
    lv_msg_send(MSG_UPDATE_WEATHER_GUI, NULL);
    lv_msg_send(MSG_UPDATE_WEATHER_TASK, NULL);
    lv_msg_send(MSG_UPDATE_WIFI_CONNECTION_ICON, NULL);
    lv_msg_send(MSG_UPDATE_TIME_TIMER, NULL);
}

WiFiApp::~WiFiApp() {}