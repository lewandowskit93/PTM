#ifndef __IDEVICE_H__
#define __IDEVICE_H__

/* Device marker.
 * Classes that Derive from IDevice should declare friendship with DeviceManager.
 * Constructor of such a Device shall be protected or private so that devices can be
 * created only through manager.*/
class IDevice
{
  public:
    virtual ~IDevice();
  protected:
    IDevice();
  private:
};

#endif // __IDEVICE_H__
