#ifndef __EVENTS_H__
#define __EVENTS_H__

#include <c++/4.9.3/functional>
#include <c++/4.9.3/memory>
#include <c++/4.9.3/map>
#include <c++/4.9.3/queue>

namespace ptm
{
namespace events
{

/*
 * Events types in the system.
 */
enum EventType
{
  EVENT_NONE = 0, EVENT_EXTI0_IRQn = 1, EVENT_BUTTON = 2
};

/*
 * Event class. Can be overrided if more information is required.
 */
class Event
{
  public:
    Event(EventType type);
    Event(EventType, void* source);
    ~Event();
    EventType getType() const;
    void* getSource() const;
  private:
    EventType _type;
    void* _source;
};

/*
 * Maps the event type to handling function.
 */
struct EventMapping
{
  public:
    EventMapping(EventType type,
        std::function<void(std::shared_ptr<Event>)> handler);
    EventType type;
    std::function<void(std::shared_ptr<Event>)> handler;
};

/*
 * Enables event handling and queueing.
 */
class EventListener
{
  public:
    /*
     * Creates an event listener.
     * Event listener is inactive by default.
     */
    EventListener();
    ~EventListener();

    /*
     * Returns true if the listener is active (accepts events)
     * or false when it is inactive (doesnt accept events).
     */
    bool isActive();

    /*
     * Activates the event listener.
     */
    void activate();
    /*
     * Deactivates the event listener
     */
    void deactivate();

    /*
     * Handles all events in the queue.
     */
    void handleEvents();

    /*
     * Handles next event.
     * Returns true if there was event to handle.
     * Returns false if the queue was empty.
     */
    bool handleNextEvent();

    /*
     * Removes all events from the queue.
     */
    void removeEvents();

    /*
     * If the listener is active and the event type is supported
     * then it is queued or handled depending on the immediate_handle field in mapping.
     * Returns true on success.
     */
    bool queueEvent(std::shared_ptr<Event> event);
    /*
     * Returns true if the event type is supported.
     */
    bool supportsEvent(EventType type);
    /*
     * Registers new handler mapping.
     */
    void registerEventHandler(EventMapping mapping);
    /*
     * Unregisters mapping for specified event type.
     */
    void unregisterEventHandler(EventType type);
  private:
    /*
     * If the event is supported then its mapping is returned.
     * If it is unsupported then the mapping with event type NONE and default handler is returned.
     */
    EventMapping getEventMapping(EventType type);
    /*
     * Events eater.
     */
    void defaultHandler(std::shared_ptr<Event> event);
    std::map<EventType, EventMapping> _event_mappings;
    std::queue<std::shared_ptr<Event>> _events_queue;
    bool _active;
};

} // namespace events

namespace system
{
/*
 * Enables event handling and queueing.
 * Automatically registers itself in the system.
 */
class SystemEventListener : public events::EventListener
{
  public:
    SystemEventListener();
    ~SystemEventListener();
};

} //namespace system
} //namespace ptm

#endif
