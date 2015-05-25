#include "EventManager.hpp"
EventManager::EventManager()
{

}
EventManager::~EventManager()
{

}

void EventManager::fireEvent(std::shared_ptr<Event> event)
{
  std::set<SystemEventListener*> active_listeners;
  for(auto listener : _listeners)
  {
    if(listener)
    {
      if(listener->isActive())active_listeners.insert(listener);
    }
  }
  for(auto listener : active_listeners)
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
