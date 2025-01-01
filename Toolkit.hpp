#ifndef TOOLKIT_HPP
#define TOOLKIT_HPP

#include <QMainWindow>

QT_BEGIN_NAMESPACE

namespace Ui {
class Toolkit;
}

QT_END_NAMESPACE

class Toolkit : public QMainWindow
{   // Основной класс окна и интерфейса.
    Q_OBJECT
public:
    Toolkit(QWidget *parent = nullptr);
    ~Toolkit();
protected:
private:
    Ui::Toolkit *interface;
};

#endif	// TOOLKIT_HPP
