#ifndef __TIMCLK_H__
#define __TIMCLK_H__

#include "../Devices/IDevice.hpp"
#include "stm32f4xx_tim.h"

namespace ptm
{
namespace system
{
class DeviceManager;
}//namespace system
namespace devices
{
class Timer : public IDevice
{
public:
	friend class system::DeviceManager;

	virtual ~Timer();

	void enable(TIM_TypeDef* TIMx);

	void disable(TIM_TypeDef* TIMx);

	bool isEnabled(TIM_TypeDef* TIMx);

protected:
		Timer(uint32_t period, uint16_t prescaler, TIM_TypeDef* TIMx);
private:
		uint32_t _period;
		uint16_t _prescaler;
		TIM_TypeDef* _TIMx;
		bool _enabled;


};
}//namespace devices

}//namespace ptm

#endif
