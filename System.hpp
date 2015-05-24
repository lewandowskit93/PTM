#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "DeviceManager.hpp"
#include "InterruptManager.hpp"
#include <c++/4.9.3/stack>
#include <c++/4.9.3/memory>

class Application;
class Event;

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
    /*
     * Starts new application as the current one.
     * It pushes current one onto the stack.
     */
    template<class ApplicationT, class ... ArgsT>
    void startApplication(const ArgsT& ... args);
    /*
     * Delivers event to current application's context or current application if any supports it.
     * Does nothing otherwise.
     */
    void fireEvent(std::shared_ptr<Event> event);
    /*
     * Runs the system loop.
     */
    void run();
    /*
     * Sets the stack-top application as the current one.
     * If there is no application on stack then system freezes.
     */
    void nextAppFromStack();
  protected:
  private:
    static System* _instance;
    std::shared_ptr<Application> _current_app;
    std::stack<std::shared_ptr<Application> > _applications_stack;
    bool _running;
    System();
};

template<class ApplicationT, class ... ArgsT>
void System::startApplication(const ArgsT& ... args)
{
    if(_current_app)
    {
      _applications_stack.push(_current_app);
    }
    _current_app=std::shared_ptr<ApplicationT>(new ApplicationT(args...));
}

#endif
