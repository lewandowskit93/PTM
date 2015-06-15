#include "../System/Application.hpp"
#include "../System/System.hpp"

namespace ptm
{
namespace system
{

ApplicationContext::ApplicationContext() :
    _app_state(ApplicationLifeState::UNINITIALIZED)
{
  _app_state = ApplicationLifeState::INITIALIZED;
}
ApplicationContext::~ApplicationContext()
{

}

void ApplicationContext::start()
{
  if (_app_state == ApplicationLifeState::INITIALIZED
      || _app_state == ApplicationLifeState::STOPPED)
  {
    _event_listener.activate();
    onStart();
    _app_state = ApplicationLifeState::RUNNABLE;
    _last_update_time = System::getInstance()->getTime();
  }
}

void ApplicationContext::update()
{
  if (_app_state == ApplicationLifeState::RUNNABLE)
  {
    _app_state = ApplicationLifeState::RUNNING;
    handleEvents();
    handleTimers();
    if (_app_state == ApplicationLifeState::RUNNING)
      onUpdate();
    if (_app_state == ApplicationLifeState::RUNNING)
      _app_state = ApplicationLifeState::RUNNABLE;
    _last_update_time = System::getInstance()->getTime();
  }
}

void ApplicationContext::handleEvents()
{
  while (_app_state == ApplicationLifeState::RUNNING)
  {
    if (!_event_listener.handleNextEvent())
      break;
  }
  if (_app_state != ApplicationLifeState::RUNNING)
    _event_listener.removeEvents();
}

void ApplicationContext::handleTimers()
{
  auto timers = _timer_manager.getTimers();
  for (auto timer : timers)
  {
    if (_app_state == ApplicationLifeState::RUNNING)
      timer->update();
    else
      break;
  }
}

void ApplicationContext::pause()
{
  if (_app_state == ApplicationLifeState::RUNNING
      || _app_state == ApplicationLifeState::RUNNABLE)
  {
    _event_listener.deactivate();
    onPause();
    _app_state = ApplicationLifeState::PAUSED;
  }
}
void ApplicationContext::resume()
{
  if (_app_state == ApplicationLifeState::PAUSED)
  {
    _event_listener.activate();
    onResume();
    _app_state = ApplicationLifeState::RUNNABLE;
    _last_update_time = System::getInstance()->getTime();
  }
}
void ApplicationContext::stop()
{
  if (_app_state == ApplicationLifeState::RUNNING
      || _app_state == ApplicationLifeState::RUNNABLE || _app_state == ApplicationLifeState::PAUSED)
  {
    _event_listener.deactivate();
    onStop();
    _app_state = ApplicationLifeState::STOPPED;
  }
}

ApplicationLifeState::ApplicationLifeState ApplicationContext::getApplicationState()
{
  return _app_state;
}

Application::Application() :
    _current_context(0), _app_state(ApplicationLifeState::UNINITIALIZED)
{
  _app_state = ApplicationLifeState::INITIALIZED;
}
Application::~Application()
{

}

void Application::start()
{
  if (_app_state == ApplicationLifeState::INITIALIZED
      || _app_state == ApplicationLifeState::STOPPED)
  {
    _event_listener.activate();
    onStart();
    if (_current_context)
      _current_context->start();
    _app_state = ApplicationLifeState::RUNNABLE;
    _last_update_time = System::getInstance()->getTime();
  }
}

void Application::update()
{
  if (_app_state == ApplicationLifeState::RUNNABLE)
  {
    _app_state = ApplicationLifeState::RUNNING;
    handleEvents();
    handleTimers();
    if (_app_state == ApplicationLifeState::RUNNING)
      onUpdate();
    if (_current_context && _app_state == ApplicationLifeState::RUNNING)
      _current_context->update();
    if (_app_state == ApplicationLifeState::RUNNING)
      _app_state = ApplicationLifeState::RUNNABLE;
    _last_update_time = System::getInstance()->getTime();
  }
}

void Application::handleEvents()
{
  while (_app_state == ApplicationLifeState::RUNNING)
  {
    if (!_event_listener.handleNextEvent())
      break;
  }
  if (_app_state != ApplicationLifeState::RUNNING)
    _event_listener.removeEvents();
}

void Application::handleTimers()
{
  auto timers = _timer_manager.getTimers();
  for (auto timer : timers)
  {
    if (_app_state == ApplicationLifeState::RUNNING)
      timer->update();
    else
      break;
  }
}

void Application::pause()
{
  if (_app_state == ApplicationLifeState::RUNNING
      || _app_state == ApplicationLifeState::RUNNABLE)
  {
    _event_listener.deactivate();
    onPause();
    if (_current_context)
      _current_context->pause();
    _app_state = ApplicationLifeState::PAUSED;
  }
}

void Application::resume()
{
  if (_app_state == ApplicationLifeState::PAUSED)
  {
    _event_listener.activate();
    onResume();
    if (_current_context)
      _current_context->resume();
    _app_state = ApplicationLifeState::RUNNABLE;
    _last_update_time = System::getInstance()->getTime();
  }
}

void Application::stop()
{
  if (_app_state == ApplicationLifeState::RUNNING
      || _app_state == ApplicationLifeState::RUNNABLE || _app_state == ApplicationLifeState::PAUSED)
  {
    _event_listener.deactivate();
    onStop();
    if (_current_context)
      _current_context->stop();
    _app_state = ApplicationLifeState::STOPPED;
  }
}

void Application::terminate()
{
  _app_state = ApplicationLifeState::TERMINATED;
}

void Application::switchContext(ApplicationContext* _new_context)
{

  if (_current_context)
    _current_context->pause();
  _current_context = _new_context;
  if (_current_context)
  {
    if (_current_context->getApplicationState()
        == ApplicationLifeState::INITIALIZED
        || _current_context->getApplicationState()
            == ApplicationLifeState::STOPPED)
      _current_context->start();
    else if (_current_context->getApplicationState()
        == ApplicationLifeState::PAUSED)
      _current_context->resume();
  }
}

ApplicationLifeState::ApplicationLifeState Application::getApplicationState()
{
  return _app_state;
}

} // namespace system
} // namespace ptm
