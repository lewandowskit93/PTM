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
        FirstContext(SecondApp* application) :
            ApplicationContext(), _application(application)
        {
          _event_listener.registerEventHandler(
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
            if (i == 1)
            {
              _application->switchContext(
                  &_application->_context2);
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

        void onStart()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          leds[1].lock()->off();
          leds[2].lock()->off();
        }

        void onPause()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          _led_one_state=leds[1].lock()->isOn();
          _led_two_state=leds[2].lock()->isOn();
        }

        void onResume()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          if (_led_one_state)
          {
            leds[1].lock()->on();
          }
          else
          {
            leds[1].lock()->off();
          }
          if (_led_two_state)
          {
            leds[2].lock()->on();
          }
          else
          {
            leds[2].lock()->off();
          }
        }

        void onStop()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          leds[1].lock()->off();
          leds[2].lock()->off();
        }
        bool _led_one_state;
        bool _led_two_state;
        SecondApp* _application;
    };

    class SecondContext : public ApplicationContext
    {
      public:
        SecondContext() :
            ApplicationContext()
        {
        }

        void onStart()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          leds[2].lock()->on();
        }

        void onUpdate()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          leds[2].lock()->toggle();
        }

        void onPause()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          _led_two_state = leds[2].lock()->isOn();
        }

        void onResume()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          if (_led_two_state)
          {
            leds[2].lock()->on();
          }
          else
          {
            leds[2].lock()->off();
          }
        }

        void onStop()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          leds[2].lock()->off();
        }

        bool _led_two_state;
    };

    SecondApp() :
        Application(), _context1(FirstContext(this))
    {
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      _event_listener.registerEventHandler(
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
        if (i == 3)
        {
          i = 0;
          stop();
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

    void onStart()
    {
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      leds[0].lock()->off();
      leds[3].lock()->off();
      switchContext(&_context1);
    }

    void onPause()
    {
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      _led_zero_state = leds[0].lock()->isOn();
      _led_three_state = leds[3].lock()->isOn();
    }
    void onResume()
    {
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      if (_led_zero_state)
      {
        leds[0].lock()->on();
      }
      else
      {
        leds[0].lock()->off();
      }

      if (_led_three_state)
      {
        leds[3].lock()->on();
      }
      else
      {
        leds[3].lock()->off();
      }
    }

    void onStop()
    {
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      leds[0].lock()->off();
      leds[3].lock()->off();
    }

    FirstContext _context1;
    SecondContext _context2;
    bool _led_zero_state;
    bool _led_three_state;
};

class InitApp : public Application
{
  public:

    class InitContext : public ApplicationContext
    {
      public:
        InitContext() :
            ApplicationContext()
        {
          _event_listener.registerEventHandler(
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
            System::getInstance()->runApplication<SecondApp>();
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

        void onStart()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          leds[1].lock()->on();
          leds[3].lock()->off();
        }

        void onPause()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          _led_one_state = leds[1].lock()->isOn();
          _led_three_state = leds[3].lock()->isOn();
        }
        void onResume()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          if (_led_one_state)
          {
            leds[1].lock()->on();
          }
          else
          {
            leds[1].lock()->off();
          }

          if (_led_three_state)
          {
            leds[3].lock()->on();
          }
          else
          {
            leds[3].lock()->off();
          }
        }

        void onStop()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          leds[1].lock()->off();
          leds[3].lock()->off();
        }
        bool _led_one_state;
        bool _led_three_state;
    };

    InitApp() :
        Application()
    {
    }

    void onStart()
    {
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      leds[0].lock()->off();
      switchContext(&_context);
    }

    void onUpdate()
    {
      Delay(3000000);
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      leds[0].lock()->toggle();
    }

    void onPause()
    {
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      _led_zero_state = leds[0].lock()->isOn();
    }

    void onResume()
    {
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      if (_led_zero_state)
      {
        leds[0].lock()->on();
      }
      else
      {
        leds[0].lock()->off();
      }
    }

    void onStop()
    {
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      leds[0].lock()->off();
    }

    bool _led_zero_state;
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

  System::getInstance()->runApplication<InitApp>();
  System::getInstance()->run();
  while (true)
  {

  }
}
