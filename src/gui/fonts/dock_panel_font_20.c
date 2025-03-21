/*******************************************************************************
 * Size: 20 px
 * Bpp: 4
 * Opts: 
 ******************************************************************************/

#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif

#ifndef DOCK_PANEL_FONT_20
#define DOCK_PANEL_FONT_20 1
#endif

#if DOCK_PANEL_FONT_20

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */

    /* U+0025 "%" */
    0x0, 0x9d, 0xd9, 0x0, 0x0, 0x0, 0xd5, 0x0,
    0x8, 0x90, 0x9, 0x80, 0x0, 0x7, 0xb0, 0x0,
    0xe, 0x10, 0x1, 0xe0, 0x0, 0x2e, 0x10, 0x0,
    0x2d, 0x0, 0x0, 0xd2, 0x0, 0xb6, 0x0, 0x0,
    0x2e, 0x0, 0x0, 0xe2, 0x6, 0xc0, 0x0, 0x0,
    0xe, 0x10, 0x1, 0xe0, 0x1e, 0x20, 0x0, 0x0,
    0x8, 0xb1, 0x1b, 0x80, 0xa8, 0x0, 0x0, 0x0,
    0x0, 0x8e, 0xe8, 0x4, 0xd0, 0x2b, 0xfd, 0x40,
    0x0, 0x0, 0x0, 0xe, 0x30, 0xe5, 0x3, 0xe2,
    0x0, 0x0, 0x0, 0x99, 0x6, 0xa0, 0x0, 0x88,
    0x0, 0x0, 0x3, 0xe1, 0x8, 0x70, 0x0, 0x4b,
    0x0, 0x0, 0xd, 0x50, 0x8, 0x70, 0x0, 0x4b,
    0x0, 0x0, 0x7b, 0x0, 0x6, 0xa0, 0x0, 0x78,
    0x0, 0x2, 0xe1, 0x0, 0x0, 0xe3, 0x2, 0xe2,
    0x0, 0xc, 0x60, 0x0, 0x0, 0x2c, 0xdd, 0x40,

    /* U+002D "-" */
    0x0, 0x0, 0x0, 0xcf, 0xff, 0xf7, 0x33, 0x33,
    0x31,

    /* U+002E "." */
    0x12, 0xd, 0xf1, 0xad, 0x0,

    /* U+0030 "0" */
    0x0, 0x8, 0xdf, 0xea, 0x20, 0x0, 0x1, 0xdf,
    0x86, 0x7d, 0xf3, 0x0, 0xb, 0xe2, 0x0, 0x0,
    0xce, 0x10, 0x3f, 0x50, 0x0, 0x0, 0x1f, 0x70,
    0x9e, 0x0, 0x0, 0x0, 0xa, 0xd0, 0xca, 0x0,
    0x0, 0x0, 0x7, 0xf0, 0xe8, 0x0, 0x0, 0x0,
    0x4, 0xf2, 0xf8, 0x0, 0x0, 0x0, 0x4, 0xf3,
    0xe8, 0x0, 0x0, 0x0, 0x4, 0xf2, 0xca, 0x0,
    0x0, 0x0, 0x7, 0xf0, 0x9e, 0x0, 0x0, 0x0,
    0xa, 0xd0, 0x3f, 0x50, 0x0, 0x0, 0x1f, 0x70,
    0xb, 0xe2, 0x0, 0x0, 0xce, 0x10, 0x1, 0xdf,
    0x96, 0x7e, 0xf3, 0x0, 0x0, 0x8, 0xdf, 0xea,
    0x20, 0x0,

    /* U+0031 "1" */
    0xdf, 0xff, 0xe4, 0x55, 0xae, 0x0, 0x8, 0xe0,
    0x0, 0x8e, 0x0, 0x8, 0xe0, 0x0, 0x8e, 0x0,
    0x8, 0xe0, 0x0, 0x8e, 0x0, 0x8, 0xe0, 0x0,
    0x8e, 0x0, 0x8, 0xe0, 0x0, 0x8e, 0x0, 0x8,
    0xe0, 0x0, 0x8e, 0x0, 0x8, 0xe0,

    /* U+0032 "2" */
    0x0, 0x7c, 0xff, 0xea, 0x20, 0x2, 0xdf, 0x96,
    0x68, 0xef, 0x20, 0x4c, 0x10, 0x0, 0x1, 0xea,
    0x0, 0x0, 0x0, 0x0, 0x9, 0xe0, 0x0, 0x0,
    0x0, 0x0, 0x8e, 0x0, 0x0, 0x0, 0x0, 0xc,
    0xb0, 0x0, 0x0, 0x0, 0x4, 0xf5, 0x0, 0x0,
    0x0, 0x2, 0xea, 0x0, 0x0, 0x0, 0x2, 0xec,
    0x0, 0x0, 0x0, 0x1, 0xdc, 0x0, 0x0, 0x0,
    0x1, 0xdd, 0x10, 0x0, 0x0, 0x1, 0xdd, 0x10,
    0x0, 0x0, 0x0, 0xce, 0x10, 0x0, 0x0, 0x0,
    0xcf, 0x75, 0x55, 0x55, 0x53, 0x3f, 0xff, 0xff,
    0xff, 0xff, 0x90,

    /* U+0033 "3" */
    0x3f, 0xff, 0xff, 0xff, 0xfc, 0x1, 0x55, 0x55,
    0x55, 0x8f, 0x70, 0x0, 0x0, 0x0, 0xc, 0xc0,
    0x0, 0x0, 0x0, 0x8, 0xf2, 0x0, 0x0, 0x0,
    0x3, 0xf6, 0x0, 0x0, 0x0, 0x0, 0xdc, 0x0,
    0x0, 0x0, 0x0, 0x8f, 0x20, 0x0, 0x0, 0x0,
    0xf, 0xff, 0xb4, 0x0, 0x0, 0x0, 0x33, 0x5b,
    0xf6, 0x0, 0x0, 0x0, 0x0, 0xa, 0xf1, 0x0,
    0x0, 0x0, 0x0, 0x3f, 0x40, 0x0, 0x0, 0x0,
    0x4, 0xf3, 0x67, 0x0, 0x0, 0x0, 0xbf, 0x6,
    0xfe, 0x96, 0x68, 0xdf, 0x50, 0x2, 0x8d, 0xff,
    0xea, 0x30, 0x0,

    /* U+0034 "4" */
    0x0, 0x0, 0x0, 0x2, 0xf7, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xdb, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x9e, 0x10, 0x0, 0x0, 0x0, 0x0, 0x5f, 0x40,
    0x0, 0x0, 0x0, 0x0, 0x2e, 0x90, 0x0, 0x0,
    0x0, 0x0, 0xc, 0xc0, 0x0, 0x0, 0x0, 0x0,
    0x8, 0xf2, 0x0, 0x2a, 0x10, 0x0, 0x4, 0xf6,
    0x0, 0x4, 0xf2, 0x0, 0x1, 0xea, 0x0, 0x0,
    0x4f, 0x20, 0x0, 0xbe, 0x21, 0x11, 0x15, 0xf3,
    0x10, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
    0x44, 0x44, 0x44, 0x47, 0xf5, 0x43, 0x0, 0x0,
    0x0, 0x0, 0x4f, 0x20, 0x0, 0x0, 0x0, 0x0,
    0x4, 0xf2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4f,
    0x20, 0x0,

    /* U+0035 "5" */
    0x0, 0xef, 0xff, 0xff, 0xfc, 0x0, 0xf, 0x95,
    0x55, 0x55, 0x40, 0x1, 0xf4, 0x0, 0x0, 0x0,
    0x0, 0x3f, 0x20, 0x0, 0x0, 0x0, 0x4, 0xf1,
    0x0, 0x0, 0x0, 0x0, 0x6f, 0x11, 0x0, 0x0,
    0x0, 0x8, 0xff, 0xff, 0xeb, 0x40, 0x0, 0x24,
    0x44, 0x47, 0xcf, 0x80, 0x0, 0x0, 0x0, 0x0,
    0x9f, 0x20, 0x0, 0x0, 0x0, 0x1, 0xf7, 0x0,
    0x0, 0x0, 0x0, 0xf, 0x80, 0x0, 0x0, 0x0,
    0x1, 0xf6, 0x3c, 0x20, 0x0, 0x0, 0xaf, 0x13,
    0xef, 0xb7, 0x67, 0xdf, 0x60, 0x0, 0x7c, 0xef,
    0xeb, 0x40, 0x0,

    /* U+0036 "6" */
    0x0, 0x5, 0xbe, 0xfe, 0xb4, 0x0, 0xa, 0xfa,
    0x64, 0x6a, 0x60, 0x9, 0xf3, 0x0, 0x0, 0x0,
    0x2, 0xf6, 0x0, 0x0, 0x0, 0x0, 0x8e, 0x0,
    0x0, 0x0, 0x0, 0xc, 0xa0, 0x0, 0x0, 0x0,
    0x0, 0xe8, 0x7, 0xdf, 0xfc, 0x50, 0xf, 0x8c,
    0xc5, 0x35, 0xaf, 0x80, 0xee, 0xb0, 0x0, 0x0,
    0x7f, 0x2d, 0xf2, 0x0, 0x0, 0x0, 0xf7, 0xaf,
    0x0, 0x0, 0x0, 0xe, 0x85, 0xf3, 0x0, 0x0,
    0x0, 0xf7, 0xd, 0xb0, 0x0, 0x0, 0x8f, 0x20,
    0x2e, 0xd6, 0x45, 0xbf, 0x60, 0x0, 0x19, 0xdf,
    0xfb, 0x40, 0x0,

    /* U+0037 "7" */
    0x5f, 0xff, 0xff, 0xff, 0xff, 0xe5, 0xf6, 0x55,
    0x55, 0x55, 0xeb, 0x5f, 0x10, 0x0, 0x0, 0x3f,
    0x55, 0xf1, 0x0, 0x0, 0xa, 0xe0, 0x2, 0x0,
    0x0, 0x1, 0xf7, 0x0, 0x0, 0x0, 0x0, 0x7f,
    0x10, 0x0, 0x0, 0x0, 0xe, 0xa0, 0x0, 0x0,
    0x0, 0x5, 0xf3, 0x0, 0x0, 0x0, 0x0, 0xcc,
    0x0, 0x0, 0x0, 0x0, 0x2f, 0x60, 0x0, 0x0,
    0x0, 0x9, 0xe0, 0x0, 0x0, 0x0, 0x1, 0xf8,
    0x0, 0x0, 0x0, 0x0, 0x7f, 0x20, 0x0, 0x0,
    0x0, 0xd, 0xb0, 0x0, 0x0, 0x0, 0x4, 0xf4,
    0x0, 0x0, 0x0,

    /* U+0038 "8" */
    0x0, 0x5, 0xcf, 0xfe, 0xa3, 0x0, 0x0, 0x9f,
    0xa5, 0x46, 0xcf, 0x50, 0x3, 0xf7, 0x0, 0x0,
    0xb, 0xe0, 0x7, 0xf0, 0x0, 0x0, 0x4, 0xf3,
    0x8, 0xf0, 0x0, 0x0, 0x3, 0xf4, 0x5, 0xf3,
    0x0, 0x0, 0x7, 0xf1, 0x0, 0xce, 0x40, 0x0,
    0x7f, 0x80, 0x0, 0x1e, 0xff, 0xff, 0xfb, 0x0,
    0x2, 0xed, 0x63, 0x33, 0x7e, 0xc0, 0xc, 0xd0,
    0x0, 0x0, 0x2, 0xf7, 0xf, 0x70, 0x0, 0x0,
    0x0, 0xbc, 0xf, 0x80, 0x0, 0x0, 0x0, 0xcb,
    0xb, 0xe1, 0x0, 0x0, 0x4, 0xf7, 0x2, 0xee,
    0x85, 0x45, 0xaf, 0xc0, 0x0, 0x18, 0xdf, 0xfe,
    0xb6, 0x0,

    /* U+0039 "9" */
    0x0, 0x2a, 0xef, 0xea, 0x20, 0x0, 0x4, 0xfc,
    0x64, 0x6c, 0xf4, 0x0, 0xe, 0xb0, 0x0, 0x0,
    0x9e, 0x10, 0x3f, 0x30, 0x0, 0x0, 0x1f, 0x80,
    0x5f, 0x10, 0x0, 0x0, 0xd, 0xd0, 0x4f, 0x20,
    0x0, 0x0, 0xf, 0xf0, 0x1f, 0x90, 0x0, 0x0,
    0x7f, 0xf1, 0x7, 0xf9, 0x21, 0x28, 0xf7, 0xf2,
    0x0, 0x6e, 0xff, 0xfc, 0x35, 0xf1, 0x0, 0x0,
    0x23, 0x10, 0x7, 0xf0, 0x0, 0x0, 0x0, 0x0,
    0xb, 0xb0, 0x0, 0x0, 0x0, 0x0, 0x2f, 0x50,
    0x0, 0x0, 0x0, 0x2, 0xdc, 0x0, 0x3, 0xb6,
    0x55, 0x9f, 0xc1, 0x0, 0x3, 0xae, 0xff, 0xc6,
    0x0, 0x0,

    /* U+0043 "C" */
    0x0, 0x0, 0x7c, 0xff, 0xeb, 0x50, 0x0, 0x3,
    0xef, 0xa7, 0x57, 0xbf, 0xc1, 0x3, 0xfc, 0x20,
    0x0, 0x0, 0x3d, 0x20, 0xdd, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x6f, 0x30, 0x0, 0x0, 0x0, 0x0,
    0xb, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe9,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0x80, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xe9, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xb, 0xc0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x6f, 0x30, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xdd, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xfc,
    0x20, 0x0, 0x0, 0x4e, 0x30, 0x4, 0xef, 0xa7,
    0x67, 0xbf, 0xb0, 0x0, 0x0, 0x7c, 0xff, 0xeb,
    0x50, 0x0,

    /* U+0046 "F" */
    0xbf, 0xff, 0xff, 0xff, 0xfe, 0xbd, 0x55, 0x55,
    0x55, 0x54, 0xbc, 0x0, 0x0, 0x0, 0x0, 0xbc,
    0x0, 0x0, 0x0, 0x0, 0xbc, 0x0, 0x0, 0x0,
    0x0, 0xbc, 0x0, 0x0, 0x0, 0x0, 0xbc, 0x0,
    0x0, 0x0, 0x0, 0xbc, 0x11, 0x11, 0x11, 0x10,
    0xbf, 0xff, 0xff, 0xff, 0xf0, 0xbd, 0x44, 0x44,
    0x44, 0x40, 0xbc, 0x0, 0x0, 0x0, 0x0, 0xbc,
    0x0, 0x0, 0x0, 0x0, 0xbc, 0x0, 0x0, 0x0,
    0x0, 0xbc, 0x0, 0x0, 0x0, 0x0, 0xbc, 0x0,
    0x0, 0x0, 0x0,

    /* U+00B0 "°" */
    0x6, 0xde, 0xa1, 0x6, 0xb1, 0x6, 0xc0, 0xd1,
    0x0, 0xa, 0x3e, 0x0, 0x0, 0x85, 0xd1, 0x0,
    0xb, 0x36, 0xb1, 0x6, 0xc0, 0x6, 0xde, 0x91,
    0x0,

    /* U+F013 "" */
    0x0, 0x0, 0x0, 0x0, 0x2, 0x20, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x6, 0xff, 0xff,
    0x60, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8,
    0xff, 0xff, 0x80, 0x0, 0x0, 0x0, 0x0, 0x1,
    0x0, 0x2b, 0xff, 0xff, 0xb2, 0x0, 0x10, 0x0,
    0x0, 0x8f, 0x87, 0xff, 0xff, 0xff, 0xff, 0x79,
    0xf8, 0x0, 0x4, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x40, 0xc, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x2f, 0xff,
    0xff, 0xff, 0xc7, 0x7c, 0xff, 0xff, 0xff, 0xf2,
    0x7, 0xff, 0xff, 0xfa, 0x0, 0x0, 0xaf, 0xff,
    0xff, 0x70, 0x0, 0x6f, 0xff, 0xf1, 0x0, 0x0,
    0x1f, 0xff, 0xf6, 0x0, 0x0, 0x7f, 0xff, 0xe0,
    0x0, 0x0, 0xe, 0xff, 0xf7, 0x0, 0x0, 0x6f,
    0xff, 0xf1, 0x0, 0x0, 0x1f, 0xff, 0xf6, 0x0,
    0x7, 0xff, 0xff, 0xfa, 0x0, 0x0, 0xaf, 0xff,
    0xff, 0x70, 0x2f, 0xff, 0xff, 0xff, 0xc7, 0x7c,
    0xff, 0xff, 0xff, 0xf2, 0xc, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x4, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x40,
    0x0, 0x8f, 0x97, 0xff, 0xff, 0xff, 0xff, 0x78,
    0xf8, 0x0, 0x0, 0x1, 0x0, 0x1b, 0xff, 0xff,
    0xb1, 0x0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x8,
    0xff, 0xff, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x6, 0xff, 0xff, 0x60, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x2, 0x20, 0x0, 0x0,
    0x0, 0x0,

    /* U+F015 "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x46, 0x10, 0x0,
    0x67, 0x60, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x9f, 0xfe, 0x20, 0xf, 0xff, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x1, 0xbf, 0xff, 0xff, 0x51, 0xff,
    0xf1, 0x0, 0x0, 0x0, 0x0, 0x2, 0xdf, 0xfc,
    0x8f, 0xff, 0x8f, 0xff, 0x10, 0x0, 0x0, 0x0,
    0x4, 0xff, 0xfa, 0x0, 0x4e, 0xff, 0xff, 0xf1,
    0x0, 0x0, 0x0, 0x7, 0xff, 0xf7, 0x8, 0xd3,
    0x2d, 0xff, 0xff, 0x10, 0x0, 0x0, 0xa, 0xff,
    0xf5, 0x1b, 0xff, 0xf5, 0xb, 0xff, 0xf4, 0x0,
    0x0, 0x1c, 0xff, 0xe2, 0x2d, 0xff, 0xff, 0xf7,
    0x8, 0xff, 0xf6, 0x0, 0x3e, 0xff, 0xc1, 0x4e,
    0xff, 0xff, 0xff, 0xfa, 0x5, 0xff, 0xf9, 0xe,
    0xff, 0x90, 0x6f, 0xff, 0xff, 0xff, 0xff, 0xfc,
    0x13, 0xef, 0xf6, 0x4f, 0x70, 0x9f, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xfe, 0x31, 0xcc, 0x0, 0x0,
    0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb,
    0x0, 0x0, 0x0, 0x4, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x4f,
    0xff, 0xff, 0x80, 0x1, 0xef, 0xff, 0xfc, 0x0,
    0x0, 0x0, 0x4, 0xff, 0xff, 0xf6, 0x0, 0xe,
    0xff, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x4f, 0xff,
    0xff, 0x60, 0x0, 0xef, 0xff, 0xfc, 0x0, 0x0,
    0x0, 0x4, 0xff, 0xff, 0xf6, 0x0, 0xe, 0xff,
    0xff, 0xc0, 0x0, 0x0, 0x0, 0x2f, 0xff, 0xff,
    0x40, 0x0, 0xcf, 0xff, 0xfa, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0,

    /* U+F0E7 "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x17, 0x10, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3, 0xef, 0xa0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x4f, 0xff, 0x80, 0x0,
    0x0, 0x0, 0x0, 0x6, 0xff, 0xff, 0x10, 0x0,
    0x0, 0x0, 0x0, 0x8f, 0xff, 0xfa, 0x0, 0x0,
    0x0, 0x0, 0xa, 0xff, 0xff, 0xf4, 0x0, 0x0,
    0x0, 0x1, 0xbf, 0xff, 0xff, 0xd0, 0x0, 0x0,
    0x0, 0x1d, 0xff, 0xff, 0xff, 0x60, 0x0, 0x0,
    0x3, 0xef, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0,
    0x3f, 0xff, 0xff, 0xff, 0xfe, 0xbb, 0xbb, 0x80,
    0xaf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3,
    0x3b, 0xbb, 0xbc, 0xff, 0xff, 0xff, 0xff, 0xb0,
    0x0, 0x0, 0x7, 0xff, 0xff, 0xff, 0xf9, 0x0,
    0x0, 0x0, 0xd, 0xff, 0xff, 0xff, 0x70, 0x0,
    0x0, 0x0, 0x4f, 0xff, 0xff, 0xf5, 0x0, 0x0,
    0x0, 0x0, 0xbf, 0xff, 0xfe, 0x40, 0x0, 0x0,
    0x0, 0x2, 0xff, 0xff, 0xe2, 0x0, 0x0, 0x0,
    0x0, 0x9, 0xff, 0xfc, 0x10, 0x0, 0x0, 0x0,
    0x0, 0xf, 0xff, 0xb0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x2f, 0xf9, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x5, 0x50, 0x0, 0x0, 0x0, 0x0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 84, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 265, .box_w = 16, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 120, .adv_w = 122, .box_w = 6, .box_h = 3, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 129, .adv_w = 68, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 134, .adv_w = 212, .box_w = 12, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 224, .adv_w = 116, .box_w = 5, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 262, .adv_w = 182, .box_w = 11, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 345, .adv_w = 180, .box_w = 11, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 428, .adv_w = 212, .box_w = 13, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 526, .adv_w = 181, .box_w = 11, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 609, .adv_w = 195, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 692, .adv_w = 188, .box_w = 11, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 775, .adv_w = 204, .box_w = 12, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 865, .adv_w = 195, .box_w = 12, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 955, .adv_w = 230, .box_w = 13, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1053, .adv_w = 203, .box_w = 10, .box_h = 15, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1128, .adv_w = 134, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 8},
    {.bitmap_index = 1153, .adv_w = 320, .box_w = 20, .box_h = 21, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1363, .adv_w = 360, .box_w = 23, .box_h = 19, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1582, .adv_w = 280, .box_w = 16, .box_h = 21, .ofs_x = 1, .ofs_y = -3}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x5, 0xd, 0xe, 0x10, 0x11, 0x12, 0x13,
    0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x23, 0x26,
    0x90, 0xeff3, 0xeff5, 0xf0c7
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 61640, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 20, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 4,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t dock_panel_font_20 = {
#else
lv_font_t dock_panel_font_20 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 21,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
    .fallback = NULL,
    .user_data = NULL,
};



#endif /*#if DOCK_PANEL_FONT_20*/

