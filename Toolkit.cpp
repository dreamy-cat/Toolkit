#include "Toolkit.hpp"
#include "./ui_Toolkit.h"

Toolkit::Toolkit(QWidget *parent) : QMainWindow(parent), ui(new Ui::Toolkit)
{
    ui->setupUi(this);
}

Toolkit::~Toolkit()
{
    delete ui;
}
