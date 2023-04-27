//
// Carpenter Software
// File: TestPWM.h
//
// Purpose: Public Github Account - MageMCU
// Repository: Communication
// Date Created: 20230407
// Folder: Digital Signals
//
// Author: Jesse Carpenter (carpentersoftware.com)
// Email:carpenterhesse@gmail.com
//
// Testing Platform:
//  * MCU:Atmega328P
//  * Editor: VSCode
//  * VSCode Extension: Microsoft C/C++ IntelliSense, debugging, and code browsing.
//  * VSCode Extension:PlatformIO
//
// MIT LICENSE
//

#include <Arduino.h>
#include <Wire.h>

#include "Timer.h"
#include "PWM.h"
#include "../TESTS/Common.h"

#ifndef Digital_Signals_Test_PWM_h
#define Digital_Signals_Test_PWM_h

// Test PWM requires Arduino loop() function...

const int GeneratorPWM = 3;
// Pin 3 ~ Frequency 492Hz *
// Pin 5 ~ Frequency 981Hx *
// Pin 6 ~ Frequency 981Hz
// Pin 9 ~ Frequency 492Hz
// Pin 10 ~ Frequency 492Hz
// Pin 11 ~ Frequency 492Hz
const int PWM_InputPin = 2;
dsg::PWM<float> pwmSignal;
nmr::Timer loopTimer;

void PWM_Setup()
{
    // Simply connect a jumper between
    // these two pins...
    pinMode(GeneratorPWM, OUTPUT);
    pinMode(PWM_InputPin, INPUT);
    pwmSignal = dsg::PWM<float>(PWM_InputPin);
    uint8_t PWM_Level = pwmSignal.Volts2pwmLevel(2.5);
    analogWrite(GeneratorPWM, PWM_Level);
}

void TestDutyCycle()
{
  double dutyCycle = pwmSignal.DutyCycle();
  double dcPercent = pwmSignal.DutyCyclePercent();

  Serial.print("dutyCycle:");
  Serial.print(dutyCycle, 4);
  Serial.print(" ");
  Serial.print(dcPercent, 2);
  Serial.println(" %");
}

void TestPeriod()
{
    // Period is a constant
    long T = pwmSignal.Period();
    Serial.print("T: ");
    Serial.print(T);
    // microseconds (us)
    Serial.println(" us");
}

void TestFrequency()
{
    // Frequency is a constant
    // see the above table...
    long freq = pwmSignal.Frequency();
    Serial.print("freq: ");
    Serial.print(freq);
    Serial.println(" Hz");
}

void TimerPWM()
{
    if (loopTimer.isTimer(1000))
    {
        TestFrequency();
        TestPeriod();
        TestDutyCycle();
    }
}

void PWM_Loop()
{
    pwmSignal.Update();
    TimerPWM();
}

#endif