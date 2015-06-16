#include "../Devices/TimClk.hpp"

namespace ptm
{
namespace devices
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

Timer::Timer(uint32_t period, uint16_t prescaler, TIM_TypeDef* TIMx) :
    IDevice(), _period(period), _prescaler(prescaler), _TIMx(TIMx)
{

		TIM_TimeBaseStructure.TIM_Period = period - 1; //84000000
		TIM_TimeBaseStructure.TIM_Prescaler = prescaler; //84
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
		//TIM_Cmd(TIM5, ENABLE);
  enable(TIMx);
}

Timer::~Timer()
{
  disable(_TIMx);
}

void Timer::enable(TIM_TypeDef* TIMx)
{
  _enabled = true;
  TIM_Cmd(TIMx, ENABLE);
}

void Timer::disable(TIM_TypeDef* TIMx)
{
  _enabled = false;
  TIM_Cmd(TIMx, DISABLE);
}

bool Timer::isEnabled(TIM_TypeDef* TIMx)
{
  return _enabled;
}

}//namespace ptm
}//namespace devices
