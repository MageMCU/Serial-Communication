//
// Carpenter Software
// File: Class BusI2C.h
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


#ifndef BUS_I2C_h
#define BUS_I2C_h

#include "Arduino.h"
#include "Wire.h"

namespace bus
{
    template <typename real>
    class BusI2C
    {
    public:
        // Public Properties
        // Constructor
        BusI2C();
        ~BusI2C() = default;

        // Public Methods
        String IsDevice(uint8_t deviceAddress);
        uint8_t GetErrorNumberI2C();

    private:
        // Private Property
        uint8_t m_errorI2C;
        // Private Methods
        String m_errorMessageI2C();
        void m_scanningI2C(uint8_t deviceAddress);
    };

    template <typename real>
    BusI2C<real>::BusI2C()
    {
        m_errorI2C = 0;
    }

    template <typename real>
    String BusI2C<real>::IsDevice(uint8_t deviceAddress)
    {
        m_scanningI2C(deviceAddress);
        if (m_errorI2C == 0)
        {
            String str = "Found address: " + String(deviceAddress, HEX);
            return str;
        }
        return m_errorMessageI2C();
    }

    template <typename real>
    uint8_t BusI2C<real>::GetErrorNumberI2C()
    {
        return m_errorI2C;
    }

    template <typename real>
    String BusI2C<real>::m_errorMessageI2C()
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

    template <typename real>
    void BusI2C<real>::m_scanningI2C(uint8_t deviceAddress)
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
