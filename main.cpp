#include "stm32f4xx.h"
#include "system_stm32f4xx.h"

#include <c++/4.9.3/memory>
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "Events/Events.hpp"
#include "System/DeviceManager.hpp"
#include "Devices/LED.hpp"
#include "Devices/Button.hpp"
#include "Devices/RCC.hpp"
#include "Interrupts/Interrupt.hpp"
#include "System/InterruptManager.hpp"
#include "System/System.hpp"
#include "Interrupts/Interrupts.hpp"
#include "System/Application.hpp"
#include "System/ManagedTimer.hpp"
#include "Devices/Display.hpp"

using namespace ptm::system;
using namespace ptm::events;
using namespace ptm::devices;
using namespace ptm::interrupts;

class SecondApp : public Application
{
  public:
    class FirstContext : public ApplicationContext
    {
      public:
        FirstContext(SecondApp* application) :
            ApplicationContext(), _application(application), _timer(
                ManagedTimer(&_timer_manager, 100, false))
        {
          _event_listener.registerEventHandler(
              EventMapping(EVENT_BUTTON,
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
              _application->switchContext(&_application->_context2);
              i = 0;
            }
            else
              i++;
          }
        }

        void onUpdate()
        {
          if (_timer.hasFinished())
          {
            auto leds =
                System::getInstance()->_device_manager.getDevices<LED>();
            leds[1].lock()->toggle();
            _timer.start();
          }
        }

        void onStart()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          leds[1].lock()->off();
          leds[2].lock()->off();
          _timer.start();
        }

        void onPause()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          _led_one_state = leds[1].lock()->isOn();
          _led_two_state = leds[2].lock()->isOn();
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
        ManagedTimer _timer;
    };

    class SecondContext : public ApplicationContext
    {
      public:
        SecondContext() :
            ApplicationContext(), _timer(
                ManagedTimer(&_timer_manager, 500,
                    std::bind(&SecondContext::onTimer, this), false))
        {
        }

        void onStart()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          leds[2].lock()->on();
          _timer.start();
        }

        void onUpdate()
        {
          if (_timer.hasFinished())
          {
            _timer.invoke();
          }
          //auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          //leds[2].lock()->toggle();
        }

        void onTimer()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          leds[2].lock()->toggle();
          _timer.start();
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
        ManagedTimer _timer;
        bool _led_two_state;
    };

    SecondApp() :
        Application(), _context1(FirstContext(this)), _timer(
            ManagedTimer(&_timer_manager, 1000,
                std::bind(&SecondApp::onTimer, this), true))
    {
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      _event_listener.registerEventHandler(
          EventMapping(EVENT_BUTTON,
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
      //System::getInstance()->sleep(300);
      //auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      //leds[0].lock()->toggle();
    }

    void onTimer()
    {
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      leds[0].lock()->toggle();
      _timer.start();
    }

    void onStart()
    {
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      leds[0].lock()->off();
      leds[3].lock()->off();
      switchContext(&_context1);
      _timer.start();
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
    ManagedTimer _timer;
};

class InitApp : public Application
{
  public:

    class InitContext : public ApplicationContext
    {
      public:
        InitContext() :
            ApplicationContext(), _timer(
                ManagedTimer(&_timer_manager, 300, false))
        {
          _event_listener.registerEventHandler(
              EventMapping(EVENT_BUTTON,
                  std::bind(&InitContext::handleButton, this,
                      std::placeholders::_1)));
        }

        void handleButton(std::shared_ptr<Event> event)
        {
          static int i = 0;
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          auto button_event = std::static_pointer_cast < ButtonEvent > (event);
          leds[3].lock()->toggle();
          if (!button_event->isPressed())
          {
            if (i == 1)
            {
              System::getInstance()->runApplication<SecondApp>();
              i = 0;
            }
            else
              ++i;
          }
        }

        void onUpdate()
        {
          if (_timer.hasFinished())
          {
            auto leds =
                System::getInstance()->_device_manager.getDevices<LED>();
            leds[1].lock()->toggle();
            _timer.start();
          }
        }

        void onStart()
        {
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          leds[1].lock()->on();
          leds[3].lock()->off();
          _timer.start();
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
        ManagedTimer _timer;
    };

    InitApp() :
        Application(), _timer(ManagedTimer(&_timer_manager, 300, false)), _x(0), _y(
            0)
    {
    }

    void onStart()
    {
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      leds[0].lock()->off();
      _x = 0;
      _y = 0;
      switchContext(&_context);
      _timer.start();
    }

    void onUpdate()
    {
      {
        auto screen_w = System::getInstance()->_device_manager.getDevice<
            displays::monochromatic::IMonochromaticDisplay>();
        auto screen_s = screen_w.lock();
        screen_s->clearScreen();
        screen_s->setPixel(_x % 84, _y % 48);
        screen_s->setPixel((_x + 1) % 84, (_y + 1) % 48);
        screen_s->setPixel(_x % 84, (_y + 1) % 48);
        screen_s->setPixel((_x + 1) % 84, _y % 48);
        screen_s->refreshScreen();
      }
      if (_timer.hasFinished())
      {
        ++_y;
        _y %= 48;
        auto leds = System::getInstance()->_device_manager.getDevices<LED>();
        leds[0].lock()->toggle();
        _timer.start();
      }
      System::getInstance()->sleep(80);
      ++_x;
      _x %= 84;
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
    ManagedTimer _timer;
    uint32_t _x;
    uint32_t _y;
};

int main(void)
{
  SystemInit();
  SystemCoreClockUpdate();
  __disable_irq();
  System::getInstance()->_device_manager.mountDevice<AHB1PeriphClock>(
      RCC_AHB1Periph_GPIOD);
  System::getInstance()->_device_manager.mountDevice<AHB1PeriphClock>(
      RCC_AHB1Periph_GPIOA);
  System::getInstance()->_device_manager.mountDevice<AHB1PeriphClock>(
      RCC_AHB1Periph_GPIOB);
  System::getInstance()->_device_manager.mountDevice<AHB1PeriphClock>(
      RCC_AHB1Periph_GPIOC);
  System::getInstance()->_device_manager.mountDevice<APB1PeriphClock>(
      RCC_APB1Periph_SPI2);
  System::getInstance()->_device_manager.mountDevice<LED>(
      Pin(GPIOD, GPIO_Pin_12));
  System::getInstance()->_device_manager.mountDevice<LED>(
      Pin(GPIOD, GPIO_Pin_13));
  System::getInstance()->_device_manager.mountDevice<LED>(
      Pin(GPIOD, GPIO_Pin_14));
  System::getInstance()->_device_manager.mountDevice<LED>(
      Pin(GPIOD, GPIO_Pin_15));
  std::weak_ptr<Button> but_w =
      System::getInstance()->_device_manager.mountDevice<Button>(
          Pin(GPIOA, GPIO_Pin_0));
  System::getInstance()->_device_manager.mountDevice<
      displays::monochromatic::PCD8544::PCD8544>(SPI2,
      PinAFMapping(Pin(GPIOC, GPIO_Pin_3), GPIO_PinSource3, GPIO_AF_SPI2), //mosi
      PinAFMapping(Pin(GPIOB, GPIO_Pin_10), GPIO_PinSource10, GPIO_AF_SPI2), //sclk
      Pin(GPIOC, GPIO_Pin_14), Pin(GPIOC, GPIO_Pin_13), //dc, ce
      Pin(GPIOC, GPIO_Pin_15)); //rst
  System::getInstance()->_interrupt_manager.addInterrupt<ButtonInterrupt>(
      but_w);
  System::getInstance()->_device_manager.getDevices<LED>();
  System::getInstance()->runApplication<InitApp>();
  System::getInstance()->run();
  while (true)
  {

  }
}
