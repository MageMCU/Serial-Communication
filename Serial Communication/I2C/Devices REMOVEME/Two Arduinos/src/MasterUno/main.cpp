//
// Carpenter Software
// File: MasterUno: main.cpp
//
// Purpose: Public Github Account - MageMCU
// Repository: Communication
// Date Created: 20230425
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

#include "Common.h"
#include "BusI2C.h"
#include "Timer.h"
#include "Button.h"

// Arduino Uno as Master communicates with another Arduino as Slave.
// ----------------------------------------------------
// CAUTION: platformio.ini ---------------------------- BEWARE
// MASTER MCU: Change to MasterUno folder
//             Upload to Master-MCU
// SLAVE MCU: Change to SlaveUno folder
//             Upload to Slave-MCU
// ----------------------------------------------------
// ----------------------------------------------------
// Note that some Wire call functions reside in both
// BusI2C.h and main.cpp...
// ----------------------------------------------------

// Global Declarations
dsg::BusI2C busI2C;
nmr::Timer loopTimer;
uno::Button joystickButton;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
    }

    // Assign BusI2C Object
    busI2C = dsg::BusI2C();
    // Wire.BEGIN: Include Timeout
    busI2C.Begin(MASTER_ADDR_0x14, 3000, true);
    // Assign Button Object
    joystickButton = uno::Button(JOYSTICK_BUTTON, JOYSTICK_LED);
}

void i2cTransmitToSlave()
{
    // I2C Message Array Byte Size
    uint8_t message[I2C_MESSAGE_SIZE];
    // Arrange Bytes
    message[0] = (uint8_t)SLAVE_ADDR_0x16;
    message[1] = (uint8_t)stateID;
    // Input-X
    busI2C.WordToBytes(xAnalog);
    message[2] = busI2C.GetHiByte();
    message[3] = busI2C.GetLoByte();
    // Input-Y
    busI2C.WordToBytes(yAnalog);
    message[4] = busI2C.GetHiByte();
    message[5] = busI2C.GetLoByte();
    // TRANSMIT
    busI2C.TransmitMessage(SLAVE_ADDR_0x16, message, I2C_MESSAGE_SIZE);
    // Print all errors...
    Serial.println(busI2C.ErrorMsg());
}

void TransmitJoystickData()
{
    // Analog Readings
    // READ = 1009 L298N Supplemental
    // specifically - the analogs...
    if (stateID == (uint8_t)BUTTON_ON)
    {
        // Read Arduino Analogs A0 and A1
        xAnalog = 1023 - analogRead(A0);
        yAnalog = analogRead(A1);
    }
    else if (stateID == (uint8_t)BUTTON_OFF)
    {
        // Analog midpoints 
        xAnalog = 511;
        yAnalog = 511;
    }

    // Send data to Slave
    i2cTransmitToSlave();
}

void loop()
{
    joystickButton.updateButton();
    if (loopTimer.isTimer(250))
    {
        if (joystickButton.isButtonOn())
            stateID = (uint8_t)BUTTON_ON;
        else
            stateID = (uint8_t)BUTTON_OFF;

        // I2C TRANSMIT
        TransmitJoystickData();
    }

    // Prevent hiccups (stalls)
    busI2C.ClearTimeout();
}
