#include "tabout.h"
#include "ui_tabout.h"

TAbout::TAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TAbout)
{
    ui->setupUi(this);
}

TAbout::~TAbout()
{
    delete ui;
}
