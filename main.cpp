#include "stm32f4xx.h"
#include "system_stm32f4xx.h"

#include <c++/4.9.3/memory>
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "DeviceManager.hpp"
#include "LED.hpp"
#include "Button.hpp"
#include "RCC.hpp"
#include "Interrupt.hpp"
#include "InterruptManager.hpp"
#include "System.hpp"
#include "Interrupts.hpp"

void Delay(int i)
{
  while (i > 0)
  {
    --i;
  }
}

class ButtonInterrupt : public AEXTInterrupt
{
  public:
    friend class InterruptManager;
    void handleInterrupt()
    {
      auto but = System::getInstance()->_device_manager.getDevice<Button>();
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      if (leds.size() >= 2)
      {
        Delay(100);
        if (but.lock()->isPressed())
        {
          leds[0].lock()->on();
          leds[1].lock()->off();
        }
        else
        {
          leds[1].lock()->on();
          leds[0].lock()->off();
        }
      }
    }
  protected:
    ButtonInterrupt() :
        AEXTInterrupt(EXTI0_IRQn, EXTI_Line0, EXTI_PortSourceGPIOA,
            EXTI_PinSource0)
    {

    }

};

int main(void)
{
  SystemInit();
  SystemCoreClockUpdate();
  System::getInstance()->_device_manager.mountDevice<AHB1PeriphClock>(
      RCC_AHB1Periph_GPIOD);
  System::getInstance()->_device_manager.mountDevice<AHB1PeriphClock>(
      RCC_AHB1Periph_GPIOA);
  System::getInstance()->_device_manager.mountDevice<LED>(GPIOD, GPIO_Pin_12);
  System::getInstance()->_device_manager.mountDevice<LED>(GPIOD, GPIO_Pin_13);
  std::weak_ptr<Button> but_w =
      System::getInstance()->_device_manager.mountDevice<Button>(GPIOA,
          GPIO_Pin_0);
  System::getInstance()->_interrupt_manager.addInterrupt<ButtonInterrupt>();
  std::shared_ptr<Button> but_d = but_w.lock();
  std::vector < std::weak_ptr<LED> > leds =
      System::getInstance()->_device_manager.getDevices<LED>();
  while (true)
  {

  }
}
