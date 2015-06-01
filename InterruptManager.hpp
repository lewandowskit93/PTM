#ifndef __INTERRUPT_MANAGER_H__
#define __INTERRUPT_MANAGER_H__

#include <c++/4.9.3/map>
#include <c++/4.9.3/memory>
#include "stm32f4xx.h"
#include "Interrupt.hpp"

namespace ptm
{
namespace system
{

/*
 * Manages Interrupt handlers.
 */
class InterruptManager
{
  public:
    InterruptManager();
    virtual ~InterruptManager();
    /*
     * Creates and maps new interrupt handler.
     */
    template<class InterruptT, class ... ArgsT>
    void addInterrupt(const ArgsT& ... args);
    //template <class InterruptT, class ... ArgsT>
    //void addInterrupt(ArgsT&& ... args);

    /*
     * Handles any interrupt.
     * The function invokes handler for the channel when the handler is mapped,
     * or invokes default handler when the one is not provided.
     */
    void handleInterrupt(IRQn_Type channel);
  protected:
  private:
    /*
     * Default handler for unknown and unexpected interrupts.
     * Default action is to go into infinite loop.
     */
    void defaultHandler();
    /*
     * Maps channel to the handler.
     */
    std::map<IRQn_Type, std::shared_ptr<interrupts::AInterrupt> > _interrupt_mappings;
};

template<class InterruptT, class ... ArgsT>
void InterruptManager::addInterrupt(const ArgsT& ... args)
{
  auto handler = std::shared_ptr < InterruptT > (new InterruptT(args...));
  if (handler)
    _interrupt_mappings.insert(
        std::pair<IRQn_Type, std::shared_ptr<InterruptT>>(handler->getChannel(),
            handler));
}

/*template <class InterruptT, class ... ArgsT>
 void InterruptManager::addInterrupt(ArgsT&& ... args)
 {
 auto handler = std::shared_ptr<InterruptT>(new InterruptT(args...));
 if(handler) _interrupt_mappings.insert(std::pair<IRQn_Type,std::shared_ptr<InterruptT>>(handler->getChannel(),handler));
 }*/

} //namespace system
} //namespace ptm

#endif
