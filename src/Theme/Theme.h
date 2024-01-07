#ifndef THEME_H
#define THEME_H

#ifdef __cplusplus
extern "C" {
#endif


#pragma once
#include "lvgl.h"


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
lv_theme_t * lv_theme_default_init(lv_disp_t * disp, lv_color_t color_primary, lv_color_t color_secondary, bool dark,
                                   const lv_font_t * font);

/**
 * Get default theme
 * @return a pointer to default theme, or NULL if this is not initialized
 */
lv_theme_t * lv_theme_default_get(void);

/**
 * Check if default theme is initialized
 * @return true if default theme is initialized, false otherwise
 */
bool lv_theme_default_is_inited(void);


#endif

#ifdef __cplusplus
} /*extern "C"*/
#endif

