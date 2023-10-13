#include "OEClocksApp.h"

void OEClocksApp::setup()
{
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    // display = new Display();
    // gui_app = new GuiApp();
    Serial.print("Will try to connect to WiFI");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
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

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    getLocalTime(&timeinfo);
    // ElegantOTA.begin(&server);
    // server.begin();
    // #if LV_USE_LOG != 0
    //     lv_log_register_print_cb( my_print ); /* register print function for debugging */
    // #endif

    // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    //           { request->send(200, "text/plain", "Hi! This is ElegantOTA AsyncDemo."); });
    // WiFi.disconnect();
    // ui_init();
    gui_app->init_gui();
}

void OEClocksApp::loop()
{
    lv_timer_handler();
    delay(5);
    getLocalTime(&timeinfo);

    hour = timeinfo.tm_hour;
    minute = timeinfo.tm_min;
    second = timeinfo.tm_sec;
    gui_app->analog_clock_screen->set_time(hour, minute, second);
    strftime(fullTime, 6, "%H"
                          ":"
                          "%M",
             &timeinfo);
    strftime(timeSecond, 3, "%S", &timeinfo);
    gui_app->digital_clock_screen->set_time(fullTime, timeSecond);
    strftime(fullDate, 25, "%d.%m.%Y, %A", &timeinfo);
    gui_app->digital_clock_screen->set_date(fullDate);

    // ElegantOTA.loop();
    // if((unsigned long)millis()- time_now > 20000){
    //   time_now = millis();
    //   client.get("/comments?id=10");
    //   int statusCode = client.responseStatusCode();
    //   if (statusCode == 200)
    //   {
    //   String response = client.responseBody();
    //   Serial.print("Status code: ");
    //   Serial.println(statusCode);
    //   Serial.print("Response: ");
    //   Serial.println(response);
    //   Serial.println("Wait fifty seconds");
    //   }
    // }
}

OEClocksApp::OEClocksApp(/* args */): server(80)
{   
    display = new Display();
    gui_app = new GuiApp();
}

OEClocksApp::~OEClocksApp()
{
}