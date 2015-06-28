#include "weather.hpp"

namespace ptm {
namespace applications {
namespace weather {

MenuSigns::MenuSigns(uint32_t x, uint32_t y) : gui::Component(0,0,84,48)
{

}

MenuSigns::~MenuSigns(){};

void MenuSigns::paintOn(gui::Canvas *canvas) {

	if (!canvas)
	    return;
	  for (int i = 0 ; i < 84 ; i++)
	  {
		  for (int j = 0 ; j < 48 ; j++)
		  {
			  if (_czujka[i][j])
				  canvas->drawPixel(i,j);

		  }

	  }
}



Signs::Signs(uint32_t x, uint32_t y) :
		Signs(x, y, false) {

}

Signs::Signs(uint32_t x, uint32_t y, bool flip_x) :
		gui::Component(0, 0, 84, 48),  _reverse(false), _flip_x(flip_x)
{

}




Signs::~Signs() {

}

u8 Signs::hundreds(u16 x) {
	return ((x) / 100);
}


u8 Signs::tens(u16 x) {
	int temp = x;
	temp = temp % 100;
	temp = temp / 10;
	return temp;
}

u8 Signs::unity(u16 x) {
	int temp = x;
	temp = temp % 10;
	return temp;
}

void Signs::paintDigitLoop(bool digit_array[8][12], uint32_t x, uint32_t y, gui::Canvas *canvas)
{
	for (int i = 0 ; i < 8 ; i++)
	{
		for (int j = 0 ; j < 12 ; j++)
		{
			if (digit_array[i][j] == true)
			{
				canvas->drawPixel(i+x, j+y);
			}
		}

	}

}

void Signs::paintDigit(u8 digit, uint32_t x, uint32_t y, gui::Canvas *canvas)
{
	switch (digit)
	{
	case 0:
		paintDigitLoop(_zero, x, y, canvas);
			break;
	case 1:
		paintDigitLoop(_one, x, y, canvas);
			break;
	case 2:
		paintDigitLoop(_two, x, y, canvas);
			break;
	case 3:
		paintDigitLoop(_three, x, y, canvas);
			break;
	case 4:
		paintDigitLoop(_four, x, y, canvas);
			break;
	case 5:
		paintDigitLoop(_five, x, y, canvas);
			break;
	case 6:
		paintDigitLoop(_six, x, y, canvas);
			break;
	case 7:
		paintDigitLoop(_seven, x, y, canvas);
			break;
	case 8:
		paintDigitLoop(_eight, x, y, canvas);
			break;
	case 9:
		paintDigitLoop(_nine, x, y, canvas);
			break;



	}
}

void Signs::paintTemp(uint32_t x, uint32_t y, gui::Canvas *canvas)
{
	auto Temp_w = ptm::system::System::getInstance()->_device_manager.getDevice<ptm::devices::DHT>();
	auto Temp = Temp_w.lock();
	u16 temp = 0;
	if(Temp)
	{
		temp=Temp->getTemperature();
	}


	//narysowanie napisu temp
	for (int i = 0 ; i < 41 ; i ++)
	{
		for (int j = 0 ; j < 15 ; j ++)
		{
			if (_temp[i][j] == true) canvas->drawPixel(x+i, y+j);
		}

	}
	x = x + 50;
	paintDigit(tens(temp), x, y, canvas);
	paintDigit(unity(temp), x+10, y, canvas);


	//paintTens(temp);
	//paintUnity(temp);


}

void Signs::paintRh(uint32_t x, uint32_t y, gui::Canvas *canvas)
{
	auto Rh_w = ptm::system::System::getInstance()->_device_manager.getDevice<ptm::devices::DHT>();
	auto Rh = Rh_w.lock();
	u16 rh=0;
	if(Rh)
	{
		rh = Rh->getHumidity();
	}

	//narysowanie napisu rh
	for (int i = 0 ; i < 22 ; i ++)
		{
			for (int j = 0 ; j < 12 ; j ++)
			{
				if (_rh[i][j] == true) canvas->drawPixel(x+i, y+j);
			}

		}

	x = x + 27;
	paintDigit(hundreds(rh), x, y, canvas);
	x+=10;
	paintDigit(tens(rh), x, y, canvas);
	x+=10;
	paintDigit(unity(rh), x,y, canvas);
	x+=10;

	for (int i = 0 ; i < 13 ; i ++)
	{
		for (int j = 0 ; j < 12 ; j++)
		{
			if (_percent[i][j]) canvas -> drawPixel(x+i, y+j);
		}
	}

//narysowane %

}

void Signs::paintOn(gui::Canvas *canvas) {

	if (!canvas)
	    return;
	  paintRh(0,0, canvas);
	  paintTemp(0,24, canvas);
}







WeatherContext::WeatherContext(Weather *app) :
    ApplicationContext(), _weather(app), _app_panel(0, 0, 84, 48), _bg_timer(&_timer_manager, 120,
        std::bind(&WeatherContext::onBgTimer, this), true), _signs(0,0), _playing(false)
{
  _event_listener.registerEventHandler(
      events::EventMapping(events::EVENT_BUTTON,
          std::bind(&WeatherContext::onButton, this, std::placeholders::_1)));


}

WeatherContext::~WeatherContext()
{

}

void WeatherContext::onUpdate()
{
}

void WeatherContext::onStart()
{
  _playing = false;
  _app_panel.addChild(&_signs);
  if (_weather)
	  _weather->_main_panel.addChild(&_app_panel);
  _bg_timer.start();
}

void WeatherContext::onPause()
{
  if (_weather)
  {
    _weather->_main_panel.removeChild(&_app_panel);
  }
  _bg_timer.pause();
}

void WeatherContext::onResume()
{
  if (_weather)
  {
    _weather->_main_panel.addChild(&_app_panel);
  }
  _bg_timer.resume();
}

void WeatherContext::onStop()
{
  if (_weather)
  {
    _weather->_main_panel.removeChild(&_app_panel);
  }
  _bg_timer.stop();
  _playing = false;
}

void WeatherContext::onBgTimer()
{
  _bg_timer.start();
}


void WeatherContext::onButton(std::shared_ptr<events::Event> event)
{
  std::shared_ptr<events::ButtonEvent> b_event = std::static_pointer_cast
      < events::ButtonEvent > (event);
  if (!b_event->isPressed())
  {
   if (_weather)
   {
	   _weather->stop();
   }

  }
}



Weather::Weather() :
		Application(), _main_panel(0, 0, 84, 48), _weather_context(this), _canvas(84, 48), _display(
				system::System::getInstance()->_device_manager.getDevice<
						devices::displays::IDisplay>()), _screen_timer(
				&_timer_manager, 40, std::bind(&Weather::onScreenUpdate, this),true)
{

}

Weather::~Weather() {

}

void Weather::onUpdate() {

}

void Weather::onStart() {
	_canvas.clear();
	switchContext(&_weather_context);
	auto Rh_w = ptm::system::System::getInstance()->_device_manager.getDevice<ptm::devices::DHT>();
	auto Rh = Rh_w.lock();
	if(Rh){
		Rh->dhtRead();
	}
	_screen_timer.start();
}

void Weather::onPause() {
	_screen_timer.pause();
}

void Weather::onResume() {
	_screen_timer.resume();
}

void Weather::onStop() {
	_screen_timer.stop();
}

void Weather::onScreenUpdate() {
	_canvas.clear();
	_main_panel.paintOn(&_canvas);
	_canvas.repaint(0, 0, _display);
	_screen_timer.start();
}

}
}
}
