
#include <Arduino.h>
#include <Wire.h>

#include "MCU_Common.h"
#include "MCU_Master.h"

// See setup()
// #include "Adafruit_Sensor.h"
// #include "Adafruit_LIS2MDL.h"
// REMOVED DEPENDENCIES................................. FIXME

#include "Bitwise.h"
#include "Timer.h"
#include "Button.h"

// ----------------------------------------------------
// CAUTION: platformio.ini ---------------------------- BEWARE
// MASTER MCU: Change to MasterUno folder
//             Upload to Master-MCU
// SLAVE MCU: Change to SlaveUno folder
//             Upload to Slave-MCU
// ----------------------------------------------------

dsg::MCU_Master master;
nmr::Bitwise<int> util;
nmr::Timer loopTimer;
uno::Button joystickButton;

uint8_t stateID;
int xAnalog;
int yAnalog;

// Used to check I2C Bus --------------------- Bus Up and Running
// Adafruit_LIS2MDL mag = Adafruit_LIS2MDL(12345);
// REMOVED DEPENDENCIES................................. FIXME

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
    }

    // I2C (TWI) Master
    Wire.begin(MASTER_ADDR_0x14);
    delay(100);
    // 3 ms timeout
    Wire.setWireTimeout(3000 /* us */, true /* reset_on_timeout */);

    // Check I2C Bus--------------------------------- OK
    // Serial.println(master.IsAddressI2C(LIS2MDL_ADDR));
    /* Initialise the sensor */
    // if (!mag.begin())
    // {
    /* There was a problem detecting the LIS2MDL ... check your connections */
    // Serial.println("Ooops, no LIS2MDL detected ... Check your wiring!");
    // while (1);
    // }
    // REMOVED DEPENDENCIES................................. FIXME

    joystickButton = uno::Button(JOYSTICK_BUTTON, JOYSTICK_LED);
}

void Debug()
{
    Serial.print("x: ");
    Serial.print(xAnalog);
    Serial.print(" y: ");
    Serial.println(yAnalog);
}

void i2cTransmitToSlave()
{
    // I2C Message Size
    uint8_t message[SLAVE_ANALOG_SIZE];
    message[0] = (uint8_t)SLAVE_ADDR_0x16;
    message[1] = (uint8_t)stateID;

    // MESSAGE
    // Input-X
    util.WordToBytes(xAnalog);
    message[2] = util.GetHiByte();
    message[3] = util.GetLoByte();

    // Input-Y
    util.WordToBytes(yAnalog);
    message[4] = util.GetHiByte();
    message[5] = util.GetLoByte();

    // TRANSMIT
    // Transmit Message
    // Write message to the Slave
    Wire.beginTransmission(SLAVE_ADDR_0x16);
    Wire.write(message[0]);
    Wire.write(message[1]);
    Wire.write(message[2]);
    Wire.write(message[3]);
    Wire.write(message[4]);
    Wire.write(message[5]);
    Wire.endTransmission();
}

void TransmitJoystickData()
{
    // Analog Readings
    // READ = 1009 L298N Supplemental
    // specifically - the analogs...
    if (stateID == (uint8_t)1)
    {
        xAnalog = 1023 - analogRead(A0);
        yAnalog = analogRead(A1);
    }
    else if (stateID == (uint8_t)0)
    {
        xAnalog = 511;
        yAnalog = 511;
    }

    // Send data to Slave
    i2cTransmitToSlave();
}

void loop()
{
    joystickButton.updateButton();
    if (loopTimer.isTimer(100))
    {
        if (joystickButton.isButtonOn())
            stateID = (uint8_t)1;
        else
            stateID = (uint8_t)0;
        //
        TransmitJoystickData();
        //
        // Debug();
    }

    if (Wire.getWireTimeoutFlag())
    {
        // Less than timer at 6ms
        delay(6);
        Wire.clearWireTimeoutFlag();
    }
}
