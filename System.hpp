#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "DeviceManager.hpp"
#include "InterruptManager.hpp"
#include "EventManager.hpp"
#include "Application.hpp"
#include <c++/4.9.3/stack>
#include <c++/4.9.3/memory>

namespace ptm
{
namespace system
{

/*
 * System singleton that gives access to all system components.
 */
class System
{
  public:
    ~System();
    static System* getInstance();
    DeviceManager _device_manager;
    InterruptManager _interrupt_manager;
    EventManager _event_manager;
    /*
     * Starts new application as the current one.
     * It pushes current one onto the stack.
     */
    template<class ApplicationT, class ... ArgsT>
    void runApplication(const ArgsT& ... args);
    /*
     * Runs the system loop.
     */
    void run();
    /*
     * Returns system time counted from the system start in milliseconds.
     */
    uint64_t getTime();
    /*
     * Sets the system time in milliseconds.
     */
    void setTime(uint64_t time);
    /*
     * Adds delta milliseconds to current system time.
     */
    void addTime(uint64_t delta);
    /*
     * Increments current system time.
     */
    void incTime();
    /*
     * Sleeps for millis milliseconds.
     * Beware! Invoking this from any interrupt my cause a freeze,
     * because system time will not be updated.
     */
    void sleep(uint64_t millis);
  protected:
  private:
    volatile uint64_t _time;
    static System* _instance;
    std::shared_ptr<Application> _current_app;
    std::stack<std::shared_ptr<Application> > _applications_stack;
    bool _running;
    System();
    /*
     * Sets the stack-top application as the current one.
     * If there is no application on stack then system freezes.
     */
    void nextAppFromStack();
};

template<class ApplicationT, class ... ArgsT>
void System::runApplication(const ArgsT& ... args)
{
  if (_current_app)
  {
    _current_app->pause();
    _applications_stack.push(_current_app);
  }
  _current_app = std::shared_ptr < ApplicationT > (new ApplicationT(args...));
}

} //namespace system
} //namespace ptm

extern "C"
{
void SysTick_Handler(void);
}

#endif
