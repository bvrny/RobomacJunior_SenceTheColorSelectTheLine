#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "esp32_wifi.hpp"
#include "esp_key.hpp"
#include "bsp_RGB.hpp"

#define AI_set_mode COLOR_AI

extern uint8_t cmd_flag;
void mode_chage() {
  if (runmode == Nornal_AI) {
    cmd_flag = 2;
  } else if (runmode == REFACE_AI) {
    cmd_flag = 3;
  } else if (runmode == QR_AI) {
    cmd_flag = 4;
  } else {
    cmd_flag = 5;
  }
}

//I2C driver chip addres
#define I2C_DriverAddress 0x40

void setup() {
  init_key();
  serial_init();
  SET_ESP_WIFI_MODE();
  SET_ESP_AI_MODE(AI_set_mode);
  RGB_init();
  mode_chage();
}

void loop() {
  // 1. Always process incoming serial data
  recv_data();

  // 2. Handle button modes
  if (runmode == COLOR_AI || runmode == REFACE_AI) {
    key_goto_state();
  }

  if (newlines == 1) {
    newlines = 0;
    if (esp32_ai_msg.area > 0 && esp32_ai_msg.cx != 160) {
      
    }
  }
}

void serialEvent() {
  recv_data();
}