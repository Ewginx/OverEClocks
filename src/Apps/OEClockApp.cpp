#include "Apps/OEClockApp.h"
#include "OEClockApp.h"

static OEClockApp *instance = NULL;

extern "C" void wifi_switch_event_cb_wrapper(lv_event_t *e) {
    instance->wifi_switch_event_cb(e);
}
extern "C" void weather_switch_event_cb_wrapper(lv_event_t *e) {
    instance->weather_switch_event_cb(e);
}
OEClockApp::OEClockApp() {
    instance = this;
    display = new Display();
    gui_app = new GuiApp();
    weather_app = new WeatherApp(gui_app->weather);
    time_app = new TimeApp(gui_app->digital_clock, this->gui_app->analog_clock_screen,
                           this->gui_app->alarm_clock);
    server_app = new ServerApp();
    gui_app->settings->set_display(display);
    gui_app->settings->set_preferences(preferences);
    lv_obj_add_event_cb(this->gui_app->settings->wifiSwitch, wifi_switch_event_cb_wrapper,
                        LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(this->gui_app->settings->weatherSwitch,
                        weather_switch_event_cb_wrapper, LV_EVENT_ALL, NULL);
}

void OEClockApp::setup() {
    Serial.begin(115200);
    esp_log_level_set("*", ESP_LOG_DEBUG);
    // this->connect_to_wifi();
    this->gui_app->settings->init_settings();
    weather_app->create_weather_task();
    time_app->config_time();
    server_app->setup();
    // #if LV_USE_LOG != 0
    //     lv_log_register_print_cb( my_print ); /* register print function for debugging
    //     */
    // #endif
    gui_app->init_gui();
}

void OEClockApp::connect_to_wifi() {
    preferences.begin(NAMESPACE);
    String ssid = preferences.getString("ssid", this->ssid);
    String password = preferences.getString("password", this->password);
    preferences.end();
    WiFi.mode(WIFI_STA);
    Serial.print("Will try to connect to WiFI");
    WiFi.begin(ssid.c_str(), password.c_str());
    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED & attempt < 20) {
        delay(500);
        Serial.print(".");
        attempt++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        gui_app->settings->set_ipAddressLabel(WiFi.localIP()[0], WiFi.localIP()[1],
                                              WiFi.localIP()[2], WiFi.localIP()[3]);
        gui_app->dock_panel->show_wifi_connection(true);
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("Unable to connect to WiFi network");
        gui_app->dock_panel->show_wifi_connection(false);
    }
}

// void OEClockApp::setup_display_brightness() {
//     preferences.begin(NAMESPACE);
//     display->set_brightness((uint8_t)preferences.getUInt("brightness", 255));
//     preferences.end();
// }

void OEClockApp::wifi_switch_event_cb(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    lv_disp_t *disp = lv_disp_get_default();
    if (event_code == LV_EVENT_VALUE_CHANGED) {
        this->preferences.begin(NAMESPACE);
        this->preferences.putBool(
            "wifi_enabled", lv_obj_has_state(target, LV_STATE_CHECKED) ? true : false);
        this->preferences.end();
        if (lv_obj_has_state(target, LV_STATE_CHECKED)) {
            this->connect_to_wifi();
            Serial.println("WiFi connected");
        } else {
            this->gui_app->settings->set_ipAddressLabel(0, 0, 0, 0);
            Serial.println("Unable to connect to WiFi network");
            this->gui_app->dock_panel->show_wifi_connection(false);
            this->weather_app->enable_weather(false);
            lv_obj_add_state(this->gui_app->settings->weatherSwitch, LV_STATE_DEFAULT);
            WiFi.disconnect();
            Serial.println("WiFi disconnected");
        }
    }
}

void OEClockApp::weather_switch_event_cb(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    lv_disp_t *disp = lv_disp_get_default();
    if (event_code == LV_EVENT_VALUE_CHANGED) {
        this->preferences.begin(NAMESPACE);
        this->preferences.putBool(
            "weather_enab", lv_obj_has_state(target, LV_STATE_CHECKED) ? true : false);
        this->preferences.end();
        if (lv_obj_has_state(target, LV_STATE_CHECKED)) {
            this->weather_app->enable_weather();
            Serial.println("Weather enabled");

        } else {
            this->weather_app->enable_weather(false);
            Serial.println("Weather disabled");
        }
    }
}

void OEClockApp::loop() {
    lv_timer_handler();
    delay(5);
    time_app->notifyAboutTime();
    server_app->run();
}

OEClockApp::~OEClockApp() {}
