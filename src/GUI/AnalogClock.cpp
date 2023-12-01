#include "AnalogClock.h"

void AnalogClock::set_time(int hour, int minute, int second) {
    int angle = 0;
    angle = second * 60;
    lv_img_set_angle(this->imageArmSecond, angle);
    angle = minute * 60;
    lv_img_set_angle(this->imageArmMinute, angle);
    angle = hour * 300 + (int)minute / 2 * 10;
    lv_img_set_angle(this->imageArmHour, angle);
}

AnalogClock::AnalogClock() {
    LV_IMG_DECLARE(img_watchface240_png);
    LV_IMG_DECLARE(img_armhour_png);
    LV_IMG_DECLARE(img_armminute_png);
    LV_IMG_DECLARE(img_armsecond_png);
    analogClockScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(analogClockScreen, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    analogClockPanel = lv_obj_create(analogClockScreen);
    lv_obj_set_size(analogClockPanel, lv_pct(100), lv_pct(100));
    lv_obj_set_align(analogClockPanel, LV_ALIGN_TOP_LEFT);
    lv_obj_clear_flag(analogClockPanel, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(analogClockPanel, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(analogClockPanel, 0, 0);

    watchface = lv_img_create(analogClockScreen);
    lv_img_set_src(watchface, &img_watchface240_png);
    lv_obj_set_width(watchface, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(watchface, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(watchface, LV_ALIGN_CENTER);
    lv_obj_add_flag(watchface, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(watchface, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    imageArmHour = lv_img_create(analogClockScreen);
    lv_img_set_src(imageArmHour, &img_armhour_png);
    lv_obj_set_width(imageArmHour, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(imageArmHour, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(imageArmHour, 0);
    lv_obj_set_y(imageArmHour, -35);
    lv_obj_set_align(imageArmHour, LV_ALIGN_CENTER);
    lv_obj_add_flag(imageArmHour, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(imageArmHour, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_img_set_pivot(imageArmHour, 9, 77);
    lv_img_set_angle(imageArmHour, 450);

    imageArmMinute = lv_img_create(analogClockScreen);
    lv_img_set_src(imageArmMinute, &img_armminute_png);
    lv_obj_set_width(imageArmMinute, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(imageArmMinute, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(imageArmMinute, 0);
    lv_obj_set_y(imageArmMinute, -49);
    lv_obj_set_align(imageArmMinute, LV_ALIGN_CENTER);
    lv_obj_add_flag(imageArmMinute, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(imageArmMinute, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_img_set_pivot(imageArmMinute, 9, 105);
    lv_img_set_angle(imageArmMinute, 1800);

    imageArmSecond = lv_img_create(analogClockScreen);
    lv_img_set_src(imageArmSecond, &img_armsecond_png);
    lv_obj_set_width(imageArmSecond, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(imageArmSecond, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(imageArmSecond, 0);
    lv_obj_set_y(imageArmSecond, -47);
    lv_obj_set_align(imageArmSecond, LV_ALIGN_CENTER);
    lv_obj_add_flag(imageArmSecond, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(imageArmSecond, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_img_set_pivot(imageArmSecond, 5, 115);
    lv_img_set_angle(imageArmSecond, 3150);
};

AnalogClock::~AnalogClock(){};