#include "../System/ManagedTimer.hpp"
#include "../System/Timer.hpp"
#include "../System/System.hpp"


namespace ptm
{
namespace system
{
TimerManager::TimerManager()
{

}

TimerManager::~TimerManager()
{
  for(auto timer : _timers)
  {
    timer->_manager=0;
  }
}

void TimerManager::update()
{
  for(auto timer : _timers)
  {
    timer->update();
  }
}

void TimerManager::update(uint64_t millis)
{
  for(auto timer : _timers)
  {
    timer->update(millis);
  }
}

void TimerManager::registerTimer(ManagedTimer* timer)
{
  if(!timer)return;
  if(timer->_manager)timer->_manager->unregisterTimer(timer);
  timer->_manager=this;
  _timers.insert(timer);
}

void TimerManager::unregisterTimer(ManagedTimer* timer)
{
  if(!timer)return;
  if(_timers.count(timer)){
    timer->_manager=0;
    _timers.erase(timer);
  }
}

std::set<ManagedTimer*> TimerManager::getTimers()
{
  return _timers;
}


ManagedTimer::ManagedTimer(TimerManager *manager, uint64_t duration)
: AutoTimer(duration), _manager(0)
{
  if(manager)
  {
    manager->registerTimer(this);
  }
}

ManagedTimer::ManagedTimer(TimerManager *manager, uint64_t duration, bool invoke_on_finish)
: AutoTimer(duration,invoke_on_finish), _manager(0)
{
  if(manager)
  {
    manager->registerTimer(this);
  }
}

ManagedTimer::ManagedTimer(TimerManager *manager, uint64_t duration, std::function<void(void)> on_finish)
: AutoTimer(duration,on_finish), _manager(0)
{
  if(manager)
  {
    manager->registerTimer(this);
  }
}

ManagedTimer::ManagedTimer(TimerManager *manager, uint64_t duration, std::function<void(void)> on_finish, bool invoke_on_finish)
: AutoTimer(duration,on_finish,invoke_on_finish), _manager(0)
{
  if(manager)
  {
    manager->registerTimer(this);
  }
}

ManagedTimer::~ManagedTimer()
{
  if(_manager)
  {
    _manager->unregisterTimer(this);
  }
}

SystemTimer::SystemTimer(uint64_t duration) :
    ManagedTimer(&(System::getInstance()->_timer_manager),duration)
{

}

SystemTimer::SystemTimer(uint64_t duration, bool invoke_on_finish) :
    ManagedTimer(&(System::getInstance()->_timer_manager),duration, invoke_on_finish)
{
}

SystemTimer::SystemTimer(uint64_t duration,
    std::function<void(void)> on_finish) :
    ManagedTimer(&(System::getInstance()->_timer_manager),duration, on_finish)
{
}

SystemTimer::SystemTimer(uint64_t duration,
    std::function<void(void)> on_finish, bool invoke_on_finish) :
    ManagedTimer(&(System::getInstance()->_timer_manager),duration, on_finish, invoke_on_finish)
{
}

SystemTimer::~SystemTimer()
{
}

} // namespace system
} // namespace ptm
