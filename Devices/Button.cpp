#include "../Devices/Button.hpp"
#include "../System/System.hpp"
#include "../Events/Events.hpp"

namespace ptm
{
namespace devices
{

Button::Button(Pin pin) :
    IDevice(), _pin(pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = _pin.pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(pin.port, &GPIO_InitStructure);
}

Button::~Button()
{

}

bool Button::isPressed()
{
  return GPIO_ReadInputDataBit(_pin.port, _pin.pin) != 0;
}

Pin Button::getPin()
{
  return _pin;
}

} // namespace devices

namespace interrupts
{

ButtonInterrupt::ButtonInterrupt(std::weak_ptr<devices::Button> device,
    IRQn_Type channel, uint32_t line, uint8_t exti_port_source,
    uint8_t exti_pin_source, uint8_t priority, uint8_t subpriority) :
    AEXTInterrupt(device, channel, line, exti_port_source, exti_pin_source,
        priority, subpriority, EXTI_Trigger_Rising_Falling, EXTI_Mode_Interrupt)
{

}

void ButtonInterrupt::handleInterrupt()
{
  if (EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    debounce();
    EXTI_ClearITPendingBit(EXTI_Line0);
    auto device_s = _device.lock();
    if (device_s)
    {
      auto button_s = std::dynamic_pointer_cast < devices::Button > (device_s);
      if (button_s)
      {
        system::System::getInstance()->_event_manager.raiseEvent(
            std::shared_ptr < events::ButtonEvent
                > (new events::ButtonEvent(button_s->isPressed(), button_s)));
      }
    }
  }
}

void ButtonInterrupt::debounce()
{
  int i = 500;
  while (i > 0)
  {
    --i;
  }
  //ToDo: with timers
}

} // namespace interrupts

namespace events
{

ButtonEvent::ButtonEvent(bool pressed, std::weak_ptr<devices::Button> device) :
    Event(EventType::EVENT_BUTTON), _pressed(pressed), _device(device)
{

}

ButtonEvent::~ButtonEvent()
{

}
bool ButtonEvent::isPressed() const
{
  return _pressed;
}

std::weak_ptr<devices::Button> ButtonEvent::getDevice() const
{
  return _device;
}

} // namespace events
} // namespace ptm
