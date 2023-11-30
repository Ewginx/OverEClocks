#include "Apps/OEClockApp.h"
#include "OEClockApp.h"
static OEClockApp *instance = NULL;

SemaphoreHandle_t mutex = xSemaphoreCreateMutex();

void reconnect_to_wifi_cb(void *subscriber, lv_msg_t *msg) {
    Serial.println("Get message about reconnect to wifi");
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
    gui_app = new GuiApp();
    weather_app = new WeatherApp(gui_app->weather);
    time_app = new TimeApp(gui_app->digital_clock, this->gui_app->analog_clock,
                           this->gui_app->alarm_clock);
    server_app = new ServerApp();
    gui_app->settings->set_display(display);
    gui_app->settings->set_preferences(preferences);
    lv_msg_subscribe(MSG_WIFI_RECONNECT, reconnect_to_wifi_cb, NULL);
}

void OEClockApp::setup() {
    Serial.begin(115200);
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
    if (this->_wifi_connected) {
        time_app->config_time();
        server_app->setup();
    }

    // #if LV_USE_LOG != 0
    //     lv_log_register_print_cb( my_print ); /* register print function for
    //     debugging
    //     */
    // #endif
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        gui_app->init_gui();
        this->gui_app->delete_loading_screen();
        xSemaphoreGive(mutex);
    }
    vTaskDelete(update_display_task);
}

void OEClockApp::init_app() {
    preferences.begin(NAMESPACE);
    this->ssid = preferences.getString("ssid", "N/A");
    this->password = preferences.getString("password", "N/A");
    String city = preferences.getString("city");
    String language = preferences.getString("language");
    bool auto_brightness = preferences.getBool("auto_bright", true);
    u_int32_t brightness = preferences.getUInt("brightness", 255);
    bool weather_enabled = preferences.getBool("weather_enab", true);
    bool dark_theme_enabled = preferences.getBool("dark_theme", false);
    preferences.end();

    this->weather_app->set_city_string(city.c_str());
    this->weather_app->set_language_string(language.c_str());
    this->weather_app->setup_weather_url();
    // this->weather_app->enable_weather(weather_enabled);
    this->set_display_brightness(brightness);

    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        this->gui_app->settings->set_wifi_settings(this->ssid.c_str(),
                                                   this->password.c_str());
        this->gui_app->settings->set_weather_settings(city.c_str(), language.c_str());
        this->gui_app->settings->set_brightness_widgets(brightness, auto_brightness);
        this->gui_app->settings->set_darktheme_switch(dark_theme_enabled);
        lv_event_send(this->gui_app->settings->darkmodeSwitch, LV_EVENT_VALUE_CHANGED,
                      NULL);
        xSemaphoreGive(mutex);
    } else {
        Serial.println("Can't obtain mutex");
    }
    Serial.println("Settings initialized");
}

void OEClockApp::connect_to_wifi() {
    preferences.begin(NAMESPACE);
    this->ssid = preferences.getString("ssid");
    this->password = preferences.getString("password");
    bool weather_enabled = this->preferences.getBool("weather_enab");
    preferences.end();
    WiFi.mode(WIFI_AP_STA);
    Serial.print("Will try to connect to WiFI");
    WiFi.begin(ssid.c_str(), password.c_str());
    WiFi.softAP("OEClock", "admin1234");
    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED & attempt < 20) {
        // lv_timer_handler();
        delay(500);
        Serial.print(".");
        attempt++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        this->_wifi_connected = true;
        this->weather_app->enable_weather(weather_enabled);
        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            gui_app->settings->set_ipAddressLabel(WiFi.localIP()[0], WiFi.localIP()[1],
                                                  WiFi.localIP()[2], WiFi.localIP()[3]);
            lv_obj_clear_state(this->gui_app->settings->weatherSwitch, LV_STATE_DISABLED);
            this->gui_app->settings->update_weather_controls_state(weather_enabled);
            xSemaphoreGive(mutex);
        }
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        this->weather_app->enable_weather(false);
        this->_wifi_connected = false;
        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            this->gui_app->settings->disable_weather_controls();
            gui_app->settings->set_ipAddressLabel(WiFi.softAPIP()[0], WiFi.softAPIP()[1],
                                                  WiFi.softAPIP()[2], WiFi.softAPIP()[3]);
            xSemaphoreGive(mutex);
        }
        Serial.println("Unable to connect to WiFi network");
    }
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        this->gui_app->dock_panel->show_wifi_connection(this->_wifi_connected);
        xSemaphoreGive(mutex);
    }
    Serial.println("Exit connect to wifi method");
}

void OEClockApp::set_display_brightness(u_int32_t brightness) {
    display->set_brightness((uint8_t)brightness);
}

void OEClockApp::loop() {
    lv_timer_handler_run_in_period(5);
    if (this->_wifi_connected) {
        time_app->notifyAboutTime();
        server_app->run();
        // Serial.println(ESP.getFreeHeap());
    }
}

OEClockApp::~OEClockApp() {}
