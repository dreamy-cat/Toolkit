#ifndef UDP_DEVICE_HPP
#define UDP_DEVICE_HPP

#include <QString>

// Абстрактный класс устройства UDP.
//
//

class UDPDevice {
public:
    explicit UDPDevice(QString name, quint16 port);
    UDPDevice(const UDPDevice& device) = delete;
    UDPDevice(UDPDevice&& device) = delete;
    virtual ~UDPDevice();       // Подумать над чист. вирт.
    UDPDevice& operator=(const UDPDevice& rv);
protected:
private:
    QString name;
    quint16 s;
};

#endif  // UDP_DEVICE_HPP
