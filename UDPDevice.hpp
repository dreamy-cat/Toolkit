#ifndef UDP_DEVICE_HPP
#define UDP_DEVICE_HPP

#include <QString>

// Базовый или не исключается полностью абстрактный класс устройства UDP.

class UDPDevice {
public:
    explicit UDPDevice(QString name, quint16 port = 0); // Или автоматически.
    UDPDevice(const UDPDevice& device) = delete;
    UDPDevice(UDPDevice&& device) = delete;
    virtual ~UDPDevice();       // Подумать над чист. вирт.
    // Копирование устройства
    UDPDevice& operator=(const UDPDevice& rv);
    QString readName() const;
protected:
    QString name;
    quint16 s;
private:

};

#endif  // UDP_DEVICE_HPP
