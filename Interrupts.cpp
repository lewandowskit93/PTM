#include "Interrupts.hpp"
#include "System.hpp"

extern "C"
{
void EXTI0_IRQHandler(void)
{
  ptm::system::System::getInstance()->_interrupt_manager.handleInterrupt(
      (EXTI0_IRQn));
}
}
