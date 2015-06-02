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

} // namespace devices
} // namespace ptm
