#include "OEClockApp.h"

static OEClockApp *instance = NULL;

SemaphoreHandle_t mutex = xSemaphoreCreateMutex();

void serial_print(const char *buf) { Serial.println(buf); }

void update_display(void *parameter) {
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
    state_app = new StateApp();
    wifi_app = new WiFiApp(this->state_app, mutex);
    gui_app = new GuiApp(this->state_app);
    weather_app = new WeatherApp(this->gui_app->weather, this->state_app);
    time_app = new TimeApp(gui_app->digital_clock, this->gui_app->analog_clock,
                           this->gui_app->alarm_clock, this->state_app);
    brightness_app =
        new BrightnessApp(this->display, this->gui_app->settings, this->state_app);
    microclimate_app = new MicroclimateApp(this->gui_app->dock_panel);
    server_app = new ServerApp(state_app, brightness_app, microclimate_app);
}

void OEClockApp::setup() {
    Serial.begin(115200);
    lv_log_register_print_cb(serial_print);
    this->init_i2c_apps();
    // lv_port_sd_fs_init();
    lv_port_littlefs_fs_init();
    TaskHandle_t update_display_task;
    this->gui_app->create_loading_screen();
    xTaskCreatePinnedToCore(update_display,        /* Function to implement the task */
                            "update_display_task", /* Name of the task */
                            10000,                 /* Stack size in words */
                            NULL,                  /* Task input parameter */
                            0,                     /* Priority of the task */
                            &update_display_task,  /* Task handle. */
                            0);
    this->weather_app->setup_weather_url();
    this->brightness_app->set_display_brightness(
        this->state_app->display_state->brightness_level);
    this->init_gui();
    weather_app->create_weather_task();
    this->wifi_app->connect_to_wifi();
    if (this->state_app->wifi_state->wifi_connected) {
        time_app->config_time();
    }
    this->server_app->setup();
    this->wifi_app->subscribe_to_wifi_disconnected_event();
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        this->gui_app->load_default_screen();
        this->gui_app->delete_loading_screen();
        xSemaphoreGive(mutex);
    }
    vTaskDelete(update_display_task);
    Serial.printf("Full heap: %d KB \n", ESP.getHeapSize() / 1024);
    Serial.printf("Max free heap chunk: %d KB \n", ESP.getMaxAllocHeap() / 1024);
    Serial.printf("Free heap: %d KB \n", ESP.getFreeHeap() / 1024);
}

void OEClockApp::init_i2c_apps() {
    Wire.begin();
    Wire.setClock(400000);
    this->brightness_app->begin();
    this->microclimate_app->begin();
}

void OEClockApp::init_gui() {
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        this->gui_app->analog_clock->set_analog_clock_img_src();
        this->gui_app->setup_gui();
        this->brightness_app->set_auto_brightness_timer(
            this->state_app->display_state->auto_brightness);
        xSemaphoreGive(mutex);
    }
    Serial.println("GUI initialized");
}

void OEClockApp::loop() {
    // red line artifacts on screen, when use this timer handler, need proper
    // investigation lv_timer_handler_run_in_period(5);
    lv_task_handler();
    delay(5);
    server_app->run();
    if (this->state_app->wifi_state->wifi_connected ||
        this->state_app->time_state->time_is_set) {
        time_app->notifyAboutTime();
        // Serial.println(ESP.getFreeHeap());
    }
}

OEClockApp::~OEClockApp() {}
