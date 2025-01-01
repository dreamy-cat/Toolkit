#include "Toolkit.hpp"
#include "./ui_Toolkit.h"

Toolkit::Toolkit(QWidget *parent) : QMainWindow(parent)
{   // Конструктор основного интрфейса.
    interface = new Ui::Toolkit;
    interface->Ui::Toolkit::setupUi(this);
}

Toolkit::~Toolkit()
{   // Деструктор основного интерфейса.
    delete interface;
}
