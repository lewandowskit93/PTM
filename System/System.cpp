#include "../System/System.hpp"
#include "../System/Application.hpp"
#include "../Events/Events.hpp"

namespace ptm
{
namespace system
{

System* System::_instance = 0;
System::System() :
   _device_manager(DeviceManager()), _interrupt_manager(InterruptManager()), _event_manager(EventManager()), _time(0), _current_app(
        std::shared_ptr<Application>()), _running(false)
{
  SysTick_Config(SystemCoreClock/1000);
  NVIC_SetPriority(SysTick_IRQn,0);
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
    while (_running)
    {
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
    }
    while (true)
    {
      //INFINITE LOOP
    }
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
  _time+=step;
}

void System::incTime()
{
  ++_time;
}

void System::sleep(uint64_t millis)
{
  uint64_t start_time = _time;
  uint64_t current_time = _time;
  while(current_time-start_time < millis)
  {
    current_time = _time;
  }
}

} // namespace system
} // namespace ptm

extern "C"{
void SysTick_Handler(void)  {
  ptm::system::System::getInstance()->incTime();
}
}
