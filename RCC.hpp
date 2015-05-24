#ifndef __RCC_H__
#define __RCC_H__

#include "stm32f4xx_rcc.h"
#include "IDevice.hpp"

class AHB1PeriphClock : public IDevice
{
  public:
    friend class DeviceManager;

    virtual ~AHB1PeriphClock();
    void enable();
    void disable();
    bool isEnabled();
  protected:
    AHB1PeriphClock(uint32_t RCC_AHB1Periph);
  private:
    uint32_t _RCC_AHB1Periph;
    bool _enabled;
};

#endif
