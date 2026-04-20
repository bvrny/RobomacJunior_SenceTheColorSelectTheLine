#ifndef __BSP_RGB_HPP_
#define __BSP_RGB_HPP_


#include <stdio.h>
#include <string.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "esp32_wifi.hpp"



#ifdef __cplusplus
extern "C" {
#endif


  // Enumerate common colors
  enum ColorType {
    BLACK,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    MAGENTA,
    CYAN,
    WHITE,
    VIOLET,
  };

  void RGB_init();
  void setRGBColor(ColorType color);  //Set the color of the RGB display


#ifdef __cplusplus
}
#endif

#endif
