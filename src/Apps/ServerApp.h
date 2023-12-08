#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <SD.h>
class ServerApp
{
private:
    short int port = 80;
    AsyncWebServer server;

public:
    void setup();
    void run();
    ServerApp();
    ~ServerApp();
};


