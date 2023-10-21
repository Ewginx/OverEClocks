#include "DockPanel.h"


DockPanel::DockPanel(lv_obj_t *parent_panel)
{
    lv_obj_t *panel = parent_panel;
    // lv_obj_t *panel = lv_obj_get_child(lv_scr_act(), 0);
    batteryLabel = lv_label_create(panel);
    lv_obj_set_pos(batteryLabel, 20, 0);
    lv_obj_set_align(batteryLabel, LV_ALIGN_TOP_LEFT);
    lv_obj_set_size(batteryLabel, 40, 30);
    lv_label_set_text(batteryLabel, LV_SYMBOL_BATTERY_2);
    lv_obj_set_style_text_font(batteryLabel, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    WiFiLabel = lv_label_create(panel);
    lv_obj_set_pos(WiFiLabel, 60, 0);
    lv_obj_set_align(WiFiLabel, LV_ALIGN_TOP_LEFT);
    lv_obj_set_size(WiFiLabel, 40, 30);
    lv_label_set_text(WiFiLabel, LV_SYMBOL_WIFI);
    lv_obj_set_style_text_font(WiFiLabel, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    settingsButton = lv_btn_create(panel);
    lv_obj_set_size(settingsButton, 35, 35);
    lv_obj_set_pos(settingsButton, 401, -6);
    lv_obj_set_align(settingsButton, LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_bg_opa(settingsButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    settingsButtonLabel = lv_label_create(settingsButton);
    lv_obj_set_align(settingsButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(settingsButtonLabel, LV_SYMBOL_SETTINGS);
    lv_obj_set_style_text_align(settingsButtonLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(settingsButtonLabel, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    
}
void DockPanel::change_dock_parent(lv_obj_t *new_parent)
{
    lv_obj_set_parent(this->settingsButton, lv_obj_get_child(new_parent, 0));
    lv_obj_set_parent(this->batteryLabel, lv_obj_get_child(new_parent, 0));
    lv_obj_set_parent(this->WiFiLabel, lv_obj_get_child(new_parent, 0));
}
void DockPanel::show_wifi_connection(bool wifi_enabled)
{
    if (!wifi_enabled)
    {
        lv_obj_add_flag(this->WiFiLabel, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_clear_flag(this->WiFiLabel, LV_OBJ_FLAG_HIDDEN);
    }
    
}
DockPanel::~DockPanel()
{
}
