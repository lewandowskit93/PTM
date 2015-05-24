#include "Button.hpp"
Button::Button(GPIO_TypeDef* GPIOx, uint32_t pin) :
    IDevice(), _GPIOx(GPIOx), _pin(pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = _pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(_GPIOx, &GPIO_InitStructure);
}

Button::~Button()
{

}

bool Button::isPressed()
{
  return GPIO_ReadInputDataBit(_GPIOx, _pin) != 0;
}
