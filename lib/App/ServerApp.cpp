#include "ServerApp.h"

ServerApp::ServerApp(/* args */): server(port)
{

    // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    //           { request->send(200, "text/plain", "Hi! This is ElegantOTA AsyncDemo."); });
}
void ServerApp::setup(){
    
    server.begin();
    ElegantOTA.begin(&server);

}
void ServerApp::run(){
    
    // ElegantOTA.loop();

}
ServerApp::~ServerApp()
{
}