#pragma once
#include "lvgl.h"

class DockPanel
{
private:
    /* data */
public:

    lv_obj_t *ui_BatteryLabel;
    lv_obj_t *ui_SettingsButton;
    lv_obj_t *ui_SettingsButtonLabel;
    lv_obj_t *WiFiLabel;
    void change_dock_parent(lv_obj_t * new_parent);
    void show_wifi_connection(bool wifi_enabled);
    DockPanel(lv_obj_t *parent_panel);
    ~DockPanel();
};

