# How to use weather icons

#### Now the weather icons are stored on the flash memory of the ESP32 (spiffs partition).

- the icons are 120x120
- since flash memory is limited duplicates have been removed from the icons (icons with the same image are counted as one icon)
- icons have been converted to .bin format on the [LVGL website](https://lvgl.io/tools/imageconverter)
- color format CF_INDEXED_4_BIT

The currently used icons are in the `icons_bin_duplicate_free` folder.
To load them into ESP32, you need to add them to the `data` folder (create one if it doesn't exist) in the root of the project.

The final path will look like this: `data/icons/night/your_icons` (`data/icons/day/your_icons`)
