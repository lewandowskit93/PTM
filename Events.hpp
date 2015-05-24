#ifndef __EVENTS_H__
#define __EVENTS_H__

#include <c++/4.9.3/functional>
#include <c++/4.9.3/memory>
#include <c++/4.9.3/map>
#include <c++/4.9.3/queue>

/*
 * Events types in the system.
 */
enum EventType
{
  EVENT_NONE=0,
  EVENT_EXTI0_IRQn=1,
  EVENT_BUTTON=2
};

/*
 * Event class. Can be overrided if more information is required.
 */
class Event
{
  public:
    Event(EventType type);
    ~Event();
    EventType getType() const;
  private:
    EventType _type;
};


/*
 * Maps the event type to handling function.
 */
struct EventMapping
{
  public:
    EventMapping(EventType type,bool immediate_handle, std::function<void (std::shared_ptr<Event>)> handler );
    EventType type;
    /*
     * Specifies if event should be handled imediately (true)
     * or queued (false).
     */
    bool immediate_handle;
    std::function<void (std::shared_ptr<Event>)> handler;
};

/*
 * Enables event handling and queueing.
 */
class EventListener
{
  public:
    EventListener();
    ~EventListener();
    /*
     * Handles all events in the queue.
     */
    void handleEvents();
    /*
     * If the event type is supported then it is queued or handled
     * depending on the immediate_handle field in mapping.
     * If the event type is supported returns true, false otherwise.
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
};


#endif
