#pragma once
#include "Config/Config.h"
#include <LovyanGFX.hpp>
#include <lvgl.h>


class LGFX : public lgfx::LGFX_Device {

    lgfx::Panel_ST7796 _panel_instance;

    lgfx::Bus_SPI _bus_instance;

    lgfx::Light_PWM _light_instance;

    lgfx::Touch_XPT2046 _touch_instance;

  public:
    LGFX(void) {
        {
            auto cfg = _bus_instance.config();

            cfg.spi_host = VSPI_HOST;

            cfg.spi_mode = 0;
            cfg.freq_write = 80000000;
            cfg.freq_read = 16000000;
            cfg.spi_3wire = false;
            cfg.use_lock = true;
            cfg.dma_channel = SPI_DMA_CH_AUTO;

            cfg.pin_sclk = 18;
            cfg.pin_mosi = 23;
            cfg.pin_miso = 19;
            cfg.pin_dc = 2;
            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }

        {
            auto cfg = _panel_instance.config();

            cfg.pin_cs = 5;
            cfg.pin_rst = 15;
            cfg.pin_busy = -1;

            cfg.panel_width = 320;
            cfg.panel_height = 480;
            cfg.offset_x = 0;
            cfg.offset_y = 0;
            cfg.offset_rotation = 0;
            cfg.dummy_read_pixel = 8;
            cfg.dummy_read_bits = 1;
            cfg.readable = true;
            cfg.invert = false;
            cfg.rgb_order = false;
            cfg.dlen_16bit = false;
            cfg.bus_shared = true;

            _panel_instance.config(cfg);
        }

        {
            auto cfg = _light_instance.config();

            cfg.pin_bl = 32;
            cfg.invert = false;
            cfg.freq = 44100;
            cfg.pwm_channel = 7;

            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance);
        }

        {
            auto cfg = _touch_instance.config();

            cfg.x_min = 0;
            cfg.x_max = 319;
            cfg.y_min = 0;
            cfg.y_max = 479;
            cfg.pin_int = 38;
            cfg.bus_shared = true;
            cfg.offset_rotation = 0;

            cfg.spi_host = VSPI_HOST;
            cfg.freq = 1000000;
            cfg.pin_sclk = 18;
            cfg.pin_mosi = 23;
            cfg.pin_miso = 19;
            cfg.pin_cs = 4;
            _touch_instance.config(cfg);
            _panel_instance.setTouch(&_touch_instance);
        }

        setPanel(&_panel_instance);
    }
};

class Display {
  private:
    LGFX *tft;

    void init_tft();
    void init_lvgl();

  public:
    Display();
    ~Display();
    void set_brightness(uint8_t value);
    int32_t get_brightness();
    void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
    void touch_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
};