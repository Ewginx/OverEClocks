#include <ArduinoHttpClient.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <Preferences.h>
#include "time.h"
#include "Display.h"
// #include "ui.h"
#include "App.h"

#define ONE_MINUTE_MS (60 * 1000)
#define ONE_HOUR_MS (60 * 60 * 1000)
#define TWELVE_HOUR_MS (12 * 60 * 60 * 1000)

unsigned long time_now = 0;
const char* ssid = "ssid";
const char* password = "password";
short int port = 80;
String weather_url = "jsonplaceholder.typicode.com";
WiFiClient wifi;
HttpClient client = HttpClient(wifi, weather_url, port);
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 36000;
const int   daylightOffset_sec = 0;
struct tm timeinfo;
char timeHour[3];
char timeMinute[3];
char timeSecond[3];
char fullTime[8];
char fullDate[20];
int16_t angle;
int hour;
int minute;
int second;
// WiFiUDP ntpUDP;
Preferences preferences;
AsyncWebServer server(80);
Display *display;
App *gui_app;
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

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  display = new Display();
  gui_app = new App();
  Serial.print("Will try to connect to WiFI");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
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
void loop()
{
  lv_timer_handler();
  delay(5);
  getLocalTime(&timeinfo);

  hour = timeinfo.tm_hour;
  minute = timeinfo.tm_min;
  second = timeinfo.tm_sec;
  gui_app->analog_clock_screen->set_time(hour, minute, second);
  strftime(fullTime,6, "%H"":""%M", &timeinfo);
  strftime(timeSecond,3, "%S", &timeinfo);
  gui_app->digital_clock_screen->set_time(fullTime, timeSecond);
  strftime(fullDate,25, "%d.%m.%Y, %A", &timeinfo);
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

