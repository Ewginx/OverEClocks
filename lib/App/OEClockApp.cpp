#include "OEClockApp.h"

static OEClockApp *instance = NULL;



OEClockApp::OEClockApp(/* args */): server(80)
{   
    Serial.begin(115200);
    instance = this;
    display = new Display();
    gui_app = new GuiApp();
    weather_app = new WeatherApp();
    time_app = new TimeApp(gui_app->digital_clock, this->gui_app->analog_clock_screen, this->gui_app->alarm);
    gui_app->settings->set_display(display);
    gui_app->settings->set_preferences(preferences);

}

void OEClockApp::setup()
{
    esp_log_level_set("*", ESP_LOG_DEBUG);
    WiFi.mode(WIFI_STA);
    Serial.print("Will try to connect to WiFI");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(300);
        Serial.print(".");
    }
    if (WiFi.status() != WL_CONNECTED)
    {

        gui_app->dock_panel->show_wifi_connection(false);
    }
    else
    {
        gui_app->dock_panel->show_wifi_connection(true);
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
    weather_app->create_weather_task();
    time_app->config_time();


    // ElegantOTA.begin(&server);
    // server.begin();
    // #if LV_USE_LOG != 0
    //     lv_log_register_print_cb( my_print ); /* register print function for debugging */
    // #endif

    // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    //           { request->send(200, "text/plain", "Hi! This is ElegantOTA AsyncDemo."); });
    // WiFi.disconnect();
    gui_app->init_gui();
}


void OEClockApp::loop()
{
    lv_timer_handler();
    delay(5);
    time_app->notifyAboutTime();
    // ElegantOTA.loop();
}

OEClockApp::~OEClockApp()
{
}
