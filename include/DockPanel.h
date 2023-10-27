#pragma once
#include "lvgl.h"

class DockPanel
{
private:
    /* data */
public:

    lv_obj_t *batteryLabel;
    lv_obj_t *settingsButton;
    lv_obj_t *settingsButtonLabel;
    lv_obj_t *temperatureLabel;
    lv_obj_t *humidityLabel;
    lv_obj_t *WiFiLabel;

    void set_temperature(char * temperature);
    void set_humidity(char * humidity)

    void change_dock_parent(lv_obj_t * new_parent);
    void show_wifi_connection(bool wifi_enabled);
    DockPanel(lv_obj_t *parent_panel);
    ~DockPanel();
};

