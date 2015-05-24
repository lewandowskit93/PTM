#ifndef __LED_H__
#define __LED_H__

#include "IDevice.hpp"
#include "stm32f4xx_gpio.h"

class LED : public IDevice
{
  public:
    friend class DeviceManager;

    virtual ~LED();
    void on();
    void off();
    void toggle();
    bool isOn();

  protected:
    LED(GPIO_TypeDef* GPIOx, uint32_t pin);
  private:
    GPIO_TypeDef* _GPIOx;
    uint32_t _pin;
    bool _enabled;
};

#endif
