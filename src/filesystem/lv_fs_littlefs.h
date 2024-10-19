#pragma once
#include "Config/Config.h"
#include <Arduino.h>
#include <LittleFS.h>
#include <lvgl.h>

 //https://github.com/100askTeam/esp-arduino-learn/blob/master/examples/08_integrated/01_lcd_sd_card_fc_joypad_fs_lv_lib_100ask/01_lcd_sd_card_fc_joypad_fs_lv_lib_100ask.ino

void fs_init_littlefs(void);
void *littlefs_fs_open(lv_fs_drv_t *drv, const char *path, lv_fs_mode_t mode);
lv_fs_res_t littlefs_fs_close(lv_fs_drv_t *drv, void *file_p);
lv_fs_res_t littlefs_fs_read(lv_fs_drv_t *drv, void *file_p, void *fileBuf, uint32_t btr,
                       uint32_t *br);
lv_fs_res_t littlefs_fs_write(lv_fs_drv_t *drv, void *file_p, const void *buf, uint32_t btw,
                        uint32_t *bw);
lv_fs_res_t littlefs_fs_seek(lv_fs_drv_t *drv, void *file_p, uint32_t pos,
                       lv_fs_whence_t whence);
lv_fs_res_t littlefs_fs_tell(lv_fs_drv_t *drv, void *file_p, uint32_t *pos_p);
void *littlefs_dir_open(lv_fs_drv_t *drv, const char *dirpath);
lv_fs_res_t littlefs_dir_read(lv_fs_drv_t *drv, void *dir_p, char *fn);
lv_fs_res_t littlefs_dir_close(lv_fs_drv_t *drv, void *dir_p);
void lv_port_littlefs_fs_init(void);