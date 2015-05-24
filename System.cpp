#include "System.hpp"
#include "Application.hpp"
#include "Events.hpp"

System* System::_instance = 0;
System::System() :
    _device_manager(DeviceManager()), _interrupt_manager(InterruptManager()), _current_app(std::shared_ptr<Application>()), _running(false)
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
  _current_app=std::shared_ptr<Application>();
  if(_applications_stack.empty())
  {
    _running=false;
  }
  else
  {
    _current_app=_applications_stack.top();
    _applications_stack.pop();
  }
}

void System::run()
{
  if(_running)return;
  else
  {
    _running=true;
    while(_running)
    {
      if(_current_app)
      {
        if(!_current_app->hasFinished())
        {
          _current_app->update();
        }
        else
        {
          nextAppFromStack();
        }
      }
      else
      {
        nextAppFromStack();
      }
    }
    while(true)
    {
      //INFINITE LOOP
    }
  }
}

void System::fireEvent(std::shared_ptr<Event> event)
{
  if(_current_app)
  {
    if(_current_app->_current_context!=0)
    {
      if(_current_app->_current_context->supportsEvent(event->getType()))
      {
        _current_app->_current_context->queueEvent(event);
      }
      else if(_current_app->supportsEvent(event->getType()))
      {
        _current_app->queueEvent(event);
      }
    }
  }
}
