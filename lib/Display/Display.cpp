#include "Display.h"

static Display *instance = NULL;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[DISPLAY_WIDTH * 10 ];

Display::Display()
{
    instance = this;
    tft = new LGFX();
    this->init_tft();
}

Display::~Display()
{
    delete tft;
}

void Display::init_tft()
{
    tft->begin();
    tft->setRotation(1);
    tft->fillScreen(TFT_BLACK);
    tft->setBrightness(255);
    uint16_t calData[] = {210, 3946, 198, 249, 3884, 3929, 3871, 278};
    tft->setTouchCalibrate(calData);
    this->init_lvgl();
}
extern "C" void disp_flush_c(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p) {
  instance->disp_flush(drv, area, color_p);
}

extern "C" void touch_read_c(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  instance->touch_read(indev_driver, data);
}
void Display::init_lvgl()
{
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, DISPLAY_WIDTH * 10);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );

    disp_drv.hor_res = DISPLAY_WIDTH;
    disp_drv.ver_res = DISPLAY_HEIGHT;
    disp_drv.flush_cb = disp_flush_c;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_read_c;
    lv_indev_drv_register( &indev_drv );
}

void Display::disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft->startWrite();
  tft->setAddrWindow(area->x1, area->y1, w, h);
  tft->writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
  tft->endWrite();

  lv_disp_flush_ready(disp);
}

void Display::touch_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  uint16_t touchX, touchY;
  bool touched = tft->getTouch(&touchX, &touchY);
  if (!touched) {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;
    data->point.x = touchX;
    data->point.y = touchY;
  }
}

void Display::set_brightness(uint8_t value){
  this->tft->setBrightness(value);
}