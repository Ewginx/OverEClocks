#include "OEClockApp.h"

static OEClockApp *instance = NULL;

SemaphoreHandle_t mutex = xSemaphoreCreateMutex();

extern "C" void setLowCpuFrequency(void *subscriber, lv_msg_t *msg) {
    setCpuFrequencyMhz(80);
}
extern "C" void setHighCpuFrequency(void *subscriber, lv_msg_t *msg) {
    setCpuFrequencyMhz(240);
}

void serialPrint(const char *buf) { Serial.println(buf); }

void updateDisplay(void *parameter) {
    for (;;) {
        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            lv_timer_handler();
            xSemaphoreGive(mutex);
        }
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}
OEClockApp::OEClockApp() {
    instance = this;
    display = new Display();
    stateApp = new StateApp();
    rgbApp = new RGBApp(this->stateApp);
    wifiApp = new WiFiApp(this->stateApp, mutex);
    guiApp = new GuiApp(this->stateApp);
    weatherApp = new WeatherApp(this->guiApp->weather, this->stateApp);
    timeApp = new TimeApp(guiApp->digital_clock, this->guiApp->analog_clock,
                          this->guiApp->alarm_clock, this->stateApp);
    brightnessApp =
        new BrightnessApp(this->display, this->guiApp->settings, this->stateApp);
    microclimateApp = new MicroclimateApp(this->guiApp->dock_panel, this->stateApp);
    serverApp = new ServerApp(stateApp, brightnessApp);
    soundApp = new SoundApp(stateApp);
    buttonApp = new ButtonApp(stateApp);
    batteryApp = new BatteryApp(stateApp);
    lv_msg_subscribe(MSG_SET_LOW_CLOCK, setLowCpuFrequency, NULL);
    lv_msg_subscribe(MSG_SET_HIGH_CLOCK, setHighCpuFrequency, NULL);
}

void OEClockApp::setup() {
    Serial.begin(115200);
    lv_log_register_print_cb(serialPrint);
    TaskHandle_t updateDisplayTask = this->createLoadingScreenTask();
    this->initI2CApps();
    // lv_port_sd_fs_init();
    lv_port_littlefs_fs_init();
    this->initGUI();
    this->initApps();
    this->deleteLoadingScreenTask(updateDisplayTask);
    setCpuFrequencyMhz(80);
    this->printHeapInformation();
}
void OEClockApp::initI2CApps() {
    Serial.println();
    Serial.println("Start I2C Apps initialization");
    Wire.begin();
    Wire.setClock(100000);
    this->brightnessApp->begin();
    this->microclimateApp->begin();
    Serial.println("I2C Apps initialized");
}

void OEClockApp::initGUI() {
    Serial.println();
    Serial.println("Start GUI initialization");
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        this->guiApp->analog_clock->set_analog_clock_img_src();
        this->guiApp->setup_gui();
        this->brightnessApp->setAutoBrightnessTimer(
            this->stateApp->displayState->autoBrightness);
        xSemaphoreGive(mutex);
    }
    Serial.println("GUI initialized");
}

void OEClockApp::initApps() {
    Serial.println();
    Serial.println("Start Apps initialization");
    this->soundApp->setup();
    this->brightnessApp->setup();
    this->weatherApp->setup();
    this->wifiApp->setup();
    this->serverApp->setup();
    this->rgbApp->setup();
    this->batteryApp->setup();
    Serial.println("Apps initialized");
}

void OEClockApp::printHeapInformation() {
    Serial.println();
    Serial.printf("Full heap: %u KB \n", ESP.getHeapSize() / 1024);
    Serial.printf("Max free heap chunk: %u KB \n", ESP.getMaxAllocHeap() / 1024);
    Serial.printf("Free heap: %u KB \n", ESP.getFreeHeap() / 1024);
}

TaskHandle_t OEClockApp::createLoadingScreenTask() {
    TaskHandle_t updateDisplayTask;
    this->guiApp->create_loading_screen();
    xTaskCreatePinnedToCore(updateDisplay,       /* Function to implement the task */
                            "updateDisplayTask", /* Name of the task */
                            3072,                /* Stack size in words */
                            NULL,                /* Task input parameter */
                            0,                   /* Priority of the task */
                            &updateDisplayTask,  /* Task handle. */
                            0);
    return updateDisplayTask;
}

void OEClockApp::deleteLoadingScreenTask(TaskHandle_t updateDisplayTask) {
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        this->guiApp->load_default_screen();
        xSemaphoreGive(mutex);
    }
    vTaskDelete(updateDisplayTask);
}

void OEClockApp::loop() {
    /* red line artifacts on screen, when use this timer handler, need proper
    investigation lv_timer_handler_run_in_period(5);*/
    lv_task_handler();
    delay(5);
    serverApp->run();
}

OEClockApp::~OEClockApp() {}
