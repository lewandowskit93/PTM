#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "DeviceManager.hpp"
#include "InterruptManager.hpp"

class System
{
  public:
    ~System();
    static System* getInstance();
    DeviceManager _device_manager;
    InterruptManager _interrupt_manager;
  protected:
  private:
    static System* _instance;

    System();
};

#endif
