//
// Carpenter Software
// File: main.cpp
//
// Purpose: Public Github Account - MageMCU
// Repository: Communication 
// Date Created: 20230219
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

// Common utility functions.
#include "TESTS/Common.h"
#include "TESTS/TestBusI2C.h"
#include "TESTS/TestPWM.h"

void setup()
{
    // Debug
    Serial.begin(9600);
    while (!Serial)
    {
    }

    // Test each method in each class
    // PWM Tests
    PWM_Setup();

}

void loop()
{
    // BusI2C_Loop(); // Pending 
    PWM_Loop();
}