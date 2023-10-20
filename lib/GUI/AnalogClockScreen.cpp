#include "AnalogClockScreen.h"

void AnalogClockScreen::set_time(int hour, int minute, int second)
{
    int angle = 0;
    angle = second * 60;
    lv_img_set_angle(this->ui_ImageArmSecond, angle);
    angle = minute * 60;
    lv_img_set_angle(this->ui_ImageArmMinute, angle);
    angle = hour * 300 + (int)minute / 2 * 10;
    lv_img_set_angle(this->ui_ImageArmHour, angle);
}

AnalogClockScreen::AnalogClockScreen()
{
    LV_IMG_DECLARE(ui_img_watchface240_png);
    LV_IMG_DECLARE(ui_img_armhour_png);
    LV_IMG_DECLARE(ui_img_armminute_png);
    LV_IMG_DECLARE(ui_img_armsecond_png);
    ui_AnalogClockScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_AnalogClockScreen, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_AnalogClockPanel = lv_obj_create(ui_AnalogClockScreen);
    lv_obj_set_size(ui_AnalogClockPanel, lv_pct(100), lv_pct(100));
    lv_obj_set_align(ui_AnalogClockPanel, LV_ALIGN_TOP_LEFT);
    lv_obj_clear_flag(ui_AnalogClockPanel, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(ui_AnalogClockPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_AnalogClockPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImageWatchface = lv_img_create(ui_AnalogClockScreen);
    lv_img_set_src(ui_ImageWatchface, &ui_img_watchface240_png);
    lv_obj_set_width(ui_ImageWatchface, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_ImageWatchface, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_ImageWatchface, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageWatchface, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_ImageWatchface, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_ImageArmHour = lv_img_create(ui_AnalogClockScreen);
    lv_img_set_src(ui_ImageArmHour, &ui_img_armhour_png);
    lv_obj_set_width(ui_ImageArmHour, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_ImageArmHour, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_ImageArmHour, 0);
    lv_obj_set_y(ui_ImageArmHour, -35);
    lv_obj_set_align(ui_ImageArmHour, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageArmHour, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_ImageArmHour, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_img_set_pivot(ui_ImageArmHour, 9, 77);
    lv_img_set_angle(ui_ImageArmHour, 450);

    ui_ImageArmMinute = lv_img_create(ui_AnalogClockScreen);
    lv_img_set_src(ui_ImageArmMinute, &ui_img_armminute_png);
    lv_obj_set_width(ui_ImageArmMinute, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_ImageArmMinute, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_ImageArmMinute, 0);
    lv_obj_set_y(ui_ImageArmMinute, -49);
    lv_obj_set_align(ui_ImageArmMinute, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageArmMinute, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_ImageArmMinute, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_img_set_pivot(ui_ImageArmMinute, 9, 105);
    lv_img_set_angle(ui_ImageArmMinute, 1800);

    ui_ImageArmSecond = lv_img_create(ui_AnalogClockScreen);
    lv_img_set_src(ui_ImageArmSecond, &ui_img_armsecond_png);
    lv_obj_set_width(ui_ImageArmSecond, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_ImageArmSecond, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_ImageArmSecond, 0);
    lv_obj_set_y(ui_ImageArmSecond, -47);
    lv_obj_set_align(ui_ImageArmSecond, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageArmSecond, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_ImageArmSecond, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_img_set_pivot(ui_ImageArmSecond, 5, 115);
    lv_img_set_angle(ui_ImageArmSecond, 3150);

};

AnalogClockScreen::~AnalogClockScreen(){};