#include "Application.hpp"
ApplicationContext::ApplicationContext(Application *application)
: _application(application)
{

}
ApplicationContext::~ApplicationContext()
{

}
void ApplicationContext::update()
{
  handleEvents();
  onUpdate();
}

Application::Application()
: _current_context(0), _finished(false)
{

}
Application::~Application()
{

}
void Application::update()
{
  if(_finished)return;
  handleEvents();
  onUpdate();
  if(_current_context!=0)
  {
    _current_context->update();
  }
  else
  {
    finish();
  }
}

void Application::finish()
{
  _finished=true;
}

bool Application::hasFinished(){
  return _finished;
}

void Application::switchContext(ApplicationContext* _new_context)
{
  _current_context=_new_context;
}
