#include "LED.hpp"

LED::LED(GPIO_TypeDef* GPIOx, uint32_t pin) :
    IDevice(), _GPIOx(GPIOx), _pin(pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = _pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(_GPIOx, &GPIO_InitStructure);
  off();
}

LED::~LED()
{

}

void LED::on()
{
  _enabled = true;
  GPIO_SetBits(_GPIOx, _pin);
}
void LED::off()
{
  _enabled = false;
  GPIO_ResetBits(_GPIOx, _pin);
}
void LED::toggle()
{
  _enabled = !_enabled;
  GPIO_ToggleBits(_GPIOx, _pin);
}
bool LED::isOn()
{
  return _enabled;
}
