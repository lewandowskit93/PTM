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
#include "Applications/Flappy_Ptero/FlappyPtero.hpp"
#include "Devices/Accelometer.hpp"
#include "GUI/SliderMenu.hpp"
#include "Applications/Sun_space_mission/SunSpaceGame.hpp"
#include "Applications/Czujka/czujnik.hpp"
#include "Devices/Dht11.hpp"
#include "Devices/TimClk.hpp"
#include "Applications/Weather/weather.hpp"


using namespace ptm::system;
using namespace ptm::events;
using namespace ptm::devices;
using namespace ptm::interrupts;
using namespace ptm::gui;

class MainApp: public Application {
public:
	MainApp() :
			Application(), _screen_timer(
					ManagedTimer(&_timer_manager, 40,
							std::bind(&MainApp::onScreenUpdate, this), true)), _canvas(
					84, 48), _display(
					System::getInstance()->_device_manager.getDevice<
							displays::IDisplay>()), _panel(0, 0, 84, 48), _menu(
					0, 0, 84, 48), _spaceship(0,0), _ptero(0, 0), _weather(0,0) {
		_event_listener.registerEventHandler(
				EventMapping(EVENT_ACC_IN_MENU,
						std::bind(&MainApp::handleAccelometer, this,
								std::placeholders::_1)));
		_event_listener.registerEventHandler(
				EventMapping(EVENT_BUTTON,
						std::bind(&MainApp::handleButton, this,
								std::placeholders::_1)));
	}

	void handleAccelometer(std::shared_ptr<Event> event) {
		auto accel_event = std::static_pointer_cast < AccelometerMenuEvent
				> (event);
		if (accel_event->movedTop()) {
			_menu.selectPrevious();
		}
		if (accel_event->movedDown()) {
			_menu.selectNext();
		}
	}

	void handleButton(std::shared_ptr<Event> event) {
		auto button_event = std::static_pointer_cast < ButtonEvent > (event);
		if (button_event->isPressed()) {
		} else {
			uint32_t selected = _menu.getSelected();
			switch (selected) {
			// run proper apps
			case 1: {
				System::getInstance()->runApplication<ptm::applications::spacegame::SpaceshipGame>();
								break;
			}
			case 2: {
				System::getInstance()->runApplication<
										ptm::applications::flappy_ptero::FlappyPteroGame>();
								break;
			}
			case 3: {
				System::getInstance()->runApplication<ptm::applications::weather::Weather>();
				break;
			}
			default: {
			}
			}
		}
	}

	void onUpdate() {
	}

	void onStart() {
		_panel.addChild(&_menu);
		//add childrens that represents apps to menu
		_menu.addChild(&_spaceship);
		_menu.addChild(&_ptero);
		_menu.addChild(&_weather);
		_screen_timer.start();
	}

	void onPause() {
		_screen_timer.pause();
	}
	void onResume() {
		_screen_timer.resume();
	}

	void onStop() {
		_screen_timer.stop();
	}

	void onScreenUpdate() {
		_canvas.clear();
		_panel.paintOn(&_canvas);
		_canvas.repaint(0, 0, _display);
		_screen_timer.start();
	}

	ManagedTimer _screen_timer;
	MonochromaticCanvas _canvas;
	std::weak_ptr<displays::IDisplay> _display;
	Panel _panel;
	SliderMenu _menu;
	ptm::applications::spacegame::Spaceship _spaceship;
	ptm::applications::flappy_ptero::Ptero _ptero;
	ptm::applications::weather::MenuSigns _weather;
};

int main(void) {
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
	System::getInstance()->_device_manager.mountDevice<APB1PeriphClock>(
			  RCC_APB1Periph_TIM5);//fisza
	System::getInstance()->_device_manager.mountDevice<ptm::devices::Timer>(
			  84000000,84,TIM5); //fisza
	System::getInstance()->_device_manager.mountDevice<DHT>(
			  Pin(GPIOD, GPIO_Pin_1),TIM5);//fisza
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
			PinAFMapping(Pin(GPIOB, GPIO_Pin_10), GPIO_PinSource10,
					GPIO_AF_SPI2), //sclk
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
					PinAFMapping(Pin(GPIOA, GPIO_Pin_5), GPIO_PinSource5,
							GPIO_AF_SPI1),
					PinAFMapping(Pin(GPIOA, GPIO_Pin_6), GPIO_PinSource6,
							GPIO_AF_SPI1),
					PinAFMapping(Pin(GPIOA, GPIO_Pin_7), GPIO_PinSource7,
							GPIO_AF_SPI1), Pin(GPIOE, GPIO_Pin_3),
					Pin(GPIOE, GPIO_Pin_0), Pin(GPIOE, GPIO_Pin_1));

	//
	////
	System::getInstance()->_interrupt_manager.addInterrupt<ButtonInterrupt>(
			but_w);
	System::getInstance()->_device_manager.getDevices<LED>();
	System::getInstance()->runApplication<MainApp>();
	System::getInstance()->run();
	while (true) {

	}
}
