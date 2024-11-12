//
// Carpenter Software
// File: Calibration: main.cpp
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

#include <Adafruit_LIS2MDL.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_LIS2MDL lis2mdl = Adafruit_LIS2MDL(12345);

int RawX, RawMinX, RawMaxX, RawOffsetX = 0;
int RawY, RawMinY, RawMaxY, RawOffsetY = 0;
int RawZ, RawMinZ, RawMaxZ, RawOffsetZ = 0;

void setup(void)
{
    Serial.begin(9600);
    while (!Serial)
    {
    }

    // Using I2C

    // Enable auto-gain
    lis2mdl.enableAutoRange(true);

    // Initialise the sensor
    lis2mdl.begin();

    // Display some basic information on this sensor
    lis2mdl.printSensorDetails();
}

int counter = 0;
void loop(void)
{
    // Get a new sensor event
    sensors_event_t event;
    lis2mdl.getEvent(&event);

    // X
    RawX = lis2mdl.raw.x;
    if (RawX < RawMinX)
        RawMinX = RawX;
    if (RawX > RawMaxX)
        RawMaxX = RawX;
    RawOffsetX = (RawMinX + RawMaxX) / 2;

    // Y
    RawY = lis2mdl.raw.y;
    if (RawY < RawMinY)
        RawMinY = RawY;
    if (RawY > RawMaxY)
        RawMaxY = RawY;
    RawOffsetY = (RawMinY + RawMaxY) / 2;

    // Z
    RawZ = lis2mdl.raw.z;
    if (RawZ < RawMinZ)
        RawMinZ = RawZ;
    if (RawZ > RawMaxZ)
        RawMaxZ = RawZ;
    RawOffsetZ = (RawMinZ + RawMaxZ) / 2;

    // Print Offsets
    // Rotate LIS2MDL in all three planes...
    // xy-plane
    // yz-plane
    // zx-plane
    // UNTIL the offset data no longer changes...

    if (counter > 100)
    {
        Serial.print(RawOffsetX);
        Serial.print(", ");
        Serial.print(RawOffsetY);
        Serial.print(", ");
        Serial.print(RawOffsetZ);
        Serial.println();
        counter = 0;
    }

    // Increment Counter
    counter++;

    // Delay before the next sample
    delay(100);
}

