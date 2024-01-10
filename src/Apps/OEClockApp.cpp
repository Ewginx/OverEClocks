#include "OEClockApp.h"

static OEClockApp *instance = NULL;

SemaphoreHandle_t mutex = xSemaphoreCreateMutex();

void serial_print(const char *buf) { Serial.println(buf); }

extern "C" void reconnect_to_wifi_cb(void *subscriber, lv_msg_t *msg) {
    instance->connect_to_wifi();
}
void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
    Serial.print("Disconnected from WiFi access point. Reason: ");
    Serial.println(info.wifi_sta_disconnected.reason);
    if (instance->state_app->wifi_connected) {
        instance->handle_wifi_state(false);
    }
    // WiFi.reconnect();
}
void update_display(void *parameter) {
    for (;;) {
        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            lv_timer_handler();
            xSemaphoreGive(mutex);
        }
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}
OEClockApp::OEClockApp() {
    instance = this;
    display = new Display();
    state_app = new StateApp();
    gui_app = new GuiApp(this->state_app);
    weather_app = new WeatherApp(this->gui_app->weather, this->state_app);
    time_app = new TimeApp(gui_app->digital_clock, this->gui_app->analog_clock,
                           this->gui_app->alarm_clock, this->state_app);
    brightness_app =
        new BrightnessApp(this->display, this->gui_app->settings, this->state_app);
    microclimate_app = new MicroclimateApp(this->gui_app->dock_panel);
    server_app = new ServerApp(state_app, brightness_app, microclimate_app);

    lv_msg_subscribe(MSG_WIFI_RECONNECT, reconnect_to_wifi_cb, NULL);
}

void OEClockApp::setup() {
    Serial.begin(115200);
    if (!LittleFS.begin(true)) {
        Serial.println("An Error has occurred while mounting LittleFS");
    }
    lv_log_register_print_cb(serial_print);
    this->init_i2c_apps();
    lv_port_sd_fs_init();
    TaskHandle_t update_display_task;
    this->gui_app->create_loading_screen();
    xTaskCreatePinnedToCore(update_display,        /* Function to implement the task */
                            "update_display_task", /* Name of the task */
                            10000,                 /* Stack size in words */
                            NULL,                  /* Task input parameter */
                            0,                     /* Priority of the task */
                            &update_display_task,  /* Task handle. */
                            0);
    this->state_app->init_state();
    this->weather_app->setup_weather_url();
    this->brightness_app->set_display_brightness(this->state_app->brightness_level);
    this->init_gui();
    weather_app->create_weather_task();
    this->connect_to_wifi();
    if (this->state_app->wifi_connected) {
        time_app->config_time();
    }
    this->server_app->setup();

    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        this->gui_app->load_default_screen();
        this->gui_app->delete_loading_screen();
        xSemaphoreGive(mutex);
    }
    vTaskDelete(update_display_task);
    WiFi.onEvent(WiFiStationDisconnected,
                 WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
}

void OEClockApp::init_i2c_apps() {
    Wire.begin();
    Wire.setClock(400000);
    this->brightness_app->begin();
    this->microclimate_app->begin();
}

void OEClockApp::init_gui() {
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        this->gui_app->setup_gui();
        this->brightness_app->set_auto_brightness_timer(this->state_app->auto_brightness);
        xSemaphoreGive(mutex);
    } else {
        Serial.println("Can't obtain mutex");
    }
    Serial.println("Settings initialized");
}

void OEClockApp::connect_to_wifi() {
    Serial.print("Will try to connect to WiFI");
    IPAddress local_ip;
    IPAddress gateway_ip;
    IPAddress primaryDNS(8, 8, 8, 8);
    IPAddress secondaryDNS(8, 8, 4, 4);
    IPAddress subnet(255, 255, 0, 0);
    local_ip.fromString(instance->state_app->ip_address);
    gateway_ip.fromString(instance->state_app->gateway_address);
    if (WiFi.isConnected()) {
        WiFi.disconnect();
    }
    WiFi.config(local_ip, gateway_ip, subnet, primaryDNS, secondaryDNS);
    WiFi.begin(this->state_app->ssid, this->state_app->password);
    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED & attempt < 20) {
        delay(500);
        Serial.print(".");
        attempt++;
    }
    this->handle_wifi_state(WiFi.status() == WL_CONNECTED);
}

void OEClockApp::handle_wifi_state(bool wifi_connected) {
    this->state_app->wifi_connected = wifi_connected;
    if (wifi_connected) {
        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            gui_app->settings->set_ipAddressLabel(WiFi.localIP().toString().c_str());
            xSemaphoreGive(mutex);
        }
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        IPAddress local_ip;
        local_ip.fromString(instance->state_app->ip_address);
        IPAddress subnet(255, 255, 255, 0);
        WiFi.disconnect();
        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(local_ip, local_ip, subnet);
        WiFi.softAP(instance->state_app->ap_login, instance->state_app->ap_password);
        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            gui_app->settings->set_ipAddressLabel(WiFi.softAPIP().toString().c_str());
            xSemaphoreGive(mutex);
        }
        Serial.println("Unable to connect to WiFi network. Initiate AP mode.");
    }
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        this->gui_app->settings->update_weather_gui_state();
        this->weather_app->update_weather_task_state();
        this->gui_app->dock_panel->show_wifi_connection(this->state_app->wifi_connected);
        xSemaphoreGive(mutex);
    }
}

void OEClockApp::loop() {
    lv_timer_handler_run_in_period(5);
    server_app->run();
    if (this->state_app->wifi_connected || this->state_app->time_is_set) {
        time_app->notifyAboutTime();
        // Serial.println(ESP.getFreeHeap());
    }
}

OEClockApp::~OEClockApp() {}
