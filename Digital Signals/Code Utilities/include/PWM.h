//
// Carpenter Software
// File: Class PWM.h
//
// Purpose: Public Github Account - MageMCU
// Repository: Communication
// Folder: Digital Signals
// namespace: dsg
//
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

#ifndef DSG_PWM_h
#define DSG_PWM_h

namespace dsg
{
    template <typename real>
    class PWM
    {
    private:
        // Arduino Input Pin Numbers
        uint8_t m_inputPin;
        uint8_t m_outputPin;

        // Arduino impulseIn() 1
        real m_onCycle;
        real m_offCycle;

        // Calculations 1
        real m_period;
        long m_frequency;
        real m_dutyCycle;

        void m_calculate();

    public:
        PWM() = default;
        // Any input pin on the arduino
        PWM(uint8_t inputPin);
        ~PWM() = default;

        void Update();

        real Period();
        long Frequency();
        real DutyCycle();
        real DutyCyclePercent();
        uint8_t Volts2pwmLevel(real desiredVoltageOutput);
    };

    template <typename real>
    PWM<real>::PWM(uint8_t inputPin)
    {
        // Used for pusleIn()
        m_inputPin = inputPin;
    }

    template <typename real>
    void PWM<real>::Update()
    {
        m_calculate();
    }

    template <typename real>
    real PWM<real>::Period()
    {
        return m_period;
    }

    template <typename real>
    long PWM<real>::Frequency()
    {
        return m_frequency;
    }

    template <typename real>
    real PWM<real>::DutyCycle()
    {
        return m_dutyCycle;
    }

    template <typename real>
    real PWM<real>::DutyCyclePercent()
    {
        return m_dutyCycle * 100.0;
    }

    template <typename real>
    uint8_t PWM<real>::Volts2pwmLevel(real desiredVoltageOutput)
    {
        // This may not be
        // Arduino Uno 0 to 5 volts...
        if (desiredVoltageOutput > (real)0.0 &&
            desiredVoltageOutput <= (real)5.0)
        {
            // 8-bit value: 0 - 255 (256)
            // Arduino Uno 5-volt device
            // 255 / 5 = 51
            // Source: Basic Pulse Width Modulation
            //         by Gerald Recktenwald (Fall 2011)
            //         Equation 2...
            return (real)51 * desiredVoltageOutput;
        }
        return 0;
    }

    template <typename real>
    void PWM<real>::m_calculate()
    {
        // microseconds (us)
        m_onCycle = pulseIn(m_inputPin, HIGH);

        // microseconds (us)
        m_offCycle = pulseIn(m_inputPin, LOW);

        // Period (T)=(Time per Cycle)=(us)
        m_period = abs(m_onCycle + m_offCycle);

        // Hz (Cycles per Time)=(cycles per seconds)
        m_frequency = 0;

        // no units (cancel)
        m_dutyCycle = 0.0;
        if (m_period > 0.0)
        {
            m_frequency = (long)(1000000.0 / m_period);
            m_dutyCycle = m_onCycle / m_period;
        }
    }
}

#endif
