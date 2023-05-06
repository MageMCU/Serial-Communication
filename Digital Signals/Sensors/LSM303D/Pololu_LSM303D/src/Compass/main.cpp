//
// Carpenter Software
// File: Compass: main.cpp
//
// Purpose: Public Github Account - MageMCU
// Repository: Communication 
// Date Created: 20230219
// Folder: I2C Bus
//
// Author: Jesse Carpenter (carpentersoftware.com)
// Email:carpenterhesse@gmail.com
// 
// Code: pololu/lsm303-arduino
// Source:https://github.com/pololu/lsm303-arduino
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

#include "LSM303.h"

LSM303 compass;

void setup()
{
    Serial.begin(9600);
    Wire.begin();

    // LSM303
    compass.init();
    compass.enableDefault();

    // Calibration values; the default values of +/-32767 for each axis...
    // Use Calibration Code
    compass.m_min = (LSM303::vector<int16_t>){-2747, -2711, -1975};
    compass.m_max = (LSM303::vector<int16_t>){+2978, +2856, +2723};
}

void loop()
{
    compass.read();

    //   When given no arguments, the heading() function returns the angular
    //   difference in the horizontal plane between a default vector and
    //   north, in degrees.
    //   The default vector is chosen by the library to point along the
    //   surface of the PCB, in the direction of the top of the text on the
    //   silkscreen. This is the +X axis on the Pololu LSM303D carrier and
    //   the -Y axis on the Pololu LSM303DLHC, LSM303DLM, and LSM303DLH
    //   carriers.
    //   To use a different vector as a reference, use the version of heading()
    //   that takes a vector argument; for example, use
    //     compass.heading((LSM303::vector<int>){0, 0, 1});
    //   to use the +Z axis as a reference.
    
    // Degrees - See LSM303.h line 270
    float heading = compass.heading();

    // NOAA - Declination 
    // Source: https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml?
    float declinationAngle = (8.0 + (3.0 / 60.0));
    heading += declinationAngle;

    Serial.println(heading);
    delay(100);
}