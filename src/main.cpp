
// #include <LovyanGFX.hpp>
#include <lvgl.h>
// #include <ArduinoHttpClient.h>
// #include <ESPAsyncWebServer.h>
// #include <Preferences.h>
#include "Display.h"
#include "ui.h"

// unsigned long time_now = 0;
// const char* ssid = "ssid";
// const char* password = "password";
// short int port = 80;
// String weather_url = "jsonplaceholder.typicode.com";
// WiFiClient wifi;
// HttpClient client = HttpClient(wifi, weather_url, port);

// Preferences preferences;

// 
// LGFX tft;
// #if LV_USE_LOG != 0
// /* Serial debugging */
// void my_print(const char * buf)
// {
//     Serial.printf(buf);
//     Serial.flush();
// }
// #endif

Display *display;

void setup() {
  display = new Display();


// #if LV_USE_LOG != 0
//     lv_log_register_print_cb( my_print ); /* register print function for debugging */
// #endif

    ui_init();

    // WiFi.begin(ssid, password);
    // while (WiFi.status() != WL_CONNECTED) {
    //   delay(500);
    //   Serial.print(".");
    // }
    // Serial.println("");
    // Serial.print("Connected to WiFi network with IP Address: ");
    // Serial.println(WiFi.localIP());
    // AsyncWebServer server(80);

}
void loop()
{
    lv_timer_handler(); /* let the GUI do its work */
    delay(5);
    // if((unsigned long)millis()- time_now > 1800000){
    //   time_now = millis();
    //   client.get("/comments?id=10");
    //   int statusCode = client.responseStatusCode();
    //   String response = client.responseBody();
    //   Serial.print("Status code: ");
    //   Serial.println(statusCode);
    //   Serial.print("Response: ");
    //   Serial.println(response);
    //   Serial.println("Wait fifty seconds");
    // }

}

