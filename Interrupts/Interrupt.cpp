#include "../Interrupts/Interrupt.hpp"

namespace ptm
{
namespace interrupts
{

AInterrupt::AInterrupt(std::weak_ptr<devices::IDevice> device,
    IRQn_Type channel, uint8_t priority, uint8_t subpriority) :
    _device(device), _channel(channel), _priority(priority), _subpriority(
        subpriority), _channel_enabled(true)
{
  NVIC_InitTypeDef NVIC_init;
  NVIC_init.NVIC_IRQChannel = _channel;
  NVIC_init.NVIC_IRQChannelPreemptionPriority = _priority;
  NVIC_init.NVIC_IRQChannelSubPriority = _subpriority;
  NVIC_init.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_init);
}
AInterrupt::~AInterrupt()
{
  NVIC_InitTypeDef NVIC_init;
  NVIC_init.NVIC_IRQChannel = _channel;
  NVIC_init.NVIC_IRQChannelPreemptionPriority = _priority;
  NVIC_init.NVIC_IRQChannelSubPriority = _subpriority;
  NVIC_init.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_init);
}

IRQn_Type AInterrupt::getChannel()
{
  return _channel;
}

bool AInterrupt::isChannelEnabled()
{
  return _channel_enabled;
}

std::weak_ptr<devices::IDevice> AInterrupt::getDevice()
{
  return _device;
}

AEXTInterrupt::AEXTInterrupt(std::weak_ptr<devices::IDevice> device,
    IRQn_Type channel, uint32_t line, uint8_t exti_port_source,
    uint8_t exti_pin_source, uint8_t priority, uint8_t subpriority,
    EXTITrigger_TypeDef trigger, EXTIMode_TypeDef mode) :
    AInterrupt(device, channel, priority, subpriority), _line(line), _exti_port_source(
        exti_port_source), _exti_pin_source(exti_pin_source), _trigger(trigger), _mode(
        mode), _line_enabled(true)
{
  EXTI_InitTypeDef EXTI_init;
  EXTI_init.EXTI_Line = _line;
  EXTI_init.EXTI_Mode = _mode;
  EXTI_init.EXTI_Trigger = _trigger;
  EXTI_init.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_init);

  SYSCFG_EXTILineConfig(_exti_port_source, _exti_pin_source);

  EXTI_ClearITPendingBit(_line);
}
AEXTInterrupt::~AEXTInterrupt()
{
  EXTI_InitTypeDef EXTI_init;
  EXTI_init.EXTI_Line = _line;
  EXTI_init.EXTI_Mode = _mode;
  EXTI_init.EXTI_Trigger = _trigger;
  EXTI_init.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_init);

  SYSCFG_EXTILineConfig(_exti_port_source, _exti_pin_source);

  EXTI_ClearITPendingBit(_line);
}
bool AEXTInterrupt::isLineEnabled()
{
  return _line_enabled;
}

} // namespace interrupts
} // namespace ptm
