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

#include <Arduino.h>
#include <Wire.h>

#ifndef BUS_I2C_h
#define BUS_I2C_h

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
        void Begin(uint8_t address);
        void Begin(uint8_t address, uint32_t timeout, bool reset);
        void ClearTimeout();
        String IsDevice(uint8_t deviceAddress);
        String ErrorMsg();
        uint8_t GetErrorNumberI2C();
        void ReceiveMessage(uint8_t *pMessage, uint8_t size);
        uint8_t RequestMessage(uint8_t addr, uint8_t msg[], uint8_t size);
        void TransmitMessage(uint8_t addr, uint8_t msg[], uint8_t size);
        uint8_t RecieveDeviceMessage(uint8_t addr, uint8_t reg, uint8_t msg[], uint8_t size);

        // Experimental - separate from bitwise
        // Byte Word Operations - Experimental
        void WordToBytes(uint16_t word);
        uint8_t GetHiByte();
        uint8_t GetLoByte();
        uint16_t BytesToWord();
        uint16_t BytesToWord(uint8_t hiByte, uint8_t loByte);

    private:
        // Private Properties
        uint8_t m_errorI2C;
        // Experimental - separate from bitwise
        // Byte - Word Properties
        // AVR-LSB Word Order bit15, bit14, ... , bit1, bit0.
        uint16_t b_wordIN;
        uint16_t b_wordOUT;
        // AVR-LSB Byte Order bit7, bit6, ... , bit1, bit0.
        uint8_t b_byteHi;
        uint8_t b_byteLo;

        // Private Methods
        void m_receive();
        String m_errorMessageI2C();
        void m_scanningI2C(uint8_t deviceAddress);
        // Byte - Word privatemethods
        void b_setHiByte();
        void b_setLoByte();
        void b_glueBytes();
    };

    BusI2C::BusI2C()
    {
        m_errorI2C = 0;
        Serial.println("BusI2C Const...");
    }

    void BusI2C::Begin(uint8_t address)
    {
        Wire.begin(address);
        delay(6);
    }

    void BusI2C::Begin(uint8_t address, uint32_t timeout, bool reset)
    {
        Wire.begin(address);
        delay(6);
        // 3 ms timeout
        Wire.setWireTimeout(timeout, reset);
    }

    void BusI2C::ClearTimeout()
    {
        if (Wire.getWireTimeoutFlag())
        {
            Wire.clearWireTimeoutFlag();
            // Less than timer at 6ms
            delay(6);
        }
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

    void BusI2C::ReceiveMessage(uint8_t *pMessage, uint8_t size)
    {
        int bufIndex = 0;
        while (Wire.available())
        {
            pMessage[bufIndex] = Wire.read();
            bufIndex++;
        };
    }

    uint8_t BusI2C::RequestMessage(uint8_t addr, uint8_t msg[], uint8_t size)
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

    void BusI2C::WordToBytes(uint16_t uint16)
    {
        b_wordIN = uint16;
        b_setHiByte();
        b_setLoByte();
    }
    
    uint8_t BusI2C::GetHiByte()
    {
        return b_byteHi;
    }
    
    uint8_t BusI2C::GetLoByte()
    {
        return b_byteLo;
    }
    
    uint16_t BusI2C::BytesToWord()
    {
        b_glueBytes();
        return b_wordOUT;
    }
    
    uint16_t BusI2C::BytesToWord(uint8_t hiByte, uint8_t loByte)
    {
        return (uint16_t)((hiByte << 8) | (loByte & 0x00FF));
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
    
    void BusI2C::b_setHiByte()
    {
        b_byteHi = (uint8_t)(b_wordIN >> 8);
    }
    
    void BusI2C::b_setLoByte()
    {
        b_byteLo = (uint8_t)(b_wordIN & 0xff);
    }
    
    void BusI2C::b_glueBytes()
    {
        b_wordOUT = (uint16_t)((b_byteHi << 8) | (b_byteLo & 0x00FF));
    }
}
#endif
