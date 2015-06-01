#ifndef __RCC_H__
#define __RCC_H__

#include "stm32f4xx_rcc.h"
#include "IDevice.hpp"

namespace ptm
{
namespace system
{
class DeviceManager;
} // namespace system

namespace devices
{

/*
 * AHB1PeriphClocks controller.
 */
class AHB1PeriphClock : public IDevice
{
  public:
    friend class system::DeviceManager;

    virtual ~AHB1PeriphClock();
    /*
     * Switches on the clock.
     */
    void enable();
    /*
     * Switches off the clock.
     */
    void disable();
    /*
     * Returns true if the clock is enabled.
     */
    bool isEnabled();
  protected:
    /*
     * Creates and switches on the clock for given AHB1Periph.
     */
    AHB1PeriphClock(uint32_t RCC_AHB1Periph);
  private:
    uint32_t _RCC_AHB1Periph;
    bool _enabled;
};

} //namespace device
} //namespace ptm
#endif
