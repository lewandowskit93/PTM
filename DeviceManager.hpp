#ifndef __DEVICE_MANAGER_H__
#define __DEVICE_MANAGER_H__

#include <c++/4.9.3/vector>
#include <c++/4.9.3/memory>
#include "IDevice.hpp"

class DeviceManager
{
  public:
    DeviceManager();
    ~DeviceManager();

    template<class DeviceT, class ... ArgsT>
    std::weak_ptr<DeviceT> mountDevice(const ArgsT& ... args);

    //template <class DeviceT,class ... ArgsT>
    //std::weak_ptr<DeviceT> mountDevice(ArgsT&& ... args);

    template<class DeviceT>
    std::weak_ptr<DeviceT> getDevice();

    template<class DeviceT>
    std::vector<std::weak_ptr<DeviceT>> getDevices();

    std::vector<std::weak_ptr<IDevice>> getAllDevices();

    void unmountDevice(std::weak_ptr<IDevice> device);
    void unmountDevices(std::vector<std::weak_ptr<IDevice>> devices);

  protected:
    std::vector<std::shared_ptr<IDevice>> _devices;
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

/*template <class DeviceT,class ... ArgsT>
 std::weak_ptr<DeviceT> DeviceManager::mountDevice(ArgsT&& ... args)
 {
 std::shared_ptr<DeviceT> device = std::shared_ptr<DeviceT>(new DeviceT(args...));
 if(device) _devices.push_back(device);
 return device;
 }*/

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

#endif
