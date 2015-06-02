#ifndef __Interrupt_H__
#define __Interrupt_H__

#include <c++/4.9.3/memory>
#include "misc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"

namespace ptm
{

namespace devices
{
class IDevice;
} // namespace devices

namespace system
{
class InterruptManager;
} // namespace system

namespace interrupts
{
/*
 * Interrupt handler abstract class.
 * Handlers for interrupts should override its handleInterrupt function.
 * Classes that derive from AInterrupt should have protected/private constructor
 * and declare friendship with ptm::system::InterruptManager, so that handlers can only be created through
 * manager.
 */
class AInterrupt
{
  public:
    friend class system::InterruptManager;
    virtual ~AInterrupt();
    IRQn_Type getChannel();
    bool isChannelEnabled();
    /*
     * Should be overrided to handle an interrupt and clear flags.
     */
    virtual void handleInterrupt()=0;
    /*
     * Returns the device that is linked to an interrupt.
     */
    std::weak_ptr<devices::IDevice> getDevice();
  protected:
    AInterrupt(std::weak_ptr<devices::IDevice> device, IRQn_Type channel,
        uint8_t priority = 1, uint8_t subpriority = 0);

    std::weak_ptr<devices::IDevice> _device;
    IRQn_Type _channel;
    uint8_t _priority;
    uint8_t _subpriority;
    bool _channel_enabled;
  private:
};

/*
 * Interrupt handler abstract class for external interrupts.
 */
class AEXTInterrupt : public AInterrupt
{
  public:
    friend class system::InterruptManager;
    virtual ~AEXTInterrupt();
    bool isLineEnabled();
  protected:
    AEXTInterrupt(std::weak_ptr<devices::IDevice> device, IRQn_Type channel,
        uint32_t line, uint8_t exti_port_source, uint8_t exti_pin_source,
        uint8_t priority = 1, uint8_t subpriority = 0,
        EXTITrigger_TypeDef trigger = EXTI_Trigger_Rising_Falling,
        EXTIMode_TypeDef mode = EXTI_Mode_Interrupt);

    uint32_t _line;
    uint8_t _exti_port_source;
    uint8_t _exti_pin_source;
    EXTITrigger_TypeDef _trigger;
    EXTIMode_TypeDef _mode;
    bool _line_enabled;
  private:
};

} //namespace interrupts

} //namespace ptm

#endif
