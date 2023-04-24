
#include <Arduino.h>
#include <Wire.h>

#include "MCU_Common.h"
#include "MCU_Slave.h"

#include "Bitwise.h"
#include "Timer.h"

// ----------------------------------------------------
// CAUTION: platformio.ini ---------------------------- BEWARE
// MASTER MCU: Change to MasterUno folder
//             Upload to Master-MCU
// SLAVE MCU: Change to SlaveUno folder
//             Upload to Slave-MCU
// ----------------------------------------------------

dsg::MCU_Slave slave;
nmr::Bitwise<int> util;
nmr::Timer loopTimer;

// Global Function Pointer
void (*pCurrentState)();
uint8_t stateID;
int xAnalog;
int yAnalog;

void Debug()
{
    Serial.print("x: ");
    Serial.print(xAnalog);
    Serial.print(" y: ");
    Serial.println(yAnalog);
}

void ButtonON()
{
    Serial.println("bON");
}

void ButtonOFF()
{
    Serial.println("bOFF");
}

void SlaveReceiveI2C(int numberBytes)
{
    uint8_t slaveMessage[numberBytes];

    // I2C Receive
    int cnt = 0;
    while (Wire.available())
    {
        slaveMessage[cnt] = Wire.read();
        cnt++;
    }

    // Joystick Neutral Position
    xAnalog = 511;
    yAnalog = 511;

    // Build the data back to its original values....
    if (slaveMessage[0] == SLAVE_ADDR_0x16)
    {
        stateID = slaveMessage[1];
        xAnalog = util.BytesToWord(slaveMessage[2], slaveMessage[3]);
        yAnalog = util.BytesToWord(slaveMessage[4], slaveMessage[5]);

        if (stateID == (uint8_t)BUTTON_ON)
            pCurrentState = ButtonON;
        else if (stateID == (uint8_t)BUTTON_OFF)
            pCurrentState = ButtonOFF;
    }
}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
    }

    // I2C
    Wire.begin(SLAVE_ADDR_0x16);
    delay(100);
    // 3 ms timeout
    Wire.setWireTimeout(3000 /* us */, true /* reset_on_timeout */);

    // State Machine
    pCurrentState = nullptr;
}

void loop()
{
    if (loopTimer.isTimer(100))
    {
        Wire.onReceive(SlaveReceiveI2C);

        // Remember Master also has debug
        // so turn one off... Or fix the
        // out for readability...
        Debug();
        //
        if (pCurrentState)
            pCurrentState();
    }

    if (Wire.getWireTimeoutFlag())
    {
        // Less than timer at 6ms
        delay(6);
        Wire.clearWireTimeoutFlag();
    }
}
