

#ifndef MCU_Master_h
#define MCU_Master_h

#include <Arduino.h>
#include <Wire.h>

#include "Bitwise.h"
#include "BusI2C.h"
#include "MCU_Common.h"

// IN DEVELOMENT ------------------------------- FIXME
namespace dsg
{
    class MCU_Master
    {
    private:
        dsg::BusI2C bus;
        nmr::Bitwise<int> util;

    public:
        MCU_Master() = default;

        String IsAddressI2C(uint8_t address);
    };

    String MCU_Master::IsAddressI2C(uint8_t address)
    {
        return bus.IsDevice(address);
    }

} // namespace dsg

#endif