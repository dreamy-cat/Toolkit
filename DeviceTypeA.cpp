#include "DeviceTypeA.hpp"

// Реализация класса устройств типа А.

DeviceTypeA::DeviceTypeA(QString name, quint16 port) : UDPDevice(name, port)
{   // Конструктор.

}

DeviceTypeA::DeviceTypeA(const DeviceTypeA& device) : UDPDevice(device.readName())
{   // Копирующий конструктор.

}

DeviceTypeA::DeviceTypeA(DeviceTypeA&& device) : UDPDevice(device.readName())
{   // Перемещающий конструктор.

}

DeviceTypeA::~DeviceTypeA()
{   // Деструктор.

}

DeviceTypeA& DeviceTypeA::operator=(const DeviceTypeA& device)
{   // Оператор присвоения.

}
