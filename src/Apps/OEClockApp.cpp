#include "Apps/OEClockApp.h"

OEClockApp::OEClockApp() {

    display = new Display();
    gui_app = new GuiApp();
    weather_app = new WeatherApp(gui_app->weather);
    time_app = new TimeApp(gui_app->digital_clock, this->gui_app->analog_clock_screen,
                           this->gui_app->alarm_clock);
    server_app = new ServerApp();
    gui_app->settings->set_display(display);
    gui_app->settings->set_preferences(preferences);
}

void OEClockApp::setup() {
    Serial.begin(115200);
    esp_log_level_set("*", ESP_LOG_DEBUG);
    this->connect_to_wifi();
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
    while (WiFi.status() != WL_CONNECTED & attempt < 20)
    {
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

void OEClockApp::setup_display_brightness() {
    preferences.begin(NAMESPACE);
    display->set_brightness(preferences.getUChar("brightness_level", 255));
    preferences.end();
}

void OEClockApp::loop() {
    lv_timer_handler();
    delay(5);
    time_app->notifyAboutTime();
    server_app->run();
}

OEClockApp::~OEClockApp() {}
