//
// Carpenter Software
// File: StateMachine.h
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
#include "Common.h"

#ifndef State_Machine_h
#define State_Machine_h

// Global Function Pointer
void (*pCurrentState)();

// States
void ButtonON()
{
    Serial.print("bON: ");
}
void ButtonOFF()
{
    Serial.print("bOFF: ");
}

// State Machine
void StateMachine()
{
    // Simple State Machine
    if (stateID == (uint8_t)BUTTON_ON)
    {
        pCurrentState = ButtonON;
    }
    else if (stateID == (uint8_t)BUTTON_OFF)
    {
        pCurrentState = ButtonOFF;
    }
    // Call Current State
    if (pCurrentState)
        pCurrentState();
    
    // Debug
    Debug();
}

#endif