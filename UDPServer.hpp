#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include <QString>
#include <QVector>
#include <QMap>
#include <QThread>
#include <QDateTime>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QNetworkDatagram>

#include "Toolkit.hpp"
#include "UDPDevice.hpp"

// Класс UDP сервера с учетом окружения.

// План вещания 2:
// - правки с предыдущего вещания;
// - немного изменить методы и переместить их по классам;
// - слоты/сигналы для сети.
// - установить UDP соединение и протестировать передачу данных;

// План вещания 3:
// - назначение портов для клиента с учетом серверных или полностью динамика;
// - подпробный вывод информации и раскидать код по методоам.
// - реализовать базовое устроство и хотя бы одно прозводное;
// - минимальный интерфейс и потоки.

class UDPServer : public QObject {   // final
    Q_OBJECT
public:
    // Спасибо за правку ShylySmiling на вещании. ;)
    enum class Status : quint16 { notExist = 0, inactive = 1, active = 2 }; 	
    explicit UDPServer(QString name, quint16 portFirs, quint16 portLast, QString* logFormat = nullptr);
    UDPServer(const UDPServer& server) = delete;
    UDPServer(UDPServer&& server) = delete;
    ~UDPServer();   // Виртуальный если потребуется.
    UDPServer& operator=(const UDPServer& server) = delete;
    QString* readLog() const;
    Status readStatus() const;
    void start();
    void stop();
    // Подумать дополнительно операторы.
    // Константы доступные для чтения глобально и в качестве настроек.
    static const quint16 minPortIdx, maxPortIdx, readBufSize;
    static const QVector<QString> statusNames;
private slots:
    void clientConnected();
signals:
    void connSign();
protected:
private:
    // Закрытые методы класса, дополнительно.
    // Статичные данные класса, для всех объектов.
    static QString debugDTFormat;
    static QMap<QString, quint32> portRanges;
    // Данные объектов класса.
    Status status;
    QString name, log, *DTFormat;
    QList<UDPDevice*> devices;
    QDateTime DTStart;
};

#endif // UDP_SERVER_HPP
