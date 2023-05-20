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
    pwmSignal.Update();
    
    // Mapping - SERVO MOTOR
    if (loopTimer.isTimer(3000))
    {
        // Random Angle
        float angle = randNum.Random();

        // Map angle to tick
        uint16_t tick;
        if (angle > 90.0)
            tick = mapLeft.Map(angle);
        else
            tick = mapRight.Map(angle);

        // Set PWM
        servo.setPWM(DRIVER_NUM_11, 0, tick);

        // Debug
        Serial.print("angle: ");
        Serial.print(angle);
        Serial.print(" tick: ");
        Serial.println(tick);
    }
}
