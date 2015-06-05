#ifndef __SYSTEM_MANAGED_TIMER_H__
#define __SYSTEM_MANAGED_TIMER_H__

#include <stdint.h>
#include <c++/4.9.3/set>
#include <../System/Timer.hpp>

namespace ptm
{
namespace system
{
class ManagedTimer;

class TimerManager
{
  public:
    TimerManager();
    ~TimerManager();

    /*
     * Updates all the timers.
     */
    void update();

    /*
     * Updates all the timers by given delta time.
     */
    void update(uint64_t delta);

    /*
     * Registers timer to manager.
     */
    void registerTimer(ManagedTimer* timer);
    /*
     * Unregisters timer from manager.
     */
    void unregisterTimer(ManagedTimer* timer);
    std::set<ManagedTimer*> _timers;
  private:
};

/*
 * Timer that can be managed by timer manager.
 * It registers and unregisters itself automatically.
 * Timer can have only one manager.
 * If it is registered to new manager it is unregistered from previous one.
 */
class ManagedTimer : public AutoTimer
{
  public:
    friend class TimerManager;
    ManagedTimer(TimerManager *manager, uint64_t duration);
    ManagedTimer(TimerManager *manager, uint64_t duration, bool invoke_on_finish);
    ManagedTimer(TimerManager *manager, uint64_t duration, std::function<void(void)> on_finish);
    ManagedTimer(TimerManager *manager, uint64_t duration, std::function<void(void)> on_finish, bool invoke_on_finish);
    virtual ~ManagedTimer();
  protected:
  private:
    TimerManager *_manager;
};

/*
 * Timer that automatically registers itself in the system's timer manager.
 */
class SystemTimer : public ManagedTimer
{
  public:
    SystemTimer(uint64_t duration);
    SystemTimer(uint64_t duration, bool invoke_on_finish);
    SystemTimer(uint64_t duration, std::function<void(void)> on_finish);
    SystemTimer(uint64_t duration, std::function<void(void)> on_finish, bool invoke_on_finish);
    virtual ~SystemTimer();
  protected:
  private:
};

} // namespace system
} // namespace ptm

#endif // __SYSTEM_TIMER_MANAGER_H__
