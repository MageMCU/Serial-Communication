//
// Carpenter Software
// File: main.cpp
//
// Purpose: Public Github Account - MageMCU
// Repository:  
// Folder:
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
#include "TestSetup.h"

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
    }

    // I2C - TWOWIRE
    Wire.begin();

    // Adafruit Servo Driver
    servo.begin();
    // The following values works for the
    // servo motor being used... Yours might
    // be different... Read Adafruit's PDF
    // file...
    servo.setOscillatorFrequency(DRIVER_OSC_FREQ);
    servo.setPWMFreq(DRIVER_FREQ_50);

    // Used for calculating PWM signals
    pwmSignal = dsg::PWM<float>(ARDUINO_INPUT_PIN_12);

    // Setup Arduino Pins
    pinMode(ARDUINO_PWM_GENERATOR_PIN_5, OUTPUT);
    // Will use adafruit drive to generate PWM signals...
    pinMode(ARDUINO_INPUT_PIN_12, INPUT);
    // Used to normalize values
    stat = nmr::Statistics<float>(tuples, statSize);
    // OK to use delay() within setup() method

    // Mapping Degrees to PWM
    // 90 to 135 degrees
    mapLeft = nmr::LinearMap<float>(90.0, 135.0, SERVO_90_Deg, SERVO_135_Deg);
    // 45 to 90 degrees
    mapRight = nmr::LinearMap<float>(45.0, 90.0, SERVO_45_Deg, SERVO_90_Deg);

    // Randon angle from 30 to 150 degrees
    randNum = nmr::RandomNumber<float>(30.0, 150.0);

    delay(10);
}

// Statistical indices
int index = 0;
bool stable()
{
    bool flag = false;

    stat.Queue(pwmSignal.DutyCyclePercent(), index);
    // Increment index
    index++;
    // Reset index
    if (index >= statSize)
        index = 0;

    // Debug
    // Serial.print(stat.Average());
    // Serial.print(" ");
    // Serial.println(stat.StandardDeviation());
    //
    // Is data stable?
    if (stat.StandardDeviation() < (float)0.1)
    {
        flag = true;
    }

    return flag;
}

void DebugPeriodFrequncy(int cnt)
{
    Serial.print(cnt);
    Serial.print(" Generator Period (T): ");
    Serial.print(pwmSignal.Period());
    Serial.print(" Frequency (f): ");
    Serial.println(pwmSignal.Frequency());
}

void DebugDutyCycle(int cnt, float volt)
{
    Serial.print(cnt);
    // Skip if volts is zero
    if (volt > __FLT_EPSILON__)
    {
        Serial.print(" Volt: ");
        Serial.print(volt);
    }
    Serial.print(" Duty cycle (%): ");
    Serial.println(String(pwmSignal.DutyCyclePercent(), 2));
    if (cnt == 4)
        Serial.println("");
}

void loop()
{
    // PWM-Meter
    pwmSignal.Update();
    
    // TESTING ARDUINO GENERATOR OUTPUT TO ARDUINO INPUT
    // Trigger at every 250ms
    if (loopTimer.isTimer(250))
    {
        if (cnt == 0)
        {
            // PWM-Meter to calculate the Period    
            // and Frequency (See PWM.h) These 
            // values are usually a constant...
            DebugPeriodFrequncy(cnt);
            cnt = 1;
        }

        else if (cnt == 1)
        {
            if (stable())
            {
                // 1.25 volts
                DebugDutyCycle(cnt, 1.25);
                stat.QueueReset();
                cnt = 2;
            }
            else
            {
                // Convert 1.25 volts to PWM
                PWM_Level = pwmSignal.Volts2pwmLevel(1.25);
                analogWrite(ARDUINO_PWM_GENERATOR_PIN_5, PWM_Level);
            }
        }
        else if (cnt == 2)
        {
            if (stable())
            {
                // 2.5 volts
                DebugDutyCycle(cnt, 2.5);
                stat.QueueReset();
                cnt = 3;
            }
            else
            {
                // Convert 2.5 volts to PWM
                PWM_Level = pwmSignal.Volts2pwmLevel(2.5);
                analogWrite(ARDUINO_PWM_GENERATOR_PIN_5, PWM_Level);
            }
        }
        else if (cnt == 3)
        {
            if (stable())
            {
                // 3.75 volts
                DebugDutyCycle(cnt, 3.75);
                stat.QueueReset();
                cnt = 4;
            }
            else
            {
                // Convert 3.75 volts to PWM
                PWM_Level = pwmSignal.Volts2pwmLevel(3.75);
                analogWrite(ARDUINO_PWM_GENERATOR_PIN_5, PWM_Level);
            }
        }
        else if (cnt == 4)
        {
            if (stable())
            {
                // 4.99 volts
                DebugDutyCycle(cnt, 4.99);
                stat.QueueReset();
                cnt = 0; // ---------------------------------- REPEAT
            }
            else
            {
                // Convert 4.99 volts to PWM
                PWM_Level = pwmSignal.Volts2pwmLevel(4.99);
                analogWrite(ARDUINO_PWM_GENERATOR_PIN_5, PWM_Level);
            }
        }
    }
}
