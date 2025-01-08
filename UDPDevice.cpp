#include "UDPDevice.hpp"

// Реализация класса устройства дополнено на вещании 2.

UDPDevice::UDPDevice(QString name, quint16 port)
{   // Конструктор базового устройства.

}

UDPDevice::~UDPDevice()
{   // Деструктор базового класса.

}

UDPDevice& UDPDevice::operator=(const UDPDevice& rv)
{   // Копирование устройства оператором.

}

QString UDPDevice::readName() const
{   // Чтение имени устройства.
    return name;
}
