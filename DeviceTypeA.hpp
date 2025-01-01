#ifndef DEVICE_TYPE_A_HPP
#define DEVICE_TYPE_A_HPP

#include "UDPDevice.hpp"

// Класс устройств типа А, подумать над производной схемой и её отображением.

class DeviceTypeA : public UDPDevice
{   //
public:
    explicit DeviceTypeA(QString name, quint16 port);
    DeviceTypeA(const DeviceTypeA& device);
    DeviceTypeA(const DeviceTypeA&& device);
    virtual ~DeviceTypeA();
    DeviceTypeA& operator=(const DeviceTypeA& device);
protected:
private:
};

#endif // DEVICE_TYPE_A_HPP
