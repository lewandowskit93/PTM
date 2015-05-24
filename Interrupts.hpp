#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

/*
* Basic IRQ Handlers should be provided for every Interrupt.
* These handlers should delegate interrupt handling to the system and clear the flag.
*/

extern "C"
{
void EXTI0_IRQHandler(void);
}

#endif
