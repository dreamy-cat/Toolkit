#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include <QString>
#include <QVector>
#include <QMap>
#include <QThread>
#include <QDateTime>
// #include <QLinkedList>
#include <QtNetwork/QUdpSocket>

#include "UDPDevice.hpp"

// Класс UDP сервера с учетом окружения.

class UDPServer {
public:
    enum serverStatus { notExist = 0, inactive = 1, active = 2 };
    explicit UDPServer(QString name, quint16 portFirs, quint16 portLast, QString* logFormat = nullptr);
    UDPServer(const UDPServer& server) = delete;
    UDPServer(UDPServer&& server) = delete;
    virtual ~UDPServer();
    UDPServer& operator=(const UDPServer& server) = delete;
    QString* getLog() const;
    serverStatus getStatus() const;
    void start();
    void stop();
    // Подумать дополнительно о операторах.
    // Константы доступные для чтения глобально и в качестве настроек.
    static const quint16 minPortIdx, maxPortIdx, logMaxLines;
    static const QVector<QString> statusNames;
protected:
private:
    void compactLog(quint16 freeLines = logMaxLines);
    static QString debugDTFormat;
    QString name, log, *DTFormat;
    static QMap<QString, quint32> portRanges;
    // serverStatus status;
    serverStatus status;
    // QMap<quint16, QString> devices;
    QList<UDPDevice*> devices;
    // QLinkedList - проверить вариант.
    QDateTime DTStart;
};


#endif // UDP_SERVER_HPP
