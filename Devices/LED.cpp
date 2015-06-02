#include "../Devices/LED.hpp"

namespace ptm
{
namespace devices
{

LED::LED(Pin pin) :
    IDevice(), _pin(pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = _pin.pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(_pin.port, &GPIO_InitStructure);
  off();
}

LED::~LED()
{

}

void LED::on()
{
  _enabled = true;
  GPIO_SetBits(_pin.port, _pin.pin);
}
void LED::off()
{
  _enabled = false;
  GPIO_ResetBits(_pin.port, _pin.pin);
}
void LED::toggle()
{
  _enabled = !_enabled;
  GPIO_ToggleBits(_pin.port, _pin.pin);
}
bool LED::isOn()
{
  return _enabled;
}

} // namespace devices
} // namespace ptm
