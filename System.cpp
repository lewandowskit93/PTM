#include "System.hpp"
#include "Application.hpp"
#include "Events.hpp"

System* System::_instance = 0;
System::System() :
    _device_manager(DeviceManager()), _interrupt_manager(InterruptManager()), _current_app(
        std::shared_ptr<Application>()), _running(false)
{

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
