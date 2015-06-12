#include "../Devices/IDevice.hpp"

namespace ptm
{
namespace devices
{

IDevice::IDevice()
{
}

IDevice::~IDevice()
{
}

Pin::Pin(GPIO_TypeDef* port, uint32_t pin) :
    port(port), pin(pin)
{

}

PinAFMapping::PinAFMapping(Pin pin, uint8_t pin_source, uint8_t alternate_function)
 : pin(pin), pin_source(pin_source), alternate_function(alternate_function)
{

}

} // namespace devices
} // namespace ptm
