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
// Testing Platform:
//  * MCU:Atmega328P
//  * Editor: VSCode
//  * VSCode Extension: Microsoft C/C++ IntelliSense, debugging, and code browsing.
//  * VSCode Extension:PlatformIO
// 
// Revised Header-Comment 20230219
//
// MIT LICENSE
//

#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LIS2MDL.h>

Adafruit_LIS2MDL mag = Adafruit_LIS2MDL(12345);

float XAxis;
float YAxis;
float ZAxis;

float heading;
float declinationAngle;

void setup(void)
{
    Serial.begin(9600);
    Serial.println("Compass Heading");
    Serial.println("");

    /* Initialise the sensor */
    if (!mag.begin())
    {
        /* There was a problem detecting the LIS2MDL ... check your connections */
        Serial.println("Ooops, no LIS2MDL detected ... Check your wiring!");
        while (1)
            ;
    }
}

void loop(void)
{
    /* Get a new sensor event */
    sensors_event_t event;
    mag.getEvent(&event);

    // Raw Offsets (Use Calibration)
    // -17, -221, -83
    
    // Calculate axis with Offsets
    // For offsets, use opposite sign (-/+)
    XAxis = (float)(mag.raw.x + 17);
    YAxis = (float)(mag.raw.y + 221);
    ZAxis = (float)(mag.raw.z + 83);

    // Calculate the angle of the vector y,x (Degrees)
    heading = atan2(YAxis, XAxis) * RAD_TO_DEG;

    // Declination (Degrees)
    // You can find your declination...
    // SOURCE: https://www.magnetic-declination.com/
    // (+) Positive or (-) for negative
    // Formula: (deg + (min / 60.0)) / (180 / M_PI);
    // This is author's location...
    // Albuquerque, NM, USA 
    declinationAngle = 8.0 + (3.0 / 60.0);
    heading += declinationAngle;

    // Normalize to 0-360
    if (heading < 0)
    {
        heading += 360;
    }
    if (heading >= 360)
    {
        heading -= 360;
    }

    Serial.print("Heading: ");
    Serial.println(heading);

    delay(500);
}
