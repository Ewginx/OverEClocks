#include "WiFiApp.h"

static WiFiApp *instance = NULL;

extern "C" void reconnectToWiFiCallback(void *subscriber, lv_msg_t *msg) {
    instance->connectToWiFi();
}

void wifiDisconnectedCallback(WiFiEvent_t event, WiFiEventInfo_t info) {
    Serial.print("Disconnected from WiFi access point. Reason: ");
    Serial.println(info.wifi_sta_disconnected.reason);
    instance->handleWiFiState(false);
    Serial.print("Trying to reconnect WiFi");
    while (WiFi.status() != WL_CONNECTED & instance->wifi_reconnect_attempts > 0) {
        bool wifi_reconnected = WiFi.reconnect();
        if (!wifi_reconnected) {
            delay(WIFI_RECONNECT_DELAY);
            Serial.print(".");
            instance->wifi_reconnect_attempts--;
        } else {
            Serial.println("\n WiFi reconnected");
            instance->handleWiFiState(wifi_reconnected);
            break;
        }
    }
}
bool WiFiApp::isWiFiConnected() { return this->stateApp->wifiState->wifiIsConnected; }

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
    localIP.fromString(this->stateApp->wifiState->ipAddress);
    gatewayIP.fromString(this->stateApp->wifiState->gatewayAddress);
    if (WiFi.isConnected()) {
        WiFi.disconnect();
    }
    WiFi.config(localIP, gatewayIP, subnet, primaryDNS, secondaryDNS);
    WiFi.begin(this->stateApp->wifiState->ssid, this->stateApp->wifiState->password);
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
    this->stateApp->wifiState->wifiIsConnected = wifiConnected;
    if (wifiConnected) {
        this->stateApp->wifiState->currentIpAddress = WiFi.localIP().toString();
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
    localIP.fromString(this->stateApp->wifiState->ipAddress);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(localIP, localIP, subnet);
    WiFi.softAP(this->stateApp->wifiState->apLogin,
                this->stateApp->wifiState->apPassword);
    this->stateApp->wifiState->currentIpAddress = WiFi.softAPIP().toString();
}

void WiFiApp::notifyOtherApps() {
    lv_msg_send(MSG_UPDATE_IP_ADDRESS, NULL);
    lv_msg_send(MSG_UPDATE_WEATHER_GUI, NULL);
    lv_msg_send(MSG_UPDATE_WEATHER_TASK, NULL);
    lv_msg_send(MSG_UPDATE_WIFI_CONNECTION_ICON, NULL);
    lv_msg_send(MSG_UPDATE_TIME_TIMER, NULL);
}

WiFiApp::~WiFiApp() {}