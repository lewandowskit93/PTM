#include "Interrupts.hpp"
#include "System.hpp"

extern "C"
{
void EXTI0_IRQHandler(void)
{

  if (EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    System::getInstance()->_interrupt_manager.handleInterrupt((EXTI0_IRQn));
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}
}
