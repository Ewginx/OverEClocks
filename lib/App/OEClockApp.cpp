#include "OEClockApp.h"

static OEClockApp *instance = NULL;

TaskHandle_t Task1;

extern "C" void settings_button_event_cb_wrapper(lv_event_t *e) {
  instance->settings_button_event_cb(e);
}
extern "C" void darkmode_switch_event_cb_wrapper(lv_event_t *e) {
  instance->darkmode_switch_event_cb(e);
}
extern "C" void send_weather_request_wrapper(void * parameter) {
  instance->send_weather_request(parameter);
}

OEClockApp::OEClockApp(/* args */): server(80)
{   
    instance = this;
    display = new Display();
    gui_app = new GuiApp();
    settings = new Settings(display, preferences);
    time_app = new TimeApp(this->gui_app->digital_clock_screen, this->gui_app->analog_clock_screen, this->gui_app->alarm_screen);
    lv_obj_add_event_cb(this->gui_app->dock_panel->settingsButton, settings_button_event_cb_wrapper, LV_EVENT_CLICKED, NULL);
    Serial.begin(115200);
    
}

void OEClockApp::settings_button_event_cb(lv_event_t *e)
{    
        lv_obj_add_event_cb(this->settings->ui_DarkmodeSwitch, darkmode_switch_event_cb_wrapper, LV_EVENT_ALL, NULL);

        this->settings->load_settings_screen(lv_scr_act());
}

void OEClockApp::darkmode_switch_event_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_VALUE_CHANGED)
    {
        lv_disp_t *disp = lv_disp_get_default();
        if (lv_obj_has_state(target, LV_STATE_CHECKED))
        {
            lv_theme_t *theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_CYAN), lv_palette_main(LV_PALETTE_NONE),
                                                      false, LV_FONT_DEFAULT);
            lv_disp_set_theme(disp, theme);
            lv_obj_set_style_text_color(this->gui_app->dock_panel->settingsButtonLabel, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(this->gui_app->dock_panel->settingsButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(this->gui_app->alarm_screen->workingDayButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(this->gui_app->alarm_screen->weekendDayButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(this->gui_app->alarm_screen->oneOffButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(this->gui_app->alarm_screen->workingDayButtonLabel, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(this->gui_app->alarm_screen->weekendDayButtonLabel, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(this->gui_app->alarm_screen->oneOffButtonLabel, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        else
        {
            lv_theme_t *theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_TEAL), lv_palette_main(LV_PALETTE_TEAL),
                                                      true, LV_FONT_DEFAULT);
            lv_disp_set_theme(disp, theme);
            lv_obj_set_style_text_color(this->gui_app->dock_panel->settingsButtonLabel, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(this->gui_app->alarm_screen->workingDayButtonLabel, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(this->gui_app->alarm_screen->weekendDayButtonLabel, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(this->gui_app->alarm_screen->oneOffButtonLabel, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}


void OEClockApp::setup()
{
    xTaskCreatePinnedToCore(
      send_weather_request_wrapper, /* Function to implement the task */
      "request", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      0,  /* Priority of the task */
      &Task1,  /* Task handle. */
      0);
    // Serial.begin(115200);
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
    time_app->config_time();
    // configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    // getLocalTime(&timeinfo);
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


void OEClockApp::loop()
{
    lv_timer_handler();
    delay(5);

    time_app->notifyAboutTime();
    // getLocalTime(&timeinfo);
    // hour = timeinfo.tm_hour;
    // minute = timeinfo.tm_min;
    // second = timeinfo.tm_sec;
    // gui_app->analog_clock_screen->set_time(hour, minute, second);

    // strftime(fullTime, 6, "%H"":""%M",&timeinfo);
    // strftime(timeSecond, 3, "%S", &timeinfo);
    // gui_app->digital_clock_screen->set_time(fullTime, timeSecond);
    // strftime(fullDate, 25, "%d.%m.%Y, %A", &timeinfo);
    // gui_app->digital_clock_screen->set_date(fullDate);
    // ElegantOTA.loop();
}
void OEClockApp::send_weather_request(void *parameter)
{
    for (;;)
    {
        client.get("/comments?id=10");
        int statusCode = client.responseStatusCode();
        if (statusCode == 200)
        {
            String response = client.responseBody();
            Serial.print("Status code: ");
            Serial.println(statusCode);
            Serial.print("Response: ");
            Serial.println(response);
            Serial.println("Wait fifty seconds");
            Serial.print("loop() running on core ");
            Serial.println(xPortGetCoreID());
        }
    vTaskDelay(WEATHER_API_POLLING_INTERVAL/portTICK_PERIOD_MS);
    }
}
OEClockApp::~OEClockApp()
{
}
