#include "OEClockApp.h"

static OEClockApp *instance = NULL;

SemaphoreHandle_t mutex = xSemaphoreCreateMutex();

void serial_print(const char *buf) { Serial.println(buf); }
extern "C" void bme_timer_cb_wrapper(lv_timer_t *timer) { instance->bme_timer_cb(timer); }
extern "C" void light_sensor_timer_cb_wrapper(lv_timer_t *timer) {
    instance->light_sensor_timer_cb(timer);
}
extern "C" void auto_brightness_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    const bool *payload = static_cast<const bool *>(lv_msg_get_payload(msg));
    instance->set_auto_brightness_timer(*payload);
}
extern "C" void reconnect_to_wifi_cb(void *subscriber, lv_msg_t *msg) {
    instance->connect_to_wifi();
}
extern "C" void anim_brightness_change_cb(void *var, int32_t v) {
    instance->set_display_brightness(v);
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
    gui_app->alarm_clock->set_preferences(preferences);
    lv_msg_subscribe(MSG_WIFI_RECONNECT, reconnect_to_wifi_cb, NULL);
    lv_msg_subscribe(MSG_AUTO_BRIGHTNESS, auto_brightness_cb_wrapper, NULL);
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
    if (!_bme_sensor.begin(0x76)) {
        Serial.println("Can't find the BME280 sensor");
    }
    if (!_light_sensor.begin()) {
        Serial.println("Can't find the light sensor");
    }
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
    if (this->_wifi_connected) {
        time_app->config_time();
    }
    this->server_app->setup();

    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        lv_anim_init(&_brightness_anim);
        this->gui_app->init_gui();
        this->gui_app->delete_loading_screen();
        xSemaphoreGive(mutex);
        this->_bme_timer = lv_timer_create(bme_timer_cb_wrapper, 600, NULL);
    }
    vTaskDelete(update_display_task);
    WiFi.onEvent(WiFiStationDisconnected,
                 WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    lv_log_register_print_cb(serial_print);
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
    bool weekdays_sw = preferences.getBool("weekdays_sw", false);
    bool weekends_sw = preferences.getBool("weekends_sw", false);
    bool oneOff_sw = preferences.getBool("oneOff_sw", false);
    String weekdays_time = preferences.getString("weekdays_time", "08:00");
    String weekends_time = preferences.getString("weekends_time", "09:15");
    String oneOff_time = preferences.getString("oneOff_time", "13:23");
    preferences.end();

    this->weather_app->set_city_string(city.c_str());
    this->weather_app->set_language_string(language.c_str());
    this->weather_app->setup_weather_url();
    this->set_display_brightness(brightness);

    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        this->gui_app->switch_darktheme(dark_theme_enabled);
        this->set_auto_brightness_timer(auto_brightness);
        this->gui_app->alarm_clock->set_alarm_switches(weekdays_sw, weekends_sw,
                                                       oneOff_sw);
        this->gui_app->alarm_clock->set_alarm_buttons(
            weekdays_time.c_str(), weekends_time.c_str(), oneOff_time.c_str());
        this->gui_app->settings->set_wifi_settings(this->ssid.c_str(),
                                                   this->password.c_str());
        this->gui_app->settings->set_weather_settings(city.c_str(), language.c_str());
        this->gui_app->settings->set_brightness_widgets(brightness, auto_brightness);
        this->gui_app->settings->set_darktheme_switch(dark_theme_enabled);

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

    preferences.end();
    WiFi.mode(WIFI_AP_STA);
    Serial.print("Will try to connect to WiFI");
    WiFi.begin(ssid.c_str(), password.c_str());
    WiFi.softAP("OEClock", "admin1234");
    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED & attempt < 20) {
        delay(500);
        Serial.print(".");
        attempt++;
    }
    this->handle_wifi_state(WiFi.status() == WL_CONNECTED);
}

void OEClockApp::bme_timer_cb(lv_timer_t *timer) {
    this->gui_app->dock_panel->set_temperature(this->_bme_sensor.readTemperature() - 1);
    this->gui_app->dock_panel->set_humidity(
        static_cast<int>(this->_bme_sensor.readHumidity()));
}
void OEClockApp::light_sensor_timer_cb(lv_timer_t *timer) {
    int light_level = static_cast<int>(this->_light_sensor.readLightLevel());
    int old_brightness = this->display->get_brightness();
    if (light_level < 10) {
        this->change_brightness_smoothly(5, old_brightness);
        // this->set_display_brightness(5);
    } else if (light_level > 50 & light_level < 100) {
        this->change_brightness_smoothly(128, old_brightness);
        // this->set_display_brightness(128);
    } else if (light_level > 100) {
        this->change_brightness_smoothly(255, old_brightness);
        // this->set_display_brightness(255);
    }
}

void OEClockApp::handle_wifi_state(bool wifi_connected) {
    this->_wifi_connected = wifi_connected;
    if (wifi_connected) {
        preferences.begin(NAMESPACE);
        bool weather_enabled = this->preferences.getBool("weather_enab");
        preferences.end();
        this->weather_app->enable_weather(weather_enabled);
        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            gui_app->settings->set_ipAddressLabel(WiFi.localIP()[0], WiFi.localIP()[1],
                                                  WiFi.localIP()[2], WiFi.localIP()[3]);
            this->gui_app->settings->update_weather_controls_state(weather_enabled);
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
        this->gui_app->dock_panel->show_wifi_connection(this->_wifi_connected);
        xSemaphoreGive(mutex);
    }
}

void OEClockApp::set_display_brightness(u_int32_t brightness) {
    display->set_brightness((uint8_t)brightness);
}

void OEClockApp::set_auto_brightness_timer(bool auto_brightness) {
    if (this->_light_sensor_timer == NULL & auto_brightness) {
        _light_sensor_timer = lv_timer_create(light_sensor_timer_cb_wrapper, 600, NULL);
    } else if (this->_light_sensor_timer != NULL & !auto_brightness) {
        lv_timer_del(this->_light_sensor_timer);
        this->_light_sensor_timer = NULL;
    }
}

void OEClockApp::change_brightness_smoothly(int new_light_level, int old_light_level) {
    lv_anim_set_exec_cb(&_brightness_anim, anim_brightness_change_cb);
    lv_anim_set_var(&_brightness_anim, NULL);
    lv_anim_set_time(&_brightness_anim, 300);
    lv_anim_set_values(&_brightness_anim, old_light_level, new_light_level);
    lv_anim_start(&_brightness_anim);
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
