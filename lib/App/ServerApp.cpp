#include "ServerApp.h"

ServerApp::ServerApp(/* args */): server(port)
{
    ElegantOTA.begin(&server);
    server.begin();
    // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    //           { request->send(200, "text/plain", "Hi! This is ElegantOTA AsyncDemo."); });
}
void ServerApp::run(){
    
    ElegantOTA.loop();

}
ServerApp::~ServerApp()
{
}