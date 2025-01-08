#include "UDPServer.hpp"

// Реализация класса UDP сервера, подумать над потоками и распределением портов.

#include <iostream> // Remove after UI.

// Константы класса, вынесены здесь а не в файле заголовка.
const quint16 UDPServer::minPortIdx = 0xC000, UDPServer::maxPortIdx= 0xFFFF, UDPServer::readBufSize = 0x100;
const QVector<QString> UDPServer::statusNames = { "not exist", "inactive", "active" };

// Статичные данные класса, значения по умолчанию заданы, но возможо стоит подумать на флагом при вызове первого конструктора.
QMap<QString, quint32> UDPServer::portRanges = {};
QString UDPServer::debugDTFormat = "HH:mm:ss";

UDPServer::UDPServer(QString name, quint16 firstPort, quint16 lastPort, QString* logFormat) : QObject()
{   // Конструктор сервера, дополнить параметрами по мере продвижения кода.
    log.clear(); DTFormat = (logFormat == nullptr) ? &debugDTFormat : logFormat;
    status = Status::notExist;
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
    status = Status::inactive;
    DTStart = QDateTime::currentDateTime();
    log.append(DTStart.toString(*DTFormat) + ": UDP Server '" + this->name +
        "' created, clear devices, set all parameters and " + statusNames[(qsizetype)status] +
        "status.\n");
    // std::cout << log.toStdString();
    // QObject::connect(this, SIGNAL(connSign()), this, SLOT(clientConnectd()));
    // QObject::connect(this, SIGNAL(QUdpSocket::connected()), this, SLOT(clientConnected()));
}

UDPServer::~UDPServer()
{   // Деструктор сервера, просто выводим предупрждения с отвалом клиентов, как есть.
    status = Status::notExist;
    portRanges.remove(name);
    log.append(QDateTime::currentDateTime().toString(*DTFormat) +
               ": UDP Server '" + name + "' destructor, clear parameters, " +
               "release ports, and devices are still exist.\n");
    Toolkit::compactLog(&log, 1);
    std::cout << log.toStdString();
    // signal
}

UDPServer::Status UDPServer::readStatus() const
{   // Возвращает текущий статус сервера.
    return status;
}

void UDPServer::clientConnected()
{   // Тестовый слот вызываемый после оператора "connectToHost".
    log.append("Client connected to server, signal recieved.\n");
    // std::cout << "Signal!" << std::endl;
}

void UDPServer::start()
{   // Условный запуск сервера и начало обработки пакетов по правилам и с учетом соединений клиентов.
    log.append("\nServer start.\n");
    QUdpSocket* server = new QUdpSocket, *client = new QUdpSocket;
    QObject::connect(client, SIGNAL(connected()), this, SLOT(clientConnected()));
    bool r = false;
    quint16 bytesSent = 0;
    r = server->bind(QHostAddress::LocalHost, 0xC000, QAbstractSocket::DontShareAddress);
    log.append("Server bind: " + QString::number(r) +
               ", address: " + server->localAddress().toString() +
               ", port: " + QString::number(server->localPort()) +
               ", socket type: " + QString::number(server->socketType()));
    server->open(QIODevice::ReadWrite);
    log.append(", is valid: " + QString::number(server->isValid()) +
               ", is open: " + QString::number(server->isOpen()) +
               ", check state: " + QString::number(server->state()) + ".\n");
    client->connectToHost(QHostAddress::LocalHost, 0xC000, QIODevice::WriteOnly);
    // // emit connSign();
    client->setReadBufferSize(readBufSize);
    log.append("Client: " + client->peerAddress().toString() +
               ", port: " + QString::number(client->peerPort()) +
               ", name: " + client->peerName() +
               ", socket type: " + QString::number(client->socketType()) +
               ", socket state: " + QString::number(client->state()) +
               ", buffer size: " + QString::number(client->readBufferSize()) + ".\n");
    QByteArray textA("Tea!");
    QNetworkDatagram writeData;
    log.append("Text write to port: '" + textA + "' " +
               QString::number(textA.size()) + " bytes.\n");
    while (!server->hasPendingDatagrams()) {
        bytesSent = client->write(textA);   // datagram
    }
    log.append("Bytes written to port by cleint: " + QString::number(bytesSent) +
               " and data size pending on server: " +
               QString::number(server->pendingDatagramSize()) + " bytes.\n");
    /*
    QByteArray textB = server->readAll();
    log.append("Text readed from port: '" + textB + "', size " +
               QString::number(textB.size()) + " bytes.\n");
    */
    QNetworkDatagram readData = server->receiveDatagram();
    log.append("Text readed from port: '" + readData.data() +
               ", size " + QString::number(readData.data().size()) + " bytes.\n");

    log.append("Check: sender '" + readData.senderAddress().toString() +
               "', port " + QString::number(readData.senderPort()) +
               ", dest '" + readData.destinationAddress().toString() +
               ", port " + QString::number(readData.destinationPort()) +
               ", hop limit " + QString::number(readData.hopLimit()) + ".\n");

    log.append("\n");
    server->close(); client->close();
    delete(server); delete(client);
}

void UDPServer::stop()
{   // Условная остановка сервера, но объект продолжает существовать, но пакеты не

}
