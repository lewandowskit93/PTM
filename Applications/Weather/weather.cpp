#include "weather.hpp"

namespace ptm {
namespace applications {
namespace weather {

Signs::Signs(uint32_t x, uint32_t y) :
		Signs(x, y, false) {

}

Signs::Signs(uint32_t x, uint32_t y, bool flip_x) :
		gui::Component(x, y, 10, 7), _current_frame(0), _reverse(false), _flip_x(
				flip_x) {

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
	temp = temp % 100;
	temp = temp % 10;

	return temp;
}

void Signs::paintUnity(u16 x)
{
	switch (unity(x))
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		}
}
    void Signs::paintTens(u16 x)
    {
    	switch (tens(x))
    			{
    			case 0:
    				break;
    			case 1:
    				break;
    			case 2:
    				break;
    			case 3:
    				break;
    			case 4:
    				break;
    			case 5:
    				break;
    			case 7:
    				break;

    			case 8:
    				break;
    			case 9:
    				break;
    			}
    }
    void Signs::paintHundreds(u16 x)
    {
    	switch (hundreds(x)) //
    				{
    				case 0:
    					break;
    				case 1:
    					break;
    				case 2:
    					break;
    				case 3:
    					break;
    				case 4:
    					break;
    				case 5:
    					break;
    				case 7:
    					break;
    				case 8:
    					break;
    				case 9:
    					break;
    				}
    }
void Signs::paintTemp(u16 x) {
	auto Temp_w = ptm::system::System::getInstance()->_device_manager.getDevice<ptm::devices::DHT>();
	auto Temp = Temp_w.lock();
	u16 temp = 0;
	if(Temp)
	{
		temp=Temp->getTemperature();
	}

	//narysowanie napisu temp
	paintTens(temp);
	paintUnity(temp);


}

void Signs::paintRh(u16 x) {
	auto Rh_w = ptm::system::System::getInstance()->_device_manager.getDevice<ptm::devices::DHT>();
	auto Rh = Rh_w.lock();
	u16 rh=0;
	if(Rh)
	{
		rh = Rh->getHumidity();
	}

	//narysowanie napisu rh
	paintHundreds(rh);
	paintTens(rh);
	paintUnity(rh);

//narysowane %

}

void Signs::paintOn(gui::Canvas *canvas) {

	if (!canvas)
	    return;
	  for (uint32_t i = 0; i < getWidth(); ++i)
	  {
	    for (uint32_t j = 0; j < getHeight(); ++j)
	    {
	    	 canvas->drawPixel(i, j);
	    }
	  }
}





Weather::Weather() :
		Application(), _main_panel(0, 0, 84, 48), _canvas(84, 48), _display(
				system::System::getInstance()->_device_manager.getDevice<
						devices::displays::IDisplay>()), _screen_timer(
				&_timer_manager, 40, std::bind(&Weather::onScreenUpdate, this),
				true) {

}

Weather::~Weather() {

}

void Weather::onUpdate() {

}

void Weather::onStart() {
	//? _main_panel.addChild(&_signs);
	_canvas.clear();
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
	_main_panel.paintOn(&_canvas);
	_canvas.repaint(0, 0, _display);
	_screen_timer.start();
}

}
}
}
