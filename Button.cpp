#include "Button.hpp"
#include "System.hpp"
#include "Events.hpp"

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

uint32_t Button::getPin()
{
  return _pin;
}

ButtonInterrupt::ButtonInterrupt(EventType e_type, IRQn_Type channel, uint32_t line, uint8_t exti_port_source,
    uint8_t exti_pin_source, uint8_t priority, uint8_t subpriority) :
    AEXTInterrupt(channel,line,exti_port_source,exti_pin_source,priority,subpriority,EXTI_Trigger_Rising_Falling,EXTI_Mode_Interrupt),
    _e_type(e_type)
{

}

void ButtonInterrupt::handleInterrupt()
{
  debounce();
  System::getInstance()->_event_manager.fireEvent(
      std::shared_ptr < Event > (new Event(_e_type)));
  auto buttons = System::getInstance()->_device_manager.getDevices<Button>();
  for(auto button : buttons)
  {
    auto button_s = button.lock();
    if(!button_s)continue;
    if(button_s->getPin()==_line)
    {
      System::getInstance()->_event_manager.fireEvent(
            std::shared_ptr < ButtonEvent > (new ButtonEvent(button_s->isPressed(),button)));
      break;
    }
  }
}

void ButtonInterrupt::debounce()
{
  int i=50;
  while(i>0)
  {
    --i;
  }
}


ButtonEvent::ButtonEvent(bool pressed, std::weak_ptr<Button> device)
: Event(EventType::EVENT_BUTTON), _pressed(pressed), _device(device)
{

}
ButtonEvent::~ButtonEvent()
{

}
bool ButtonEvent::isPressed() const
{
  return _pressed;
}

std::weak_ptr<Button> ButtonEvent::getDevice() const
{
  return _device;
}
