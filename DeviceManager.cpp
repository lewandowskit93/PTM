#include "DeviceManager.hpp"

DeviceManager::DeviceManager()
{
}
DeviceManager::~DeviceManager()
{

}

std::vector<std::weak_ptr<IDevice>> DeviceManager::getAllDevices()
{
  std::vector < std::weak_ptr<IDevice> > devices_copy;
  for (auto device : _devices)
  {
    devices_copy.push_back(device);
  }
  return devices_copy;
}

void DeviceManager::unmountDevice(std::weak_ptr<IDevice> device)
{
  if (auto s_device = device.lock())
  {
    for (auto d_iter = _devices.begin(); d_iter != _devices.end(); ++d_iter)
    {
      if ((*d_iter).get() == s_device.get())
      {
        _devices.erase(d_iter);
        break;
      }
    }
  }
}
void DeviceManager::unmountDevices(std::vector<std::weak_ptr<IDevice>> devices)
{
  for (auto device : devices)
  {
    unmountDevice(device);
  }
}
