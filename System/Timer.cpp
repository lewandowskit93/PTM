#include "../System/Timer.hpp"
#include "../System/ManagedTimer.hpp"
#include "../System/System.hpp"
#include "math.h"

namespace ptm
{
namespace system
{

Timer::Timer(uint64_t duration) :
    _current_time(0), _duration(duration), _state(TimerState::STOPPED), _invoke_on_finish(
        true), _on_finish(std::bind(&Timer::onFinish, this))
{

}

Timer::Timer(uint64_t duration, bool invoke_on_finish) :
    _current_time(0), _duration(duration), _state(TimerState::STOPPED), _invoke_on_finish(
        invoke_on_finish), _on_finish(std::bind(&Timer::onFinish, this))
{

}

Timer::Timer(uint64_t duration, std::function<void(void)> on_finish) :
    _current_time(0), _duration(duration), _state(TimerState::STOPPED), _invoke_on_finish(
        true), _on_finish(on_finish)
{

}

Timer::Timer(uint64_t duration, std::function<void(void)> on_finish,
    bool invoke_on_finish) :
    _current_time(0), _duration(duration), _state(TimerState::STOPPED), _invoke_on_finish(
        invoke_on_finish), _on_finish(on_finish)
{

}

Timer::~Timer()
{

}

bool Timer::update(uint64_t delta)
{
  if (_state == TimerState::RUNNING)
  {
    _current_time += delta;
    if (checkFinish())
    {
      finish();
    }
    return true;
  }
  else
    return false;
}

void Timer::setDuration(uint64_t duration)
{
  _duration = duration;
}
void Timer::setCurrentTime(uint64_t time)
{
  _current_time = time;
}
bool Timer::start()
{
  switch (_state)
  {
    case TimerState::FINISHED:
    {
      stop();
    }
    case TimerState::STOPPED:
    {
      _state = TimerState::RUNNING;
      return true;
    }
    case TimerState::PAUSED:
    case TimerState::RUNNING:
    default:
    {
      return false;
    }
  }
}

bool Timer::resume()
{
  if (_state == TimerState::PAUSED)
  {
    _state = TimerState::RUNNING;
    return true;
  }
  return false;
}

void Timer::stop()
{
  _state = TimerState::STOPPED;
  _current_time = 0;
}
bool Timer::pause()
{
  if (_state == TimerState::RUNNING)
  {
    _state = TimerState::PAUSED;
    return true;
  }
  return false;
}

bool Timer::finish()
{
  if (_state == TimerState::RUNNING)
  {
    _state = TimerState::FINISHED;
    if(_invoke_on_finish)invoke();
    return true;
  }
  return false;
}

bool Timer::invoke()
{
  if (_on_finish)
  {
    _on_finish();
    return true;
  }
  return false;
}

bool Timer::checkFinish()
{
  if (_state == TimerState::RUNNING && _current_time >= _duration)
  {
    return true;
  }
  else
    return false;
}

float Timer::getProgress()
{
  float progress = (float) (_current_time) / (float) (_duration);
  if (progress > 1.0f)
    return 1.0f;
  else if (progress < 0.0f)
    return 0.0f;
  else
    return progress;
}

uint64_t Timer::getDuration()
{
  return _duration;
}
uint64_t Timer::getCurrentTime()
{
  return _current_time;
}
bool Timer::hasFinished()
{
  return _state == TimerState::FINISHED;
}
bool Timer::isStopped()
{
  return _state == TimerState::STOPPED;
}
bool Timer::isRunning()
{
  return _state == TimerState::RUNNING;
}
TimerState::TimerState Timer::getState()
{
  return _state;
}
void Timer::onFinish()
{

}

AutoTimer::AutoTimer(uint64_t duration)
: Timer(duration), _last_update_time(0)
{

}

AutoTimer::AutoTimer(uint64_t duration, bool invoke_on_finish)
: Timer(duration,invoke_on_finish), _last_update_time(0)
{

}

AutoTimer::AutoTimer(uint64_t duration, std::function<void(void)> on_finish)
: Timer(duration,on_finish), _last_update_time(0)
{

}

AutoTimer::AutoTimer(uint64_t duration, std::function<void(void)> on_finish, bool invoke_on_finish)
: Timer(duration,on_finish,invoke_on_finish), _last_update_time(0)
{

}

AutoTimer::~AutoTimer()
{

}

bool AutoTimer::update()
{
  uint64_t update_time = System::getInstance()->getTime();
  return update(update_time - _last_update_time);
}

bool AutoTimer::update(uint64_t delta)
{
  uint64_t update_time = System::getInstance()->getTime();
  if(Timer::update(delta))
  {
    _last_update_time=update_time;
    return true;
  }
  else return false;
}
bool AutoTimer::start()
{
  if(Timer::start())
  {
    _last_update_time=System::getInstance()->getTime();
    return true;
  }
  return false;
}
bool AutoTimer::resume()
{
  if(Timer::resume())
  {
    _last_update_time=System::getInstance()->getTime();
    return true;
  }
  return false;
}

} // namespace system
} // namespace ptm
