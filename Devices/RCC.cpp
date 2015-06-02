#include "../Devices/RCC.hpp"

namespace ptm
{
namespace devices
{

AHB1PeriphClock::AHB1PeriphClock(uint32_t RCC_AHB1Periph) :
    IDevice(), _RCC_AHB1Periph(RCC_AHB1Periph)
{
  enable();
}

AHB1PeriphClock::~AHB1PeriphClock()
{
  disable();
}

void AHB1PeriphClock::enable()
{
  _enabled = true;
  RCC_AHB1PeriphClockCmd(_RCC_AHB1Periph, ENABLE);
}

void AHB1PeriphClock::disable()
{
  _enabled = false;
  RCC_AHB1PeriphClockCmd(_RCC_AHB1Periph, DISABLE);
}

bool AHB1PeriphClock::isEnabled()
{
  return _enabled;
}

} // namespace devices
} // namespace ptm
