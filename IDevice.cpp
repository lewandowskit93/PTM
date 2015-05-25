#include "IDevice.hpp"

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
