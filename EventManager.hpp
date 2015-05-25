#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__
#include "Events.hpp"
#include <c++/4.9.3/set>

/*
 * Manages system events
 */
class EventManager
{
  public:
    friend class SystemEventListener;
    EventManager();
    ~EventManager();
    /*
     * Delivers event to all active listeners.
     */
    void fireEvent(std::shared_ptr<Event> event);
  private:
    /*
     * Registers listener in manager.
     */
    void registerListener(SystemEventListener* listener);
    /*
     * Unregisters listener from manager.
     */
    void unregisterListener(SystemEventListener* listener);
    std::set<SystemEventListener*> _listeners;
};

#endif
