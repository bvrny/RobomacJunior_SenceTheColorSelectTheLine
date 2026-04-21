#include <Wire.h>                     
#include <Adafruit_PWMServoDriver.h> 

// Define servo related parameters
#define SERVO_MIN_PULSE_WIDTH 500
#define SERVO_MAX_PULSE_WIDTH 2500
#define SERVO_PULSE_WIDTH_CYCLE 20000
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180
#define PWM_FREQUENCY 50

// Define the servos control pin
#define Servo1_PIN 0
#define Servo2_PIN 1
#define Servo3_PIN 2
#define Servo4_PIN 3

// Define the IIC address of the Bottom-layer driver chip
#define Bottom_Layer_Driver_ADDR 0x40

// Create an instance of the Adafruit_PWMServoDriver class
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(Bottom_Layer_Driver_ADDR);

void setServoAngle(uint8_t servoPin, int angle);Set the servo rotation angle

void setup() {
  Wire.begin();                   // Initialize I2C communication
  delay(1000);                    
  pwm.begin();                    // Initialize the Pulse Width Modulation (PWM) library
  pwm.setPWMFreq(PWM_FREQUENCY);  // Set the PWM frequency
}

void loop() {
  setServoAngle(Servo1_PIN, 90);

  delay(4000);
  for (int angle = 0; angle <= 180; angle += 5) {
    setServoAngle(Servo1_PIN, angle);
    delay(200);
  }
  for (int angle = 180; angle >= 0; angle -= 15) {
    setServoAngle(Servo1_PIN, angle);
    delay(200);
  }
}

/**
 * @brief Set the servo rotation angle
 * @param servoPin: Servo control pin
 * @param angle: Servo rotation angle
 */
void setServoAngle(uint8_t servoPin, int angle) {
  long pulseWidth = map(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, SERVO_MIN_PULSE_WIDTH, SERVO_MAX_PULSE_WIDTH);
  long PWM_Value = pulseWidth * 4096 / SERVO_PULSE_WIDTH_CYCLE;
  pwm.setPWM(servoPin, 0, PWM_Value);
}
