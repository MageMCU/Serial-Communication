
#ifndef PWM_Test_Setup_h
#define PWM_Test_Setup_h

#include <Arduino.h>
#include <Wire.h>

#include "Adafruit_PWMServoDriver.h"
#include "PWM.h"
#include "Statistics.h"
#include "Timer.h"
#include "LinearMap.h"
#include "RandomNumber.h"

// ARDUINO SELF TEST - DRIVER NOT USED HERE
// Please study the code

// Driver Initialization
Adafruit_PWMServoDriver servo = Adafruit_PWMServoDriver();
// HAving Fun   
nmr::Statistics<float> stat;
int statSize = 10;
float tuples[]{3.2, 5.3, 7.0, 2.2, 9.8, 4.6, 0.5, 4.5, 7.4, 6.5};
uint8_t cnt = 0;
// Loop timer
nmr::Timer loopTimer;
// Linear Mapping
nmr::LinearMap<float> mapLeft;
nmr::LinearMap<float> mapRight;
// Random Number
nmr::RandomNumber<float> randNum;
// PWM Signal Calculations
dsg::PWM<float> pwmSignal;
uint8_t PWM_Level;

// Arduino Setup
#define ARDUINO_PWM_GENERATOR_PIN_5 5
#define ARDUINO_INPUT_PIN_12 12
// Driver Setup
#define DRIVER_FREQ_50 50
#define DRIVER_OSC_FREQ 23000000
// Servo Numbers
#define DRIVER_NUM_11 11 // CAUTION: Connected to Servo - Use 6-volts
#define DRIVER_NUM_15 15 // CAUTION: Connected to Arduino Input - Use 5-volts
// Servo Positions (Calibrate servo-values to angles)
// Use a cardborad paper with three angles (45, 90, 135)
#define SERVO_135_Deg 490   // LEFT-OFFSET FROM-CENTER 45 degrees (135 degrees)
#define SERVO_90_Deg 375 // CENTER at 90 degrees between 0 and 180...
#define SERVO_45_Deg 250  // RIGHT-OFFSET FROM-CENTER 45 degrees (45 degrees)

#endif