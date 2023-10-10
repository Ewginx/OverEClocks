#include "DockPanel.h"


DockPanel::DockPanel(lv_obj_t *parent_panel)
{
    lv_obj_t *panel = parent_panel;
    // lv_obj_t *panel = lv_obj_get_child(lv_scr_act(), 0);
    ui_BatteryLabel = lv_label_create(panel);
    lv_obj_set_pos(ui_BatteryLabel, 20, 0);
    lv_obj_set_align(ui_BatteryLabel, LV_ALIGN_TOP_LEFT);
    lv_obj_set_size(ui_BatteryLabel, 40, 30);
    lv_label_set_text(ui_BatteryLabel, LV_SYMBOL_BATTERY_2);
    lv_obj_set_style_text_font(ui_BatteryLabel, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingsButton = lv_btn_create(panel);
    lv_obj_set_size(ui_SettingsButton, 35, 35);
    lv_obj_set_pos(ui_SettingsButton, 401, -6);
    lv_obj_set_align(ui_SettingsButton, LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_bg_opa(ui_SettingsButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingsButtonLabel = lv_label_create(ui_SettingsButton);
    lv_obj_set_align(ui_SettingsButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_SettingsButtonLabel, LV_SYMBOL_SETTINGS);
    lv_obj_set_style_text_align(ui_SettingsButtonLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
}
void DockPanel::change_dock_parent(lv_obj_t *new_parent)
{
    lv_obj_set_parent(this->ui_SettingsButton, lv_obj_get_child(new_parent, 0));
    lv_obj_set_parent(this->ui_BatteryLabel, lv_obj_get_child(new_parent, 0));
}
DockPanel::~DockPanel()
{
}
