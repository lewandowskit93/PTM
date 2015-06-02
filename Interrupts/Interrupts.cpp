#include "../Interrupts/Interrupts.hpp"
#include "../System/System.hpp"

extern "C"
{
void EXTI0_IRQHandler(void)
{
  ptm::system::System::getInstance()->_interrupt_manager.handleInterrupt(
      (EXTI0_IRQn));
}
}
