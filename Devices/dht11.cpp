#include "../Devices/Dht11.hpp"

namespace ptm {
namespace devices {

GPIO_InitTypeDef GPIO_InitStructure;


DHT::DHT(Pin pin, TIM_TypeDef* TIMx) :
		IDevice(), _pin(pin), _TIMx(TIMx)
{

	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//jest w mainie
	GPIO_InitStructure.GPIO_Pin = _pin.pin; //GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(pin.port, &GPIO_InitStructure); //GPIO_Init(GPIOD,&GPIO_InitStructure);


}
DHT::~DHT()
{

}

Pin DHT::getPin()
{
	return _pin;
}
/*
void DHT::dhtTim5Init(void)
{
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);//jest w mainie
	//TIM_TimeBaseStructure.TIM_Period = 84000000 - 1;
	//TIM_TimeBaseStructure.TIM_Prescaler = 84;
	//TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	//TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	//TIM_Cmd(TIM5, ENABLE);
}
*/
void DHT::dhtGpioOutInit(void)
{
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//jest w mainie
	GPIO_InitStructure.GPIO_Pin =getPin().pin;// GPIO_Pin_1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(getPin().port, &GPIO_InitStructure); //GPIOD
}
void DHT::dhtGpioInInit(void)
{
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //jest w mainie
	GPIO_InitStructure.GPIO_Pin = getPin().pin;// GPIO_Pin_1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(getPin().port, &GPIO_InitStructure); //GPIOD
}
void DHT::dhtDelay(int us)
{
	_TIMx->CNT = 0;
	while ((_TIMx->CNT) <= us)
		;
}
void DHT::dhtRead()
{

	u8 tmp, j, i, tab[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
	dhtGpioOutInit();
	GPIO_ResetBits(getPin().port, getPin().pin); //(GPIOD, GPIO_Pin_1);
	dhtDelay(18000);
	GPIO_SetBits(getPin().port, getPin().pin); //(GPIOD, GPIO_Pin_1);
	dhtDelay(40);
	dhtGpioInInit();

	while (!GPIO_ReadInputDataBit(getPin().port, getPin().pin))  //(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1))
		;
	while (GPIO_ReadInputDataBit(getPin().port, getPin().pin))  //(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1))
		;

	for (i = 0; i < 5; ++i) {
		for (j = 0; j < 8; ++j) {
			while (!GPIO_ReadInputDataBit(getPin().port, getPin().pin))  //(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1))
				;
			TIM_SetCounter(TIM5, 0);
			while (GPIO_ReadInputDataBit(getPin().port, getPin().pin))  //(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1))
				;
			tmp = TIM_GetCounter(TIM5);
			if (tmp < 30) { // trwanie sygna³u <30us-> 0; ok. 70us -> 1;
				tab[i] = tab[i] << 1;
			} else {
				tab[i] = tab[i] << 1;
				tab[i] += 1;
			}
		}
	}
	setHumidity(tab[0]);
	setTemperature(tab[2]);
	setCheskSum(tab[4]);

}

void DHT::setHumidity(u16 rh)
{
	_humidity=rh;
}

void DHT::setTemperature(u16 temp)
{
	_temperature=temp;
}

void DHT::setCheskSum(u16 sum)
{
	_checkSum=sum;
}

u16 DHT::getCheckSum()
{
	return _checkSum;
}

u16 DHT::getHumidity()
{
	//dhtRead();
	return _humidity;
}

u16 DHT::getTemperature()
{
	//dhtRead();
	return _temperature;
}


} //namepsace devices
} //namepsace ptm
