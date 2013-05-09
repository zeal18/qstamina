#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    m_settings = new QSettings("QStamina","QStamina");
    m_lastLayoutFile = m_settings->value("lastLayoutFile",QLocale::system().name()+".ltf").toString();
    m_fontSize = m_settings->value("fontSize",QVariant(12)).toInt();
    ui->fontSize->setValue(m_fontSize);
}

Settings::~Settings()
{
    delete ui;
}

QString Settings::lastLayoutFile()
{
    return m_lastLayoutFile;
}

void Settings::setLastLayoutFile(QString filename)
{
    m_lastLayoutFile = filename;
    m_settings->setValue("lastLayoutFile",m_lastLayoutFile);
}

int Settings::fontSize()
{
    return m_fontSize;
}

void Settings::setFontSize(int size)
{
    m_fontSize = size;
    m_settings->setValue("fontSize",m_fontSize);
}

void Settings::on_buttonBox_accepted()
{
    setFontSize(ui->fontSize->value());
    emit settingsSaved();
}

void Settings::on_buttonBox_rejected()
{
    this->hide();
}
