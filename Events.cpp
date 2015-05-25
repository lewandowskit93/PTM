#include "Events.hpp"
#include "System.hpp"
#include "EventManager.hpp"

Event::Event(EventType type) :
    _type(type)
{

}
Event::~Event()
{

}
EventType Event::getType() const
{
  return _type;
}

EventMapping::EventMapping(EventType type, bool immediate_handle,
    std::function<void(std::shared_ptr<Event>)> handler) :
    type(type), immediate_handle(immediate_handle), handler(handler)
{

}


EventListener::EventListener() : _active(false)
{

}
EventListener::~EventListener()
{

}

bool EventListener::isActive()
{
  return _active;
}

void EventListener::activate()
{
  _active = true;
}

void EventListener::deactivate()
{
  _active = false;
}

void EventListener::handleEvents()
{
  std::queue<std::shared_ptr<Event>> e_queue;
  _events_queue.swap(e_queue);
  while(!e_queue.empty())
  {
    std::shared_ptr<Event> event = e_queue.front();
    e_queue.pop();
    if(supportsEvent(event->getType()))
    {
      EventMapping e_mapping = getEventMapping(event->getType());
      if(e_mapping.type==EventType::EVENT_NONE)continue;
      e_mapping.handler(event);
    }
  }
}
bool EventListener::queueEvent(std::shared_ptr<Event> event)
{
  if(isActive() && supportsEvent(event->getType()))
  {
    EventMapping e_mapping = getEventMapping(event->getType());
    if(e_mapping.type==EventType::EVENT_NONE)return false;
    if(e_mapping.immediate_handle)
    {
      e_mapping.handler(event);
    }
    else {
      _events_queue.push(event);
    }
    return true;
  }
  return false;
}
bool EventListener::supportsEvent(EventType type)
{
  return _event_mappings.find(type)!=_event_mappings.end();
}
void EventListener::registerEventHandler(EventMapping mapping)
{
  _event_mappings.emplace(mapping.type,mapping);
}
void EventListener::unregisterEventHandler(EventType type)
{
  _event_mappings.erase(type);
}

EventMapping EventListener::getEventMapping(EventType type)
{
  auto mapping = _event_mappings.find(type);
  if(mapping!=_event_mappings.end())
  {
    return mapping->second;
  }
  else return EventMapping(EventType::EVENT_NONE,true,std::bind(&EventListener::defaultHandler,this,std::placeholders::_1));
}

void EventListener::defaultHandler(std::shared_ptr<Event> event)
{

}


SystemEventListener::SystemEventListener()
: EventListener()
{
  System::getInstance()->_event_manager.registerListener(this);
}
SystemEventListener::~SystemEventListener()
{
  System::getInstance()->_event_manager.unregisterListener(this);
}
