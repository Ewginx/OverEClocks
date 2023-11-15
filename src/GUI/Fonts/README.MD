# HOW TO CREATE YOUR FONTS AND SYMBOLS

All fonts were created via LVGL FontConverter, available at [link](https://lvgl.io/tools/fontconverter)

Bpp was set to 4.

---
### Basic font, that used in a project

The basic font used in the project is *Montserrat-Regular.ttf* (available at Google Fonts or in the Assets folder).

You can replace it with any font that has compatible extension (TFF or WOFF) and Unicode support

All you need to do is just font file to .c array via LVGL FontConverter with the right height (px) and character/range. 

Don't forget to include the Basic Latin-1 and ° sign in the range. 
Except for 48 px font, which is only used to display the current temperature on the Weather Screen and therefore only needs ° and minus signs and all numbers

---
### Dock panel font

For WiFi connectivity symbols, you need to convert *MaterialIcons-Regular.ttf* font with this range codes:
- **0xe648** for WiFi Connected
- **0xe63e** for WiFi Disconnected

For Weather screen symbols, you need to convert 
1. *weathericons-regular-webfont.ttf* with range codes:
   - **0xf079,0xf07a,0xf07b,0xf078,0xf076,0xf0cc,0xf072,0xf048,0xf04d,0xf058,0xf044,0xf043,0xf088,0xf057,0xf087**
  
2. append to it **flaticon_temperature.ttf** with range codes:
    - **0xe900,0xe901,0xe902,0xe903** (this symbols was converted from SVG file in IcoMoon App)
3. then add **Montserrat-Regular.ttf* with code ranges of your language (you should probably add Basic Latin, at least for numbers, and ° sign)