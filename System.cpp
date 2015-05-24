#include "System.hpp"
System* System::_instance = 0;
System::System() :
    _device_manager(DeviceManager()), _interrupt_manager(InterruptManager())
{

}
System::~System()
{
}
System* System::getInstance()
{
  if (!_instance)
  {
    _instance = new System();
  }
  return _instance;
}
