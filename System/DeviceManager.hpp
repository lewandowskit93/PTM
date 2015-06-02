#ifndef __DEVICE_MANAGER_H__
#define __DEVICE_MANAGER_H__

#include <c++/4.9.3/vector>
#include <c++/4.9.3/memory>
#include "../Devices/IDevice.hpp"

namespace ptm
{
namespace system
{

class DeviceManager
{
  public:
    DeviceManager();
    ~DeviceManager();

    /*
     * Creates and mounts a device of specified type with given arguments.
     */
    template<class DeviceT, class ... ArgsT>
    std::weak_ptr<DeviceT> mountDevice(const ArgsT& ... args);

    //template <class DeviceT,class ... ArgsT>
    //std::weak_ptr<DeviceT> mountDevice(ArgsT&& ... args);

    /*
     * Returns first device of specified type or null if no such device is mounted.
     */
    template<class DeviceT>
    std::weak_ptr<DeviceT> getDevice();

    /*
     * Returns all mounted devices of specified type.
     */
    template<class DeviceT>
    std::vector<std::weak_ptr<DeviceT>> getDevices();

    /*
     * Returns safe vector copy of all devices.
     */
    std::vector<std::weak_ptr<devices::IDevice>> getAllDevices();

    /*
     * Unmounts device from manager. The device's destructor will be called as soon
     * as all shared pointers to that device will be destroyed.
     */
    void unmountDevice(std::weak_ptr<devices::IDevice> device);

    /*
     * Unmount devices from manager.
     */
    void unmountDevices(std::vector<std::weak_ptr<devices::IDevice>> devices);

  protected:
    /*
     * Mounted devices.
     */
    std::vector<std::shared_ptr<devices::IDevice>> _devices;
  private:

};

template<class DeviceT, class ... ArgsT>
std::weak_ptr<DeviceT> DeviceManager::mountDevice(const ArgsT& ... args)
{
  std::shared_ptr<DeviceT> device = std::shared_ptr < DeviceT
      > (new DeviceT(args...));
  if (device)
    _devices.push_back(device);
  return device;
}

template<class DeviceT>
std::weak_ptr<DeviceT> DeviceManager::getDevice()
{
  for (auto device : _devices)
  {
    std::shared_ptr<DeviceT> casted_device = std::dynamic_pointer_cast < DeviceT
        > (device);
    if (casted_device)
    {
      return casted_device;
    }
  }
  return std::weak_ptr<DeviceT>();
}

template<class DeviceT>
std::vector<std::weak_ptr<DeviceT>> DeviceManager::getDevices()
{
  std::vector < std::weak_ptr<DeviceT> > devices;
  for (auto device : _devices)
  {
    std::shared_ptr<DeviceT> casted_device = std::dynamic_pointer_cast < DeviceT
        > (device);
    if (casted_device)
    {
      devices.push_back(casted_device);
    }
  }
  return devices;
}

} // namespace system
} // namespace ptm

#endif
