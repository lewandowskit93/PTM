#ifndef __DHT11_H__
#define __DHT11_H__

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_rcc.h"
#include "../Devices/IDevice.hpp"
#include "../Devices/TimClk.hpp"
namespace ptm {
namespace system {
class DeviceManager;
}

namespace devices {
class DHT: public IDevice {
public:
	friend class system::DeviceManager;
	virtual ~DHT();
	void dhtRead(); //odczytuje dane z czujnika
	//void dhtTim5Init(void); //inicjalizuje timer 5
	Pin getPin();
	u16 getTemperature();//zwraca temperature
	u16 getHumidity();////zwraca wilgotnosc
	u16 getCheckSum();//zwraca sumê kontrolna
protected:
	DHT(Pin pin, TIM_TypeDef* TIMx);

private:
	void dhtGpioInInit(void); //inicjalizuje OneWire jako in
	void dhtGpioOutInit(void); //inicjalizuje OneWire jako on
	void dhtDelay(int us); //wprowadza opoznienie do odczytu
	Pin _pin;
	u16 _temperature, _humidity,_checkSum;
	TIM_TypeDef* _TIMx;
	void setTemperature(u16 temp);
	void setHumidity(u16 rh);
	void setCheskSum(u16 sum);
};

} //namespace devices
} //namespace ptm

#endif
