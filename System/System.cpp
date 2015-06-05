#include "../System/System.hpp"
#include "../System/Application.hpp"
#include "../Events/Events.hpp"
#include "cmsis_boot/stm32f4xx.h"

namespace ptm
{
namespace system
{

System* System::_instance = 0;
System::System() :
    _device_manager(DeviceManager()), _interrupt_manager(InterruptManager()), _event_manager(
        EventManager()), _timer_manager(TimerManager()), _time(0), _current_app(
        std::shared_ptr<Application>()), _running(false)
{
  SysTick_Config(SystemCoreClock / 1000);
  NVIC_SetPriority(SysTick_IRQn, 0);
}
System::~System()
{
}
System* System::getInstance()
{
  if (!_instance)
  {
    _instance = new System();
  }
  return _instance;
}

void System::nextAppFromStack()
{
  _current_app = std::shared_ptr<Application>();
  if (_applications_stack.empty())
  {
    _running = false;
  }
  else
  {
    _current_app = _applications_stack.top();
    _current_app->resume();
    _applications_stack.pop();
  }
}

void System::run()
{
  if (_running)
    return;
  else
  {
    _running = true;
    __enable_irq();
    while (_running)
    {
      step();
      System::sleep(1);
    }
    while (true)
    {
      //INFINITE LOOP
    }
  }
}

void System::step()
{
  if (_running)
  {
    __disable_irq();
    update();
    if (_current_app)
    {
      switch (_current_app->getApplicationState())
      {
        case ApplicationLifeState::INITIALIZED:
        {
          _current_app->start();
        }
        case ApplicationLifeState::RUNNABLE:
        {
          _current_app->update();
          break;
        }
        case ApplicationLifeState::PAUSED:
        case ApplicationLifeState::RUNNING:
        {
          break;
        }
        default:
        {
          nextAppFromStack();
        }
      }
    }
    else
    {
      nextAppFromStack();
    }
    __enable_irq();
  }
}

uint64_t System::getTime()
{
  return _time;
}
void System::setTime(uint64_t time)
{
  _time = time;
}
void System::addTime(uint64_t step)
{
  _time += step;
}

void System::incTime()
{
  ++_time;
}

void System::sleep(uint64_t millis)
{
  if(_running)
  {
    __enable_irq();
    uint64_t start_time = _time;
    uint64_t current_time = _time;
    while (current_time - start_time < millis)
    {
      current_time = _time;
    }
    __disable_irq();
  }

}

void System::update()
{
  //_device_manager.update();
  //_interrupt_manager.update();
  _event_manager.update();
  _timer_manager.update();
}

} // namespace system
} // namespace ptm

extern "C"
{
void SysTick_Handler(void)
{
  ptm::system::System::getInstance()->incTime();
}
}
