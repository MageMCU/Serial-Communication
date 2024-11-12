//
// Carpenter Software
// File: Common.h
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

#ifndef CommonMCU_h
#define CommonMCU_h

//--------------------------------------
// MASTER UNO --------------------------
#define MASTER_ADDR_0x14 0x14
// SLAVE UNO----------------------------
#define SLAVE_ADDR_0x16 0x16
// MESSAGE SIZE
#define I2C_MESSAGE_SIZE 6

// Arduino Input Button - Output LED
#define JOYSTICK_LED 12
#define JOYSTICK_BUTTON 2

// State Defines
#define BUTTON_ON 1
#define BUTTON_OFF 0
//--------------------------------------

// COMMON Global Variables 
uint8_t stateID;

// Joystick Potentiometers
int xAnalog;
int yAnalog;

// COMMON Debug
void Debug()
{
    Serial.print("x: ");
    Serial.print(xAnalog);
    Serial.print(" y: ");
    Serial.println(yAnalog);
}

#endif