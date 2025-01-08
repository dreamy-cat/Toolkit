#include "Toolkit.hpp"
#include "./ui_Toolkit.h"

// Статичные константы, общие для всего приложения.
const quint16 Toolkit::logMaxLines = 0x100;

Toolkit::Toolkit(QWidget *parent) : QMainWindow(parent)
{   // Конструктор основного интрфейса.
    interface = new Ui::Toolkit;
    interface->Ui::Toolkit::setupUi(this);
}

Toolkit::~Toolkit()
{   // Деструктор основного интерфейса.
    delete interface;
}

void Toolkit::compactLog(QString* localLog, quint16 freeLines)
{   // Удаление более "давних" сообщения в журнале и освобождение минмиума свободных строк.
    quint16 logLines = localLog->count("\n");
    while ((logMaxLines - logLines) < freeLines) {
        localLog->remove(0, localLog->indexOf('\n', 0, Qt::CaseInsensitive) + 1);
        --logLines;
    }
}
