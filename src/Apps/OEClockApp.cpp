#include "Apps/OEClockApp.h"


OEClockApp::OEClockApp()
{   

    display = new Display();
    gui_app = new GuiApp();
    weather_app = new WeatherApp();
    time_app = new TimeApp(gui_app->digital_clock, this->gui_app->analog_clock_screen, this->gui_app->alarm);
    server_app = new ServerApp();
    gui_app->settings->set_display(display);
    gui_app->settings->set_preferences(preferences);

}



void OEClockApp::setup()
{
    Serial.begin(115200);
    esp_log_level_set("*", ESP_LOG_DEBUG);
    this->connect_wifi();
    weather_app->create_weather_task();
    time_app->config_time();
    server_app->setup();
    // #if LV_USE_LOG != 0
    //     lv_log_register_print_cb( my_print ); /* register print function for debugging */
    // #endif

    gui_app->init_gui();
}

void OEClockApp::connect_wifi()
{
    preferences.begin(NAMESPACE);
    String ssid = preferences.getString("ssid", this->ssid);
    String password =  preferences.getString("password", this->password);;
    WiFi.mode(WIFI_STA);
    Serial.print("Will try to connect to WiFI");
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(300);
        Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        gui_app->dock_panel->show_wifi_connection(true);

    }
    else
    {
        gui_app->dock_panel->show_wifi_connection(false);
    }
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
}

void OEClockApp::setup_display_brightness()
{
    preferences.begin(NAMESPACE);
    display->set_brightness(preferences.getUChar("brightness_level", 255));
    preferences.end();
}

void OEClockApp::loop()
{
    lv_timer_handler();
    delay(5);
    time_app->notifyAboutTime();
    server_app->run();
}

OEClockApp::~OEClockApp()
{
}
