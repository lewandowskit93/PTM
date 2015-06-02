#include "../System/InterruptManager.hpp"

namespace ptm
{
namespace system
{

InterruptManager::InterruptManager()
{

}
InterruptManager::~InterruptManager()
{

}

void InterruptManager::defaultHandler()
{
  while (true)
  {
    //INFINITE LOOP
  }
}
void InterruptManager::handleInterrupt(IRQn_Type channel)
{
  auto handler = _interrupt_mappings.find(channel);
  if (handler != _interrupt_mappings.end())
  {
    if (handler->second)
    {
      handler->second->handleInterrupt();
    }
    else
    {
      defaultHandler();
    }
  }
  else
  {
    defaultHandler();
  }
}

} // namespace system
} // namespace ptm
