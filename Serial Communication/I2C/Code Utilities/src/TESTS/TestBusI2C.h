//
// Carpenter Software
// File: TestBusI2C.h
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

#ifndef Digital_Signals_Test_BusI2C_h
#define Digital_Signals_Test_BusI2C_h

#include "../TESTS/Common.h"

// Pending

void BusI2C_T1_BytesWordOperations()
{
    printTitle("BusI2C T1 Bytes Word Operations");

    dsg::BusI2C bus = dsg::BusI2C();
    uint16_t word = 5678;

    bus.WordToBytes(word);
    Serial.print("word: ");
    Serial.println(word);

    uint8_t hi = bus.GetHiByte();
    Serial.print("hi-byte: ");
    Serial.println(hi);

    uint8_t lo = bus.GetLoByte();
    Serial.print("lo-byte: ");
    Serial.println(lo);

    word = bus.BytesToWord();
    Serial.print("combine implicitly to word: ");
    Serial.println(word);

    word = bus.BytesToWord(hi, lo);
    Serial.print("combine explicitly to word: ");
    Serial.println(word);
}



#endif
