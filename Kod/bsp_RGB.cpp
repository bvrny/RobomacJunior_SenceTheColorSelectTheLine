#include "bsp_RGB.hpp"

// Define RGB control pins and quantity
#define RGB_PIN 6
#define RGB_NUM 1

// Create an instance of the Adafruit_NeoPixel class
Adafruit_NeoPixel RGB = Adafruit_NeoPixel(RGB_NUM, RGB_PIN, NEO_GRB + NEO_KHZ800);

void RGB_init() {
  RGB.begin();  // RGB Initialize RGB
  RGB.show();   // Refresh RGB display
  if (runmode == COLOR_AI || runmode == REFACE_AI) {
    setRGBColor(YELLOW);

  } else {
    setRGBColor(BLACK);
  }
}

/**
 * @brief Set RGB display color
 * @param color: Set the color
 * @retval None
 */
void setRGBColor(ColorType color) {
  switch (color) {
    case RED:
      RGB.setPixelColor(0, RGB.Color(255, 0, 0));
      RGB.show();
      break;
    case GREEN:
      RGB.setPixelColor(0, RGB.Color(0, 255, 0));
      RGB.show();
      break;
    case BLUE:
      RGB.setPixelColor(0, RGB.Color(0, 0, 255));
      RGB.show();
      break;
    case YELLOW:
      RGB.setPixelColor(0, RGB.Color(255, 255, 0));
      RGB.show();
      break;
    case MAGENTA:
      RGB.setPixelColor(0, RGB.Color(255, 0, 255));
      RGB.show();
      break;
    case CYAN:
      RGB.setPixelColor(0, RGB.Color(0, 255, 255));
      RGB.show();
      break;
    case WHITE:
      RGB.setPixelColor(0, RGB.Color(255, 255, 255));
      RGB.show();
      break;
    case VIOLET:
      RGB.setPixelColor(0, RGB.Color(75, 125, 255));
      RGB.show();
      break;
    default:
      RGB.setPixelColor(0, RGB.Color(0, 0, 0));
      RGB.show();
      break;
  }
}
