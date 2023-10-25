#include "WeatherApp.h"

static WeatherApp *instance = NULL;


WeatherApp::WeatherApp()
{

}

void WeatherApp::send_weather_request(void *parameter)
{
    WeatherApp *l_pThis = (WeatherApp *) parameter;   
    for (;;)
    {
        if(WiFi.status() == WL_CONNECTED){
            l_pThis->client.get("/comments?id=10");
            int statusCode =  l_pThis->client.responseStatusCode();
            Serial.print("Status code: ");
            Serial.println(statusCode);
            if (statusCode == 200)
            {
                String response =  l_pThis->client.responseBody();
                Serial.print("Response: ");
                Serial.println(response);
                Serial.printf("Wait %i minutes", WEATHER_API_POLLING_INTERVAL_MINUTES);
            }
        }
        
    vTaskDelay(WEATHER_API_POLLING_INTERVAL_MILLISECONDS/portTICK_PERIOD_MS);
    }
}
void WeatherApp::create_weather_task()
{
    xTaskCreatePinnedToCore(
      this->send_weather_request, /* Function to implement the task */
      "request", /* Name of the task */
      10000,  /* Stack size in words */
      this,  /* Task input parameter */
      0,  /* Priority of the task */
      &this->Weather_Task,  /* Task handle. */
      0);
}
WeatherApp::~WeatherApp()
{
}