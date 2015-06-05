#ifndef __SYSTEM_TIMER_HPP__
#define __SYSTEM_TIMER_HPP__

#include <c++/4.9.3/functional>
#include <stdint.h>

namespace ptm
{
namespace system
{

namespace TimerState
{
enum TimerState
{
  STOPPED, RUNNING, PAUSED, FINISHED
};
} // namespace TimerState

/*
 * Simple timer that can counts to the given value and can invoke function when finished counting.
 */
class Timer
{
  public:
    /*
     * Creates a timer. Timer is stopped by default.
     */
    Timer(uint64_t duration);
    Timer(uint64_t duration, bool invoke_on_finish);
    Timer(uint64_t duration, std::function<void(void)> on_finish);
    Timer(uint64_t duration, std::function<void(void)> on_finish, bool invoke_on_finish);
    virtual ~Timer();
    /*
     * Updates the timer by given milliseconds.
     * The counter is incremented by given value.
     * The timer is finished if it is ready to.
     * Returns true if the timer was updated.
     */
    virtual bool update(uint64_t delta);
    /*
     * Sets the value that the timer will count to.
     */
    void setDuration(uint64_t duration);
    /*
     * Sets timer's counter value.
     */
    void setCurrentTime(uint64_t time);
    /*
     * Starts the timer.
     * Only finished or stopped timer can be started.
     * If the timer is finished it will be stopped before starting again.
     * Returns true if timer was started.
     * Returns false if timer could not be started.
     */
    virtual bool start();
    /*
     * Stops (resets) the timer.
     * Timer counter will be set to 0.
     * Stopped timer can be started again.
     */
    void stop();
    /*
     * Pauses the timer. Timer will not be updated until resumed.
     * The counter value is saved.
     * Returns true if timer was paused.
     * Returns false if timer could not be paused.
     */
    bool pause();
    /*
     * Resumes the timer.
     * Only paused timer can be resumed.
     * Returns true on success or false on failure.
     */
    virtual bool resume();
    /*
     * Finishes the timer counting.
     * Invokes the function connected to the timer if _invoke_on_finish is set.
     * Returns true on success or false on failure.
     * Caution: checkFinish should be called to ensure that timer finished counting.
     * finish can be called on timer that havent counted to the duration yet.
     */
    bool finish();
    /*
     * Invokes the function connected to the timer.
     * Returns true if the function was invoked or false otherwise.
     */
    bool invoke();
    /*
     * Returns true if the timer is ready to finish.
     */
    bool checkFinish();
    /*
     * Gets the timer progress as value between 0 and 1.
     */
    float getProgress();
    /*
     * Returns timer duration. The timer counts to that value.
     */
    uint64_t getDuration();
    /*
     * Returns current value of timer's counter.
     */
    uint64_t getCurrentTime();
    /*
     * Returns true if timer is in finished state.
     */
    bool hasFinished();
    /*
     * Returns true if timer is stopped.
     */
    bool isStopped();
    /*
     * Returns true if timer is running.
     */
    bool isRunning();
    TimerState::TimerState getState();
  protected:
    /*
     * The function invoked by default if no other is provided.
     * Can be overrided.
     */
    virtual void onFinish();
  private:
    uint64_t _current_time;
    uint64_t _duration;
    TimerState::TimerState _state;
    bool _invoke_on_finish;
    std::function<void(void)> _on_finish;
};

/*
 * Timer that can keep track of its update times so it can compute delta on its own.
 */
class AutoTimer : public Timer
{
public:
    AutoTimer(uint64_t duration);
    AutoTimer(uint64_t duration, bool invoke_on_finish);
    AutoTimer(uint64_t duration, std::function<void(void)> on_finish);
    AutoTimer(uint64_t duration, std::function<void(void)> on_finish, bool invoke_on_finish);
    virtual ~AutoTimer();

    /*
     * Checks the current time and updates the timer by the delta of current time and last update.
     */
    bool update();

    virtual bool update(uint64_t delta);
    virtual bool start();
    virtual bool resume();
private:
    uint64_t _last_update_time;
};

} // namespace system
} // namespace ptm

#endif // __SYSTEM_TIMER_HPP__
