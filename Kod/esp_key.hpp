#ifndef __ESP_KEY_HPP_
#define __ESP_KEY_HPP_


#include <stdio.h>
#include <string.h>
#include <Arduino.h>
#include "esp32_wifi.hpp"
#include "bsp_RGB.hpp"


#ifdef __cplusplus
extern "C" {
#endif

  void init_key(void);
  int key_state();
  void key_goto_state(void);


#ifdef __cplusplus
}
#endif


#endif
