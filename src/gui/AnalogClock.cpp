#include "AnalogClock.h"

AnalogClock::AnalogClock() {
    this->createAnalogClockWidgets();
    this->setDefaultValues();
}

void AnalogClock::createAnalogClockWidgets() {
    analogClockScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(analogClockScreen, LV_OBJ_FLAG_SCROLLABLE);

    analogClockPanel = lv_obj_create(analogClockScreen);
    lv_obj_set_size(analogClockPanel, lv_pct(100), lv_pct(100));
    lv_obj_set_align(analogClockPanel, LV_ALIGN_TOP_LEFT);
    lv_obj_clear_flag(analogClockPanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(analogClockPanel, LV_OBJ_FLAG_EVENT_BUBBLE);

    watchface = lv_img_create(analogClockScreen);
    lv_obj_set_width(watchface, LV_SIZE_CONTENT);
    lv_obj_set_height(watchface, LV_SIZE_CONTENT);
    lv_obj_set_align(watchface, LV_ALIGN_CENTER);
    lv_obj_set_y(watchface, 15);
    lv_obj_add_flag(watchface, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(watchface, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(watchface, LV_OBJ_FLAG_EVENT_BUBBLE);

    imageArmHour = lv_img_create(analogClockScreen);
    lv_obj_set_width(imageArmHour, LV_SIZE_CONTENT);
    lv_obj_set_height(imageArmHour, LV_SIZE_CONTENT);
    lv_obj_set_x(imageArmHour, 0);
    lv_obj_set_y(imageArmHour, -20);
    lv_obj_set_align(imageArmHour, LV_ALIGN_CENTER);
    lv_obj_add_flag(imageArmHour, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(imageArmHour, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(imageArmHour, LV_OBJ_FLAG_EVENT_BUBBLE);

    imageArmMinute = lv_img_create(analogClockScreen);
    lv_obj_set_width(imageArmMinute, LV_SIZE_CONTENT);
    lv_obj_set_height(imageArmMinute, LV_SIZE_CONTENT);
    lv_obj_set_x(imageArmMinute, 0);
    lv_obj_set_y(imageArmMinute, -34);
    lv_obj_set_align(imageArmMinute, LV_ALIGN_CENTER);
    lv_obj_add_flag(imageArmMinute, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(imageArmMinute, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(imageArmMinute, LV_OBJ_FLAG_EVENT_BUBBLE);

    imageArmSecond = lv_img_create(analogClockScreen);
    lv_obj_set_width(imageArmSecond, LV_SIZE_CONTENT);
    lv_obj_set_height(imageArmSecond, LV_SIZE_CONTENT);
    lv_obj_set_x(imageArmSecond, 0);
    lv_obj_set_y(imageArmSecond, -32);
    lv_obj_set_align(imageArmSecond, LV_ALIGN_CENTER);
    lv_obj_add_flag(imageArmSecond, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(imageArmSecond, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(imageArmSecond, LV_OBJ_FLAG_EVENT_BUBBLE);
};

void AnalogClock::setTime(int hour, int minute, int second) {
    int angle = 0;
    angle = second * 60;
    lv_img_set_angle(this->imageArmSecond, angle);
    angle = minute * 60;
    lv_img_set_angle(this->imageArmMinute, angle);
    angle = hour * 300 + (int)minute / 2 * 10;
    lv_img_set_angle(this->imageArmHour, angle);
}

void AnalogClock::setAnalogClockImages() {
    lv_img_set_src(this->watchface, "F:/analog_clock/watchface.bin");
    lv_img_set_src(imageArmSecond, "F:/analog_clock/armsecond.png");
    lv_img_set_src(imageArmHour, "F:/analog_clock/armhour.png");
    lv_img_set_src(imageArmMinute, "F:/analog_clock/armminute.png");
    lv_img_cache_invalidate_src("F:/analog_clock/watchface.bin");
    lv_img_cache_invalidate_src("F:/analog_clock/armsecond.png");
    lv_img_cache_invalidate_src("F:/analog_clock/armhour.png");
    lv_img_cache_invalidate_src("F:/analog_clock/armminute.png");
    lv_img_set_pivot(imageArmHour, 9, 77);
    lv_img_set_pivot(imageArmMinute, 9, 105);
    lv_img_set_pivot(imageArmSecond, 5, 115);
}

void AnalogClock::setDefaultValues() { this->setTime(0, 0, 0); }

AnalogClock::~AnalogClock() {}
