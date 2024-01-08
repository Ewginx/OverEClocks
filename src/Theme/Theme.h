#ifndef THEME_H
#define THEME_H

#ifdef __cplusplus
extern "C" {
#endif

#pragma once
#include "lvgl.h"

#define MODE_DARK 1
#define RADIUS_DEFAULT                                                                   \
    (disp_size == DISP_LARGE ? lv_disp_dpx(theme.disp, 12) : lv_disp_dpx(theme.disp, 8))
/*SCREEN*/
#define LIGHT_COLOR_SCR lv_palette_lighten(LV_PALETTE_GREY, 4)
#define LIGHT_COLOR_CARD lv_color_white()
#define LIGHT_COLOR_TEXT lv_palette_darken(LV_PALETTE_GREY, 4)
#define LIGHT_COLOR_GREY lv_palette_lighten(LV_PALETTE_GREY, 2)

#define DARK_COLOR_SCR lv_color_hex(0x15171A)
#define DARK_COLOR_CARD lv_color_hex(0x282b30)
#define DARK_COLOR_TEXT lv_palette_lighten(LV_PALETTE_GREY, 5)
#define DARK_COLOR_GREY lv_color_hex(0x2f3237)

#define TRANSITION_TIME 80
#define BORDER_WIDTH lv_disp_dpx(theme.disp, 2)
#define OUTLINE_WIDTH lv_disp_dpx(theme.disp, 3)

#define PAD_DEF                                                                          \
    (disp_size == DISP_LARGE    ? lv_disp_dpx(theme.disp, 24)                            \
     : disp_size == DISP_MEDIUM ? lv_disp_dpx(theme.disp, 20)                            \
                                : lv_disp_dpx(theme.disp, 16))
#define PAD_SMALL                                                                        \
    (disp_size == DISP_LARGE    ? lv_disp_dpx(theme.disp, 14)                            \
     : disp_size == DISP_MEDIUM ? lv_disp_dpx(theme.disp, 12)                            \
                                : lv_disp_dpx(theme.disp, 10))
#define PAD_TINY                                                                         \
    (disp_size == DISP_LARGE    ? lv_disp_dpx(theme.disp, 8)                             \
     : disp_size == DISP_MEDIUM ? lv_disp_dpx(theme.disp, 6)                             \
                                : lv_disp_dpx(theme.disp, 2))

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Initialize the theme
 * @param color_primary the primary color of the theme
 * @param color_secondary the secondary color for the theme
 * @param font pointer to a font to use.
 * @return a pointer to reference this theme later
 */
lv_theme_t *theme_custom_init(lv_disp_t *disp, lv_color_t color_primary,
                              lv_color_t color_secondary, lv_color_t color_screen,
                              lv_color_t color_card, lv_color_t color_text,
                              lv_color_t color_grey, bool dark, const lv_font_t *font);

/**
 * Get default theme
 * @return a pointer to default theme, or NULL if this is not initialized
 */
lv_theme_t *theme_custom_get(void);

/**
 * Check if default theme is initialized
 * @return true if default theme is initialized, false otherwise
 */
bool theme_custom_is_inited(void);

#endif

#ifdef __cplusplus
} /*extern "C"*/
#endif
