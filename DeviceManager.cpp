#include "DeviceManager.hpp"

namespace ptm
{
namespace system
{

DeviceManager::DeviceManager()
{
}
DeviceManager::~DeviceManager()
{

}

std::vector<std::weak_ptr<devices::IDevice>> DeviceManager::getAllDevices()
{
  std::vector < std::weak_ptr<devices::IDevice> > devices_copy;
  for (auto device : _devices)
  {
    devices_copy.push_back(device);
  }
  return devices_copy;
}

void DeviceManager::unmountDevice(std::weak_ptr<devices::IDevice> device)
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
void DeviceManager::unmountDevices(
    std::vector<std::weak_ptr<devices::IDevice>> devices)
{
  for (auto device : devices)
  {
    unmountDevice(device);
  }
}

} // namespace system
} // namespace ptm
