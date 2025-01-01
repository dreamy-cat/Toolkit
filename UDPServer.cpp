#include "UDPServer.hpp"

// Реализация класса UDP сервера.

#include <iostream>

// Константы класса.
const quint16 UDPServer::minPortIdx = 0xC000, UDPServer::maxPortIdx= 0xFFFF, UDPServer::logMaxLines = 0x03;
const QVector<QString> UDPServer::statusNames = { "not exist", "inactive", "active" };

// Статичные данные класса.
QMap<QString, quint32> UDPServer::portRanges = {};
QString UDPServer::debugDTFormat = "HH:mm:ss";


UDPServer::UDPServer(QString name, quint16 firstPort, quint16 lastPort, QString* logFormat)
{   // Конструктор сервера, дополнить парамтрами по продвижению.
    // Потоки! :)
    log.clear(); DTFormat = (logFormat == nullptr) ? &debugDTFormat : logFormat;
    status = serverStatus::notExist;
    log.append(QDateTime::currentDateTime().toString(*DTFormat) + ": " +
               "UDP server '" + name + "' constructor, trying allocate ports (" +
               QString::number(firstPort) + ":" + QString::number(lastPort) + "), ");

    if (lastPort < firstPort || name.isEmpty() || portRanges.contains(name) ||
        DTFormat->isEmpty()) {
        log.append("name, ports or format are incorrect or server already exist.\n");
        return;
    }
    if (!portRanges.isEmpty()) {
        for (auto& ports : portRanges) {
            quint16 first = ports & 0xFFFF, last = ports >> 0x10;
            log.append("(" + QString::number(first) + ":" + QString::number(last) + ") ");
            if ((firstPort >= first && firstPort <= last)
                || (lastPort >= first && lastPort <= last)) {
                log.append("ports are already in use by other server can't start new server.\n");
                return;
            }
        }
    } else {
        portRanges.clear();
        log.append("no other objects ");
    }
    log.append("- server port range avalable and allocated.\n");
    this->name = name;
    devices.clear();
    quint32 ports = (lastPort << 0x10) | (firstPort & 0xFFFF);
    portRanges.insert(name, ports);
    status = serverStatus::inactive;
    DTStart = QDateTime::currentDateTime();
    log.append(DTStart.toString(*DTFormat)) + ": UDP Server '" + this->name +
        "' created, clear devices, set all parameters and " + statusNames[status] +
        "status.\n";
    // std::cout << log.toStdString();
}

UDPServer::~UDPServer()
{   // Деструктор сервера, просто выводим предупрждения с отвалом клиентов.
    status = serverStatus::notExist;
    portRanges.remove(name);
    log.append(QDateTime::currentDateTime().toString(*DTFormat) +
               ": UDP Server '" + name + "' destructor, clear parameters, " +
               "release ports, and devices are still exist.\n");
    compactLog(1);
    std::cout << log.toStdString();
    // signal
}

void UDPServer::compactLog(quint16 freeLines)
{
    quint16 logLines = log.count("\n");
    while ((logMaxLines - logLines) < freeLines) {
        log.remove(0, log.indexOf('\n', 0, Qt::CaseInsensitive) + 1);
        --logLines;
    }
}

UDPServer::serverStatus UDPServer::getStatus() const
{   //
    return status;
}

void UDPServer::start()
{   //

}

void UDPServer::stop()
{   //

}











