#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Events.hpp"

class Application;

/*
 * Application Context is an Application State.
 * Every Application has to have at least one context to run.
 * An application can have multiple contexts which can be used for e.g. to implement different menus.
 * Every context can implement different actions than any other for the same event.
 * Every system tick an update function will be invoked on currently running application.
 */
class ApplicationContext : public EventListener
{
  public:
    ApplicationContext(Application *application);
    virtual ~ApplicationContext();
    /*
     * Handles the events and updates the context.
     */
    void update();
  protected:
    /*
     * Should be overrided for updating the context (context logic).
     */
    virtual void onUpdate()=0;
    Application *_application;

};

/*
 * The smallest runnable class.
 * Application has to have context in order to run.
 * It can be used to implement global meanings for events for whole application.
 * If the current context doesnt support an event, it will be delivered to application (if the application supports it).
 */
class Application : public EventListener
{
  public:
    friend class System;
    Application();
    virtual ~Application();
    /*
     * Handles the events.
     * Updates the application and its current context.
     * If there is no context then the application finishes itself.
     */
    void update();
    /*
     * Finishes the application.
     * The top application on the system stack becomes the current one.
    */
    void finish();
    /*
     * Returns true if application finished its work.
     */
    bool hasFinished();
    /*
     * Switches application context.
     */
    void switchContext(ApplicationContext* _new_context);
  protected:
    /*
     * Should be overrided to provide logic for whole application.
     */
    virtual void onUpdate()=0;
  private:
    ApplicationContext* _current_context;
    bool _finished;
};

#endif
