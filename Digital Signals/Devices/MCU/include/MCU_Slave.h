
#ifndef MCU_Slave_h
#define MCU_Slave_h

#include <Arduino.h>
#include <Wire.h>

#include "Bitwise.h"
#include "BusI2C.h"
#include "MCU_Common.h"

// IN DEVELOMENT ------------------------------- FIXME
namespace dsg
{
    class MCU_Slave
    {
    private:
        dsg::BusI2C bus;
        nmr::Bitwise<int> util;

    public:
        MCU_Slave() = default;
    };

} // namespace dsg

#endif