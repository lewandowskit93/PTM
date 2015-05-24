#ifndef __LED_H__
#define __LED_H__

#include "IDevice.hpp"
#include "stm32f4xx_gpio.h"

/*
 * Simple LED Device Controller.
 */
class LED : public IDevice
{
  public:
    friend class DeviceManager;

    virtual ~LED();
    /*
     * Switches on the LED.
     */
    void on();
    /*
     * Switches off the LED.
     */
    void off();
    /*
     * Toggles the LED state.
     * on->off
     * off->on
     */
    void toggle();
    /*
     * Returns the state of the LED.
     * True when the LED is switched on or false otherwise.
     */
    bool isOn();

  protected:
    LED(GPIO_TypeDef* GPIOx, uint32_t pin);
  private:
    GPIO_TypeDef* _GPIOx;
    uint32_t _pin;
    bool _enabled;
};

#endif
