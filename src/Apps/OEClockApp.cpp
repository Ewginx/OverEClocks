#include "OEClockApp.h"

static OEClockApp *instance = NULL;

SemaphoreHandle_t mutex = xSemaphoreCreateMutex();

void serial_print(const char *buf) { Serial.println(buf); }

extern "C" void reconnect_to_wifi_cb(void *subscriber, lv_msg_t *msg) {
    instance->connect_to_wifi();
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
    weather_app = new WeatherApp(gui_app->weather);
    time_app = new TimeApp(gui_app->digital_clock, this->gui_app->analog_clock,
                           this->gui_app->alarm_clock);
    server_app = new ServerApp();
    brightness_app = new BrightnessApp(this->display, this->gui_app->settings);
    microclimate_app = new MicroclimateApp(this->gui_app->dock_panel);

    this->weather_app->set_city_string(this->state_app->city.c_str());
    this->weather_app->set_language_string(this->state_app->language.c_str());
    this->weather_app->setup_weather_url();
    this->brightness_app->set_display_brightness(this->state_app->brightness_level);

    gui_app->settings->set_display(display);
    lv_msg_subscribe(MSG_WIFI_RECONNECT, reconnect_to_wifi_cb, NULL);
}
void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
    Serial.print("Disconnected from WiFi access point. Reason: ");
    Serial.println(info.wifi_sta_disconnected.reason);
    instance->handle_wifi_state(false);
    // Serial.println("Trying to Reconnect");
    // WiFi.reconnect();
}

void OEClockApp::setup() {
    Serial.begin(115200);
    Wire.begin();
    Wire.setClock(400000);
    brightness_app->begin();
    microclimate_app->begin();
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
    this->init_app();
    weather_app->create_weather_task();
    this->connect_to_wifi();
    if (this->state_app->wifi_connected) {
        time_app->config_time();
    }
    this->server_app->setup();

    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        this->gui_app->init_gui();
        this->gui_app->delete_loading_screen();
        xSemaphoreGive(mutex);
    }
    vTaskDelete(update_display_task);
    WiFi.onEvent(WiFiStationDisconnected,
                 WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    lv_log_register_print_cb(serial_print);
}

void OEClockApp::init_app() {
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        this->gui_app->switch_darktheme(this->state_app->dark_theme_enabled);
        this->brightness_app->set_auto_brightness_timer(this->state_app->auto_brightness);
        this->gui_app->alarm_clock->set_alarm_switches(
            this->state_app->weekdays_switch_enabled,
            this->state_app->weekends_switch_enabled,
            this->state_app->oneOff_switch_enabled);
        this->gui_app->alarm_clock->set_alarm_buttons(
            this->state_app->weekdays_time.c_str(),
            this->state_app->weekends_time.c_str(), this->state_app->oneOff_time.c_str());
        this->gui_app->settings->set_wifi_settings(this->state_app->ssid.c_str(),
                                                   this->state_app->password.c_str());
        this->gui_app->settings->set_weather_settings(this->state_app->city.c_str(),
                                                      this->state_app->language.c_str());
        this->gui_app->settings->set_brightness_slider(this->state_app->brightness_level);
        this->gui_app->settings->set_brightness_checkbox(
            this->state_app->auto_brightness);
        this->gui_app->settings->set_darktheme_switch(
            this->state_app->dark_theme_enabled);

        xSemaphoreGive(mutex);
    } else {
        Serial.println("Can't obtain mutex");
    }
    Serial.println("Settings initialized");
}

void OEClockApp::connect_to_wifi() {
    WiFi.mode(WIFI_AP_STA);
    Serial.print("Will try to connect to WiFI");
    WiFi.begin(this->state_app->ssid.c_str(), this->state_app->password.c_str());
    WiFi.softAP("OEClock", "admin1234");
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
        this->weather_app->enable_weather(this->state_app->weather_enabled);
        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            gui_app->settings->set_ipAddressLabel(WiFi.localIP()[0], WiFi.localIP()[1],
                                                  WiFi.localIP()[2], WiFi.localIP()[3]);
            this->gui_app->settings->update_weather_controls_state(
                this->state_app->weather_enabled);
            xSemaphoreGive(mutex);
        }
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        this->weather_app->enable_weather(false);
        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            this->gui_app->settings->disable_weather_controls();
            gui_app->settings->set_ipAddressLabel(WiFi.softAPIP()[0], WiFi.softAPIP()[1],
                                                  WiFi.softAPIP()[2], WiFi.softAPIP()[3]);
            xSemaphoreGive(mutex);
        }
        this->gui_app->set_screens_to_default_values();
        Serial.println("Unable to connect to WiFi network");
    }
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        this->gui_app->dock_panel->show_wifi_connection(this->state_app->wifi_connected);
        xSemaphoreGive(mutex);
    }
}

void OEClockApp::loop() {
    lv_timer_handler_run_in_period(5);
    if (this->state_app->wifi_connected) {
        time_app->notifyAboutTime();
        server_app->run();
        // Serial.println(ESP.getFreeHeap());
    }
}

OEClockApp::~OEClockApp() {}
