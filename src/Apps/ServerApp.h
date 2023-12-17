#include "StateApp.h"
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include "SPIFFS.h"

class ServerApp {
  private:
    short int port = 80;
    AsyncWebServer server;
    StateApp *_state_app;

  public:
    void setup();
    void run();
    ServerApp(StateApp *state_app);
    ~ServerApp();
};
