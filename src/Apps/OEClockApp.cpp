#include "Apps/OEClockApp.h"
#include "OEClockApp.h"
static OEClockApp *instance = NULL;

void reconnect_to_wifi_cb(void *subscriber, lv_msg_t *msg) {
    Serial.println("Get message about reconnect to wifi");
    instance->connect_to_wifi();
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
    esp_log_level_set("*", ESP_LOG_DEBUG);
    this->init_app();
    this->connect_to_wifi();
    if (this->_wifi_connected) {
        weather_app->create_weather_task();
        time_app->config_time();
        server_app->setup();
    }

    // #if LV_USE_LOG != 0
    //     lv_log_register_print_cb( my_print ); /* register print function for debugging
    //     */
    // #endif
    gui_app->init_gui();
}

void OEClockApp::init_app() {
    preferences.begin(NAMESPACE);

    this->gui_app->settings->set_wifi_settings(
        preferences.getString("ssid", "N/A").c_str(),
        preferences.getString("password", "N/A").c_str());

    bool weather_enabled = preferences.getBool("weather_enab", true);
    this->weather_app->enable_weather(weather_enabled);
    String city = preferences.getString("city", "");
    this->gui_app->settings->set_weather_settings(city.c_str(), weather_enabled);

    bool auto_brightness = preferences.getBool("auto_bright", true);
    u_int32_t brightness = preferences.getUInt("brightness", 255);
    this->gui_app->settings->set_brightness_widgets(brightness, auto_brightness);

    this->set_display_brightness(brightness);

    this->gui_app->settings->set_darktheme_switch(
        preferences.getBool("dark_theme", false));
    lv_event_send(this->gui_app->settings->darkmodeSwitch, LV_EVENT_VALUE_CHANGED, NULL);

    preferences.end();

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
        lv_timer_handler();
        delay(500);
        Serial.print(".");
        attempt++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        this->_wifi_connected = true;
        gui_app->settings->set_ipAddressLabel(WiFi.localIP()[0], WiFi.localIP()[1],
                                              WiFi.localIP()[2], WiFi.localIP()[3]);
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(WiFi.localIP());
        lv_obj_clear_state(this->gui_app->settings->weatherSwitch, LV_STATE_DISABLED);
        this->gui_app->settings->update_weather_switch_state(weather_enabled);
        this->weather_app->enable_weather(weather_enabled);

    } else {
        gui_app->settings->set_ipAddressLabel(WiFi.softAPIP()[0], WiFi.softAPIP()[1],
                                              WiFi.softAPIP()[2], WiFi.softAPIP()[3]);
        this->_wifi_connected = false;
        Serial.println("Unable to connect to WiFi network");
        this->gui_app->settings->disable_weather_switch();
        this->weather_app->enable_weather(false);
    }
    this->gui_app->dock_panel->show_wifi_connection(this->_wifi_connected);
}

void OEClockApp::set_display_brightness(u_int32_t brightness) {
    display->set_brightness((uint8_t)brightness);
}

void OEClockApp::loop() {
    lv_timer_handler();
    delay(5);
    if (this->_wifi_connected) {
        time_app->notifyAboutTime();
        server_app->run();
    }
}

OEClockApp::~OEClockApp() {}
