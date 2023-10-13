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

    DockPanel(lv_obj_t *parent_panel);
    ~DockPanel();
};

