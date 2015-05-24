#ifndef __Interrupt_H__
#define __Interrupt_H__

#include "misc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"

/*
 * Interrupt handler abstract class.
 * Handlers for interrupts should override its handleInterrupt function.
 * Classes that derive from AInterrupt should have protected/private constructor
 * and declare friendship with InterruptManager, so that handlers can only be created through
 * manager.
 */
class AInterrupt
{
  public:
    friend class InterruptManager;
    virtual ~AInterrupt();
    IRQn_Type getChannel();
    bool isChannelEnabled();
    virtual void handleInterrupt()=0;
  protected:
    AInterrupt(IRQn_Type channel, uint8_t priority = 0,
        uint8_t subpriority = 0);

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
    friend class InterruptManager;
    virtual ~AEXTInterrupt();
    bool isLineEnabled();
  protected:
    AEXTInterrupt(IRQn_Type channel, uint32_t line, uint8_t exti_port_source,
        uint8_t exti_pin_source, uint8_t priority = 0, uint8_t subpriority = 0,
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

#endif
