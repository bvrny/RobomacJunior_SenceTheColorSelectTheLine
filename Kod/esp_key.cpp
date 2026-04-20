#include "esp_key.hpp"
#include <avr/sleep.h>

#define KEY_PIN 7
const int Press_KEY = 1;
const int Release_KEY = 0;

int touchState = LOW;               
int lastTouchState = LOW;           
unsigned long touchDownTime = 0;     
bool touchInProgress = false;       
bool singleClick = false;            
bool longPress = false;              
unsigned long debounceDelay = 1;     
unsigned long longPressDelay = 1000;
unsigned long lastDebounceTime = 0;  

void init_key(void) {
  pinMode(KEY_PIN, INPUT_PULLUP);  
}

int key_state() {
  int touchReading = digitalRead(KEY_PIN);
  int8_t key_state = 0;
  if (touchReading != lastTouchState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (touchReading != touchState) {
      touchState = touchReading;

      if (touchState == LOW) {
        touchDownTime = millis();
        touchInProgress = true;
        singleClick = false;
        longPress = false;
      } else {
        if (touchInProgress) {
          if ((millis() - touchDownTime) < longPressDelay) {
            if (!singleClick) {
              singleClick = true;
            }
          } else {
            longPress = true;
          }
          touchInProgress = false;
        }
      }
    }
  }

  if (singleClick) {
    //ESPWIFISerial.println("1");
    key_state = 1;
    singleClick = false;
  }
  if (longPress) {
    //ESPWIFISerial.println("2");
    key_state = 2;
    longPress = false;
  }

  lastTouchState = touchReading;

  return key_state;
}


uint8_t Virtual_key = 1; 
static void send_key(void) {
  switch (Virtual_key) {
    case 1: ESPWIFISerial.print("KEY_MENU"); break; 
    case 2: ESPWIFISerial.print("KEY_PLAY"); break;  
    case 3: ESPWIFISerial.print("KEY_DOWN"); break;  
    case 4: ESPWIFISerial.print("KEY_PLAY"); break; 
  }
}

static void key_RGB(void) {
  switch (Virtual_key) {
    case 1: setRGBColor(GREEN); break;
    case 3: setRGBColor(RED); break;

    case 2:
    case 4:
      setRGBColor(BLUE);
      ;
      break;
  }
}


void key_goto_state(void) {
  uint8_t key_oo = key_state();
  if (key_oo == 2) 
  {
    Virtual_key++;            
    if (runmode == COLOR_AI)  
    {
      if (Virtual_key > 4) {
        Virtual_key = 1;
      }
    } else 
    {
      if (Virtual_key > 2) {
        Virtual_key = 1;
      }
    }


    key_RGB();

  }

  else if (key_oo == 1) 
  {
    if (runmode == COLOR_AI || runmode == REFACE_AI) 
    {
      send_key();  
    }
  }
}
