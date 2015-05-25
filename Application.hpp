#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Events.hpp"

class Application;

enum ApplicationLifeState
{
  UNINITIALIZED, INITIALIZED, RUNNABLE, RUNNING, PAUSED, STOPPED, TERMINATED
};

/*
 * Application Context is an Application State.
 * It can be used to implement behaviour variety within single application.
 * An application doesnt have to have context.
 * An application can have multiple contexts.
 * At least one context can be active within application at any given moment.
 * Every system tick an update function will be invoked on current application context.
 */
class ApplicationContext
{
  public:
    ApplicationContext();
    virtual ~ApplicationContext();
    /*
     * Starts the context.
     * Context have to be started before it is run.
     */
    void start();

    /*
     * Handles the events and updates the context.
     */
    void update();

    /*
     * Pauses the context.
     * While the context is paused it is not updated and its events are not handled.
     */
    void pause();

    /*
     * Resumes the paused context.
     */
    void resume();

    /*
     * Stops the context.
     */
    void stop();

    ApplicationLifeState getApplicationState();
  protected:
    virtual void onStart()=0;
    /*
     * Should be overrided for updating the context (context logic).
     */
    virtual void onUpdate()=0;
    virtual void onPause()=0;
    virtual void onResume()=0;
    virtual void onStop()=0;
    ApplicationLifeState _app_state;
    SystemEventListener _event_listener;

};

/*
 * The smallest runnable class.
 * It can be used to implement global meanings for whole application.
 * Every system tick the application is updated.
 */
class Application
{
  public:
    friend class System;
    Application();
    virtual ~Application();

    /*
     * Starts the application.
     * Application have to be started before it is run.
     */
    void start();

    /*
     * Handles the events.
     * Updates the application and its current context if any exists.
     */
    void update();

    /*
     * Pauses the application and its current context.
     */
    void pause();

    /*
     * Resumes the paused application and its current context.
     * While the application is paused it is not updated and its events are not handled.
     */
    void resume();

    /*
     * Stops the application and its current context.
     */
    void stop();

    /*
     * Terminates the application.
     */
    void terminate();

    /*
     * Switches application context.
     * The old context is paused, and the new one is
     * resumed or started depending on its state.
     */
    void switchContext(ApplicationContext* _new_context);

    ApplicationLifeState getApplicationState();
  protected:
    virtual void onStart()=0;
    /*
     * Should be overrided to provide logic for whole application.
     */
    virtual void onUpdate()=0;
    virtual void onPause()=0;
    virtual void onResume()=0;
    virtual void onStop()=0;
    SystemEventListener _event_listener;
  private:
    ApplicationContext* _current_context;
    ApplicationLifeState _app_state;
};

#endif
