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
#include "GUI/Canvas.hpp"
#include "GUI/Panel.hpp"
#include "GUI/Component.hpp"
#include "Devices/Accelometer.hpp"

using namespace ptm::system;
using namespace ptm::events;
using namespace ptm::devices;
using namespace ptm::interrupts;
using namespace ptm::gui;

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

    class Box : public Component
    {
      public:
        Box(uint32_t x, uint32_t y) : Component(x,y,2,2)
        {

        }
        virtual ~Box()
        {

        }

        virtual void paintOn(Canvas * canvas)
        {
          if(canvas)
          {
            canvas->drawPixel(0,0);
            canvas->drawPixel(0,1);
            canvas->drawPixel(1,0);
            canvas->drawPixel(1,1);
          }
        }
      protected:
      private:
    };

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
        Application(), _timer(ManagedTimer(&_timer_manager, 300, false)),_canvas(83,48),_box(0,0), _box2(18,9), _panel(1,0,83,48), _inner_panel(10,10,20,20)
    {
    }

    void onStart()
    {
      auto leds = System::getInstance()->_device_manager.getDevices<LED>();
      leds[0].lock()->off();
      _box.setX(0);
      _box.setY(0);
      _panel.addChild(&_inner_panel);
      _inner_panel.addChild(&_box);
      _inner_panel.addChild(&_box2);
      switchContext(&_context);
      _timer.start();
    }

    void onUpdate()
    {
      {
        auto screen_w = System::getInstance()->_device_manager.getDevice<
            displays::monochromatic::IMonochromaticDisplay>();
        auto screen_s = screen_w.lock();
        _panel.paintOn(&_canvas);
        _canvas.repaint(_panel.getX(),_panel.getY(),screen_w);
        screen_s->togglePixel(0,0);
      }
      if (_timer.hasFinished())
      {
        _box.setY((_box.getY()+1)%_inner_panel.getHeight());
        auto leds = System::getInstance()->_device_manager.getDevices<LED>();
        leds[0].lock()->toggle();
        _timer.start();
      }
      System::getInstance()->sleep(80);
      _box.setX((_box.getX()+1)%_inner_panel.getWidth());
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
    MonochromaticCanvas _canvas;
    Box _box;
    Box _box2;
    Panel _panel;
    Panel _inner_panel;
};
/////////////////////////////////////////TESTS
class Accel: public Application
{
  public:

    class AccelContext : public ApplicationContext
    {
      public:
        AccelContext() :
            ApplicationContext(), _timer(
                ManagedTimer(&_timer_manager, 300, false))
        {
          _event_listener.registerEventHandler(
              EventMapping(EVENT_ACC_IN_MENU,
                  std::bind(&AccelContext::handleButton, this,
                      std::placeholders::_1)));
        }

        void handleButton(std::shared_ptr<Event> event)
        {
          static int i = 0;
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          auto button_event = std::static_pointer_cast < AccelometerMenuEvent > (event);
          if (button_event->movedTop())
          {
        	  leds[3].lock()->toggle();
          }
          if (button_event->movedDown())
          {
        	  leds[2].lock()->toggle();
          }
        }

        void onUpdate()
        {
          if (_timer.hasFinished())
          {
            _timer.start();
          }
        }

        void onStart()
        {
          _timer.start();
        }

        void onPause()
        {
        }
        void onResume()
        {
        }

        void onStop()
        {
        }

        bool _led_two_state;
        bool _led_three_state;
        ManagedTimer _timer;
    };

    Accel() :
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
      System::getInstance()->sleep(80);
    }

    void onPause()
    {

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
    AccelContext _context;
    ManagedTimer _timer;
    uint32_t _x;
    uint32_t _y;
};
///////////////////////////////////////////////////
////////////////////////////////////////////////
////Accel in game event
class AccelInGame: public Application
{
  public:

    class AccelContext : public ApplicationContext
    {
      public:
        AccelContext() :
            ApplicationContext(), _timer(
                ManagedTimer(&_timer_manager, 300, false))
        {
          _event_listener.registerEventHandler(
              EventMapping(EVENT_ACC_IN_GAME,
                  std::bind(&AccelContext::handleButton, this,
                      std::placeholders::_1)));
        }

        void handleButton(std::shared_ptr<Event> event)
        {
          static int i = 0;
          auto leds = System::getInstance()->_device_manager.getDevices<LED>();
          auto button_event = std::static_pointer_cast < AccelometerGetPositionEvent > (event);
          ptm::axisXY xy = button_event->getPosition();
          if (xy.x > 30)
        	  leds[3].lock()->toggle();
          if (xy.y > 30)
        	  leds[2].lock()->toggle();
          //debugging xy
        }

        void onUpdate()
        {
          if (_timer.hasFinished())
          {
            _timer.start();
          }
        }

        void onStart()
        {
          _timer.start();
        }

        void onPause()
        {
        }
        void onResume()
        {
        }

        void onStop()
        {
        }

        bool _led_two_state;
        bool _led_three_state;
        ManagedTimer _timer;
    };

    AccelInGame() :
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
      System::getInstance()->sleep(80);
    }

    void onPause()
    {

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
    AccelContext _context;
    ManagedTimer _timer;
    uint32_t _x;
    uint32_t _y;
};

////////////////////////////////////////////////
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

  ////Accelometer
  //Pins used: 5 6 7: A ; 3, 0, 1: E
  System::getInstance()->_device_manager.mountDevice<APB2PeriphClock>(
		  RCC_APB2Periph_SPI1);
  System::getInstance()->_device_manager.mountDevice<AHB1PeriphClock>(
 		  RCC_AHB1Periph_GPIOE);

  std::weak_ptr<Accelometer> acc =
		  System::getInstance()->_device_manager.mountDevice<Accelometer>(
			  SPI1,
			  PinAFMapping(Pin(GPIOA, GPIO_Pin_5), GPIO_PinSource5, GPIO_AF_SPI1),
			  PinAFMapping(Pin(GPIOA, GPIO_Pin_6), GPIO_PinSource6, GPIO_AF_SPI1),
			  PinAFMapping(Pin(GPIOA, GPIO_Pin_7), GPIO_PinSource7, GPIO_AF_SPI1),
			  Pin(GPIOE, GPIO_Pin_3),
			  Pin(GPIOE, GPIO_Pin_0),
			  Pin(GPIOE, GPIO_Pin_1));


  //
  ////
  System::getInstance()->_interrupt_manager.addInterrupt<ButtonInterrupt>(
      but_w);
  System::getInstance()->_device_manager.getDevices<LED>();
  System::getInstance()->runApplication<InitApp>();
  System::getInstance()->run();
  while (true)
  {

  }
}
