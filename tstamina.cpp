#include "tstamina.h"
#include "ui_tstamina.h"

TStamina::TStamina(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TStamina)
{
    ui->setupUi(this);
}

TStamina::~TStamina()
{
    delete ui;
}
