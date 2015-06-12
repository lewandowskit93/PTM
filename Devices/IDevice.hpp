#ifndef __IDEVICE_H__
#define __IDEVICE_H__

#include "stm32f4xx_gpio.h"

namespace ptm
{
namespace devices
{

/* Device marker.
 * Classes that Derive from IDevice should declare friendship with ptm::system::DeviceManager.
 * Constructor of such a Device shall be protected or private so that devices can be
 * created only through manager.*/
class IDevice
{
  public:
    virtual ~IDevice();
  protected:
    IDevice();
  private:
};

/*
 * Structure that describes single pin on specific port.
 */
struct Pin
{
    Pin(GPIO_TypeDef* port, uint32_t pin);
    /*
     * GPIOx port
     */
    GPIO_TypeDef* port;
    /*
     * GPIO_Pin
     */
    uint32_t pin;
};

struct PinAFMapping
{
    PinAFMapping(Pin pin, uint8_t pin_source, uint8_t alternate_function);
    Pin pin;
    uint8_t pin_source;
    uint8_t alternate_function;
};

} //namespace devices
} //namespace ptm
#endif // __IDEVICE_H__
