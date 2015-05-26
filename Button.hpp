#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "IDevice.hpp"
#include "Interrupt.hpp"
#include "Events.hpp"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"

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
    Pin getPin();

  protected:
    Button(Pin pin);
  private:
    Pin _pin;
};

/*
 * Handles button interrupts.
 */
class ButtonInterrupt : public AEXTInterrupt
{
  public:
    friend class InterruptManager;
    /*
     * Fires events:
     * ButtonEvent - Pressed or Released.
     */
    void handleInterrupt();
  protected:
    ButtonInterrupt(std::weak_ptr<Button> device, IRQn_Type channel = EXTI0_IRQn, uint32_t line = EXTI_Line0, uint8_t exti_port_source = EXTI_PortSourceGPIOA,
        uint8_t exti_pin_source = EXTI_PinSource0, uint8_t priority = 0, uint8_t subpriority = 0);
  private:
    /*
     * Gets rid of contact bouncing.
     */
    void debounce();

};

/*
 * Event class related to button events.
 */
class ButtonEvent : public Event
{
  public:
    ButtonEvent(bool pressed, std::weak_ptr<Button> _device);
    ~ButtonEvent();
    /*
     * Returns true if the event is related to the button pressing event
     * or false when it is related to the button release event.
     */
    bool isPressed() const;
    /*
     * Returns button device that generated the event.
     */
    std::weak_ptr<Button> getDevice() const;
  protected:
    bool _pressed;
    std::weak_ptr<Button> _device;
};

#endif
