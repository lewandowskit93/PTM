#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "IDevice.hpp"
#include "stm32f4xx_gpio.h"

/*
 * Simple Button Device Controller.
 */
class Button : public IDevice
{
  public:
    friend class DeviceManager;
    virtual ~Button();
    /*
     * Returns true if button is pressed.
     */
    bool isPressed();

  protected:
    Button(GPIO_TypeDef* GPIOx, uint32_t pin);
  private:
    GPIO_TypeDef* _GPIOx;
    uint32_t _pin;
};

#endif
