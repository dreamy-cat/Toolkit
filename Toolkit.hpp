#ifndef TOOLKIT_HPP
#define TOOLKIT_HPP

#include <QMainWindow>
#include <QPlainTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class Toolkit;
}
QT_END_NAMESPACE

class Toolkit : public QMainWindow
{   // Основной класс приложения, интерфейса и общих статичных методов и данных классов.
    Q_OBJECT
public:
    Toolkit(QWidget *parent = nullptr);
    ~Toolkit();
    // Глобально доступные параметры для настроек или использования.
    QPlainTextEdit* console;
    // Глобальные константы, достпуные для чтения.
    const QString dateTimeFormat = "dd.MM.yyyy";
    static const quint16 logMaxLines;
    static void compactLog(QString* localLog, quint16 freeLines = logMaxLines);
private:
    Ui::Toolkit *interface;
    QString mainLog;
};

#endif	// TOOLKIT_HPP
