#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__
#include "../Events/Events.hpp"
#include <c++/4.9.3/set>
#include <c++/4.9.3/queue>

namespace ptm
{
namespace system
{

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
     * Pushes event to the queue.
     */
    void raiseEvent(std::shared_ptr<events::Event> event);
    /*
     * Delivers events to listeners
     */
    void update();
  private:
    /*
     * Delivers event to all active listeners.
    */
    void fireEvent(std::shared_ptr<events::Event> event);
    /*
     * Registers listener in manager.
     */
    void registerListener(SystemEventListener* listener);
    /*
     * Unregisters listener from manager.
     */
    void unregisterListener(SystemEventListener* listener);
    std::set<SystemEventListener*> _listeners;
    std::queue<std::shared_ptr<events::Event>> _events;
};

} //namespace system
} //namespace ptm

#endif
