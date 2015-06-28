#include "czujnik.hpp"

namespace ptm
{
namespace applications
{
namespace czujka
{

Czujka::Czujka() :
    gui::Component(0, 0, 84, 48), _reverse(false)
{

}

Czujka::~Czujka()
{

}

void Czujka::paintOn(gui::Canvas *canvas)
{
  paintFrame(canvas);
}



void Czujka::paintFrame(gui::Canvas *canvas)
{

  if (canvas)
  {
    for (uint32_t i = 0; i < getWidth(); ++i)
    {
      for (uint32_t j = 0; j < getHeight(); ++j)
      {
        if (_czujka[i][j])
          canvas->drawPixel(i, j);
      }
    }
  }
}

void Czujka::paintMenu(gui::Canvas *canvas)
{

  if (canvas)
  {
    for (uint32_t i = 0; i < getWidth(); ++i)
    {
      for (uint32_t j = 0; j < getHeight(); ++j)
      {
        if (_czujka[i][j])
          canvas->drawPixel(i, j);
      }
    }
  }
}

CzujkaContext::CzujkaContext(CzujkaApp *app) :
    ApplicationContext(), _app(app), _app_panel(0, 0, 84, 48), _bg_timer(&_timer_manager, 120,
        std::bind(&CzujkaContext::onBgTimer, this), true), _playing(false)
{
  _event_listener.registerEventHandler(
      events::EventMapping(events::EVENT_BUTTON,
          std::bind(&CzujkaContext::onButton, this, std::placeholders::_1)));


}
CzujkaContext::~CzujkaContext()
{

}
void CzujkaContext::onUpdate()
{
}
void CzujkaContext::onStart()
{
  _playing = false;
  _app_panel.addChild(&_czujka);
  if (_app)
    _app->_main_panel.addChild(&_app_panel);
  _bg_timer.start();
}
void CzujkaContext::onPause()
{
  if (_app)
  {
    _app->_main_panel.removeChild(&_app_panel);
  }
  _bg_timer.pause();
}

void CzujkaContext::onResume()
{
  if (_app)
  {
    _app->_main_panel.addChild(&_app_panel);
  }
  _bg_timer.resume();
}

void CzujkaContext::onStop()
{
  if (_app)
  {
    _app->_main_panel.removeChild(&_app_panel);
  }
  _bg_timer.stop();
  _playing = false;
}

void CzujkaContext::onBgTimer()
{
  _bg_timer.start();
}


void CzujkaContext::onButton(std::shared_ptr<events::Event> event)
{
  std::shared_ptr<events::ButtonEvent> b_event = std::static_pointer_cast
      < events::ButtonEvent > (event);
  if (!b_event->isPressed())
  {
   if (_app)
   {
	   _app->stop();
   }

  }
}

CzujkaApp::CzujkaApp() :
    Application(), _main_panel(0, 0, 84, 48), _czujka_context(this), _canvas(84,
        48), _display(
        system::System::getInstance()->_device_manager.getDevice<
            devices::displays::IDisplay>()), _screen_timer(&_timer_manager, 40,
        std::bind(&CzujkaApp::onScreenUpdate, this), true)
{

}

CzujkaApp::~CzujkaApp()
{

}

void CzujkaApp::onUpdate()
{

}

void CzujkaApp::onStart()
{
  _canvas.clear();
  switchContext(&_czujka_context);
  _screen_timer.start();
}

void CzujkaApp::onPause()
{
  _screen_timer.pause();
}

void CzujkaApp::onResume()
{
  _screen_timer.resume();
}

void CzujkaApp::onStop()
{
  _screen_timer.stop();
}

void CzujkaApp::onScreenUpdate()
{
  _main_panel.paintOn(&_canvas);
  _canvas.repaint(0, 0, _display);
  _screen_timer.start();
}


}
}
}
