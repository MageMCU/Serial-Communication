//
// Carpenter Software
// File: Common.h
//
// Purpose: Public Github Account - MageMCU
// Repository: Communication 
// Date Created: 20230407
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

#ifndef Digital_Signals_Test_Common_h
#define Digital_Signals_Test_Common_h

#include <Arduino.h>
#include <Wire.h>

#include "BusI2C.h"
#include "PWM.h"


template <typename real>
void printResult(String s, real result)
{
    Serial.print(s);
    Serial.println(String(result, 5));
}

template <typename real>
void printResults(String s1, real result1, String s2, real result2)
{
    Serial.print(s1);
    Serial.print(result1);
    Serial.print(s2);
    Serial.println(result2);
}

template <typename integer>
void printSpecial(String s1, integer integerNumber, String s2)
{
    Serial.print(s1);
    Serial.print(integerNumber);
    Serial.print(" - ");
    Serial.println(s2);
}

void printTitle(String s)
{
    Serial.print("---------------- + ");
    Serial.print(s);
    Serial.println(" +");
}

void printSubTitle(String s)
{
    Serial.print(" ** ");
    Serial.print(s);
    Serial.println(" **");
}

void printNewLine()
{
    Serial.println("");
}

#endif 
