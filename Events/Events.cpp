#include "../Events/Events.hpp"
#include "../System/System.hpp"
#include "../System/EventManager.hpp"

namespace ptm
{
namespace events
{

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

EventMapping::EventMapping(EventType type,
    std::function<void(std::shared_ptr<Event>)> handler) :
    type(type), handler(handler)
{

}

EventListener::EventListener() :
    _active(false)
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
  while (handleNextEvent())
  {

  }
}

bool EventListener::handleNextEvent()
{
  if (_events_queue.empty())
    return false;
  std::shared_ptr<Event> event = _events_queue.front();
  _events_queue.pop();
  if (supportsEvent(event->getType()))
  {
    EventMapping e_mapping = getEventMapping(event->getType());
    if (e_mapping.type != EventType::EVENT_NONE)
      e_mapping.handler(event);
  }
  return true;
}

void EventListener::removeEvents()
{
  _events_queue=std::queue<std::shared_ptr<Event>>();
}

bool EventListener::queueEvent(std::shared_ptr<Event> event)
{
  if (isActive() && supportsEvent(event->getType()))
  {
    EventMapping e_mapping = getEventMapping(event->getType());
    if (e_mapping.type == EventType::EVENT_NONE)
      return false;
    _events_queue.push(event);
    return true;
  }
  return false;
}
bool EventListener::supportsEvent(EventType type)
{
  return _event_mappings.find(type) != _event_mappings.end();
}
void EventListener::registerEventHandler(EventMapping mapping)
{
  _event_mappings.emplace(mapping.type, mapping);
}
void EventListener::unregisterEventHandler(EventType type)
{
  _event_mappings.erase(type);
}

EventMapping EventListener::getEventMapping(EventType type)
{
  auto mapping = _event_mappings.find(type);
  if (mapping != _event_mappings.end())
  {
    return mapping->second;
  }
  else
    return EventMapping(EventType::EVENT_NONE,
        std::bind(&EventListener::defaultHandler, this, std::placeholders::_1));
}

void EventListener::defaultHandler(std::shared_ptr<Event> event)
{

}

} // namespace events

namespace system
{

SystemEventListener::SystemEventListener() :
    events::EventListener()
{
  System::getInstance()->_event_manager.registerListener(this);
}
SystemEventListener::~SystemEventListener()
{
  System::getInstance()->_event_manager.unregisterListener(this);
}

} // namespace system
} // namespace ptm
