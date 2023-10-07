#include <ArduinoHttpClient.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <Preferences.h>
// #include <NTPClient.h>
// #include <WiFiUdp.h>
#include "time.h"
#include "Display.h"
#include "ui.h"
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
  ElegantOTA.begin(&server);
  server.begin();
  // #if LV_USE_LOG != 0
  //     lv_log_register_print_cb( my_print ); /* register print function for debugging */
  // #endif
  // printLocalTime();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "Hi! This is ElegantOTA AsyncDemo."); });
  // WiFi.disconnect();
  ui_init();



}
void loop()
{
  getLocalTime(&timeinfo);
  hour = timeinfo.tm_hour;
  minute = timeinfo.tm_min;
  second = timeinfo.tm_sec;
  angle = timeinfo.tm_sec * 60;
  lv_img_set_angle(ui_ImageArmSecond, angle);
  angle = timeinfo.tm_min * 60;
  lv_img_set_angle(ui_ImageArmMinute, angle);
  angle = timeinfo.tm_hour * 300 + (int)minute / 2 * 10;
  lv_img_set_angle(ui_ImageArmHour, angle);
  strftime(timeHour,3, "%H", &timeinfo);
  strftime(timeMinute,3, "%M", &timeinfo);
  strftime(timeSecond,3, "%S", &timeinfo);
  strcpy(fullTime, timeHour); // Copy str1 into result
  strcat(fullTime, ":");  // Add a space to result
  strcat(fullTime, timeMinute); // Concatenate str2 to result
  lv_label_set_text(ui_DigitalClockLabel, fullTime);
  lv_label_set_text(ui_DigitalClockSecondLabel, timeSecond);
  strftime(fullDate,20, "%d.%m.%Y", &timeinfo);
  lv_label_set_text(ui_DigitalClockDateLabel, fullDate);
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
  ElegantOTA.loop();
  if((unsigned long)millis()- time_now > 20000){
    time_now = millis();
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
    }
  }

}

