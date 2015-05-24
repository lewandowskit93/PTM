#ifndef __INTERRUPT_MANAGER_H__
#define __INTERRUPT_MANAGER_H__

#include <c++/4.9.3/map>
#include <c++/4.9.3/memory>
#include "stm32f4xx.h"
#include "Interrupt.hpp"

class InterruptManager
{
  public:
    InterruptManager();
    virtual ~InterruptManager();
    template<class InterruptT, class ... ArgsT>
    void addInterrupt(const ArgsT& ... args);
    //template <class InterruptT, class ... ArgsT>
    //void addInterrupt(ArgsT&& ... args);
    void handleInterrupt(IRQn_Type channel);
  protected:
  private:
    void defaultHandler();
    std::map<IRQn_Type, std::shared_ptr<AInterrupt> > _interrupt_mappings;
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

#endif
