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
            cfg.freq_write = 16000000;
            cfg.freq_read = 16000000;
            cfg.spi_3wire = false;
            cfg.dma_channel = SPI_DMA_CH_AUTO;

            cfg.pin_sclk = LCD_SCK;
            cfg.pin_mosi = LCD_MOSI;
            cfg.pin_miso = LCD_MISO;
            cfg.pin_dc = LCD_DC;
            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }

        {
            auto cfg = _panel_instance.config();

            cfg.pin_cs = LCD_CS;
            cfg.pin_rst = LCD_RST;

            // height and width intentionally swapped, setRotation(1) is set
            cfg.panel_width = DISPLAY_HEIGHT;
            cfg.panel_height = DISPLAY_WIDTH;

            _panel_instance.config(cfg);
        }

        {
            auto cfg = _light_instance.config();

            cfg.pin_bl = LCD_BACKLIGHT;
            cfg.freq = 44100;
            cfg.pwm_channel = 7;

            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance);
        }

        {
            auto cfg = _touch_instance.config();
            //if you use ST7796s display, you must remove D1 diode and bridge the pads https://github.com/Bodmer/TFT_eSPI/issues/849
            cfg.x_min = 0;
            cfg.x_max = DISPLAY_HEIGHT - 1;
            cfg.y_min = 0;
            cfg.y_max = DISPLAY_WIDTH - 1;
            cfg.bus_shared = true;
            cfg.spi_host = VSPI_HOST;
            cfg.freq = 1000000;
            cfg.pin_sclk = TS_SCK;
            cfg.pin_mosi = TS_MOSI;
            cfg.pin_miso = TS_MISO;
            cfg.pin_cs = TS_CS;

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