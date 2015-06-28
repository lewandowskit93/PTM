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

APB1PeriphClock::APB1PeriphClock(uint32_t RCC_APB1Periph) :
    IDevice(), _RCC_APB1Periph(RCC_APB1Periph)
{
  enable();
}

APB1PeriphClock::~APB1PeriphClock()
{
  disable();
}

void APB1PeriphClock::enable()
{
  _enabled = true;
  RCC_APB1PeriphClockCmd(_RCC_APB1Periph, ENABLE);
}

void APB1PeriphClock::disable()
{
  _enabled = false;
  RCC_APB1PeriphClockCmd(_RCC_APB1Periph, DISABLE);
}

bool APB1PeriphClock::isEnabled()
{
  return _enabled;
}


APB2PeriphClock::APB2PeriphClock(uint32_t RCC_APB2Periph) :
    IDevice(), _RCC_APB2Periph(RCC_APB2Periph)
{
  enable();
}

APB2PeriphClock::~APB2PeriphClock()
{
  disable();
}

void APB2PeriphClock::enable()
{
  _enabled = true;
  RCC_APB2PeriphClockCmd(_RCC_APB2Periph, ENABLE);
}

void APB2PeriphClock::disable()
{
  _enabled = false;
  RCC_APB2PeriphClockCmd(_RCC_APB2Periph, DISABLE);
}

bool APB2PeriphClock::isEnabled()
{
  return _enabled;
}


} // namespace devices
} // namespace ptm
