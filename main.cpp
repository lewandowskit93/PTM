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
#include "Application.hpp"

void Delay(int i)
{
  while (i > 0)
  {
    --i;
  }
}

class SecondApp : public Application
{
  public:
    class FirstContext : public ApplicationContext
    {
      public:
        FirstContext(Application* application) :
            ApplicationContext(application)
        {
          registerEventHandler(
              EventMapping(EVENT_BUTTON, true,
                  std::bind(&FirstContext::handleButton, this,
                      std::placeholders::_1)));
        }

        void handleButton(std::shared_ptr<Event> event)
        {
          static int i = 0;
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          auto button_event = std::static_pointer_cast < ButtonEvent > (event);
          if (button_event->isPressed())
          {
            leds[2].lock()->on();
          }
          else
          {
            leds[2].lock()->off();
            if (i == 2)
            {
              _application->switchContext(
                  &((SecondApp*) _application)->_context2);
              i = 0;
            }
            else
              i++;
          }
        }

        void onUpdate()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          leds[1].lock()->toggle();
        }
    };

    class SecondContext : public ApplicationContext
    {
      public:
        SecondContext(Application* application) :
            ApplicationContext(application)
        {
        }

        void onUpdate()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          leds[2].lock()->toggle();
        }
    };

    SecondApp() :
        Application(), _context1(FirstContext(this)), _context2(
            SecondContext(this))
    {
      switchContext(&_context1);
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      for (auto led : leds)
      {
        led.lock()->off();
      }
      registerEventHandler(
          EventMapping(EVENT_BUTTON, true,
              std::bind(&SecondApp::handleButton, this,
                  std::placeholders::_1)));
    }

    void handleButton(std::shared_ptr<Event> event)
    {
      static int i = 0;
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      auto button_event = std::static_pointer_cast < ButtonEvent > (event);
      if (button_event->isPressed())
      {
        leds[3].lock()->on();
      }
      else
      {
        leds[3].lock()->off();
        if (i == 1)
        {
          i = 0;
          finish();
        }
        else
          i++;
      }
    }

    void onUpdate()
    {
      Delay(3000000);
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      leds[0].lock()->toggle();
    }

    FirstContext _context1;
    SecondContext _context2;
};

class InitApp : public Application
{
  public:

    class InitContext : public ApplicationContext
    {
      public:
        InitContext(Application* application) :
            ApplicationContext(application)
        {
          registerEventHandler(
              EventMapping(EVENT_EXTI0_IRQn, true,
                  std::bind(&InitContext::handleButton, this,
                      std::placeholders::_1)));
        }

        void handleButton(std::shared_ptr<Event> event)
        {
          static int i = 0;
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          leds[3].lock()->toggle();
          if (i == 3)
          {
            System::getInstance()->startApplication<SecondApp>();
            i = 0;
          }
          else
            ++i;
        }

        void onUpdate()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          leds[1].lock()->toggle();
        }
    };

    InitApp() :
        Application(), _context(InitContext(this))
    {
      switchContext(&_context);
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      leds[1].lock()->on();
    }

    void onUpdate()
    {
      Delay(3000000);
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      leds[0].lock()->toggle();
    }

    InitContext _context;
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
  System::getInstance()->_device_manager.mountDevice<LED>(GPIOD, GPIO_Pin_14);
  System::getInstance()->_device_manager.mountDevice<LED>(GPIOD, GPIO_Pin_15);
  std::weak_ptr<Button> but_w =
      System::getInstance()->_device_manager.mountDevice<Button>(GPIOA,
          GPIO_Pin_0);
  System::getInstance()->_interrupt_manager.addInterrupt<ButtonInterrupt>();
  std::shared_ptr<Button> but_d = but_w.lock();
  std::vector < std::weak_ptr<LED> > leds =
      System::getInstance()->_device_manager.getDevices<LED>();

  System::getInstance()->startApplication<InitApp>();
  System::getInstance()->run();
  while (true)
  {

  }
}
