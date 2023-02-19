//
// Carpenter Software
// File: main.cpp
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

#include "Arduino.h"

#include "BusI2C.h"

// Global Varibles
bus::BusI2C<float> scanAddressI2C;

void setup()
{
    // Debug
    Serial.begin(9600);
    while (!Serial)
    {
    }
    // Arduino I2C init
    Wire.begin();

    // Intialize BusI2C
    scanAddressI2C = bus::BusI2C<float>();
    // Address for the Honeywell HMC5883L: 0x1E
    // Purpose whether this device can be detected???
    Serial.println(scanAddressI2C.IsDevice(0x1E));
}

void loop()
{
}