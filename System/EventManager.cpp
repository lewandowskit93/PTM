#include "../System/EventManager.hpp"

namespace ptm
{
namespace system
{

EventManager::EventManager()
{

}
EventManager::~EventManager()
{

}

void EventManager::raiseEvent(std::shared_ptr<events::Event> event)
{
  _events.push(event);
}

void EventManager::update()
{
  std::queue<std::shared_ptr<events::Event> > events(_events);
  _events = std::queue<std::shared_ptr<events::Event> > ();
  while(!events.empty())
  {
    fireEvent(events.front());
    events.pop();
  }
}

void EventManager::fireEvent(std::shared_ptr<events::Event> event)
{
  std::set<SystemEventListener*> active_listeners;
  for (auto listener : _listeners)
  {
    if (listener)
    {
      if (listener->isActive())
        active_listeners.insert(listener);
    }
  }
  for (auto listener : active_listeners)
  {
    listener->queueEvent(event);
  }
}

void EventManager::registerListener(SystemEventListener* listener)
{
  _listeners.insert(listener);
}
void EventManager::unregisterListener(SystemEventListener* listener)
{
  _listeners.erase(listener);
}

} // namespace system
} // namespace ptm
