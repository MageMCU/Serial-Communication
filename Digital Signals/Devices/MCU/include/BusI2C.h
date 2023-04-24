//
// Carpenter Software
// File: Class BusI2C.h
//
// Purpose: Public Github Account - MageMCU
// Repository: Communication
// Date Created: 20230219
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

#ifndef BUS_I2C_h
#define BUS_I2C_h

#include <Arduino.h>
#include <Wire.h>

// IN DEVELOMENT ------------------------------- FIXME
namespace dsg
{
    class BusI2C
    {
    public:
        // Public Properties
        // Constructor
        BusI2C();
        ~BusI2C() = default;

        // Public Methods
        String IsDevice(uint8_t deviceAddress);
        String ErrorMsg();
        uint8_t GetErrorNumberI2C();
        void SetWireTimeout(uint32_t timeout, bool reset);
        bool IsTimeout();
        void ClearTimeout();
        uint8_t RecieveMessage(uint8_t addr, uint8_t msg[], uint8_t size);
        void TransmitMessage(uint8_t addr, uint8_t msg[], uint8_t size);
        uint8_t RecieveDeviceMessage(uint8_t addr, uint8_t reg, uint8_t msg[], uint8_t size);

    private:
        // Private Property
        uint8_t m_errorI2C;
        // Private Methods
        String m_errorMessageI2C();
        void m_scanningI2C(uint8_t deviceAddress);
    };

    BusI2C::BusI2C()
    {
        m_errorI2C = 0;
        Serial.println("BusI2C Contructor");
    }

    String BusI2C::IsDevice(uint8_t deviceAddress)
    {
        m_scanningI2C(deviceAddress);
        if (m_errorI2C == 0)
        {
            String str = "Found address: " + String(deviceAddress, HEX);
            return str;
        }
        return m_errorMessageI2C();
    }

    String BusI2C::ErrorMsg()
    {
        return m_errorMessageI2C();
    }

    uint8_t BusI2C::GetErrorNumberI2C()
    {
        return m_errorI2C;
    }

    void BusI2C::SetWireTimeout(uint32_t timeout, bool reset)
    {
        // microseconds 3000us = 3ms = 0.003s
        // This may be used in setup()
        Wire.setWireTimeout(timeout, reset);
    }

    bool BusI2C::IsTimeout()
    {
        return Wire.getWireTimeoutFlag();
    }

    void BusI2C::ClearTimeout()
    {
        Wire.clearWireTimeoutFlag();
    }

    uint8_t BusI2C::RecieveMessage(uint8_t addr, uint8_t msg[], uint8_t size)
    {
        int bufIndex = 0;
        Wire.beginTransmission(addr);
        Wire.requestFrom(addr, size);
        while (!Wire.available())
        {
        };
        while (Wire.available())
        {
            msg[bufIndex] = Wire.read();
            bufIndex++;
        };
        m_errorI2C = Wire.endTransmission();
        return (bufIndex - 1);
    }

    void BusI2C::TransmitMessage(uint8_t addr, uint8_t msg[], uint8_t size)
    {
        Wire.beginTransmission(addr);
        for (uint8_t bIdx = 0; bIdx < size; bIdx++)
        {
            Wire.write(msg[bIdx]);
        }
        m_errorI2C = Wire.endTransmission();
    }

    uint8_t BusI2C::RecieveDeviceMessage(uint8_t addr, uint8_t reg, uint8_t msg[], uint8_t size)
    {
        int bufIndex = 0;

        Wire.beginTransmission(addr);
        Wire.write(reg);
        m_errorI2C = Wire.endTransmission();
        if (m_errorI2C == 0)
        {
            Wire.requestFrom(addr, size);
            while (!Wire.available())
            {
            };
            while (Wire.available())
            {
                msg[bufIndex] = Wire.read();
                bufIndex++;
            };
            m_errorI2C = Wire.endTransmission();
        }
        return (bufIndex - 1);
    }

    String BusI2C::m_errorMessageI2C()
    {
        String message = "";
        if (m_errorI2C >= 0 && m_errorI2C <= 5)
        {
            switch (m_errorI2C)
            {
            case 0:
                message = "twiErr0: success";
                break;
            case 1:
                message = "twiErr1: length to long for buffer";
                break;
            case 2:
                message = "twiErr2: address send, NACK received";
                break;
            case 3:
                message = "twiErr3: data send, NACK received";
                break;
            case 4:
                message = "twiErr4:(lost bus arbitration, bus error, ..)";
                break;
            case 5:
                message = "twiErr5: timeout";
                break;
            default:
                message = "twiErrDefault: Not an Address";
                break;
            }
        }
        return message;
    }

    void BusI2C::m_scanningI2C(uint8_t deviceAddress)
    {
        uint8_t lowerAddress = 0x08;
        uint8_t upperAddress = 0x77;
        if (deviceAddress >= lowerAddress && deviceAddress <= upperAddress)
        {
            Wire.beginTransmission(deviceAddress);
            m_errorI2C = Wire.endTransmission();
        }
    }
}
#endif
