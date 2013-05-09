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
    m_separateKeyboard = m_settings->value("separateKeyboard",QVariant(false)).toBool();
    ui->fontSize->setValue(m_fontSize);
    ui->separateKeyboard->setChecked(m_separateKeyboard);
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

bool Settings::separateKeyboard()
{
    return m_separateKeyboard;
}

void Settings::setSeparateKeyboard(bool sk)
{
    m_separateKeyboard = sk;
    m_settings->setValue("separateKeyboard",sk);
}

void Settings::on_buttonBox_accepted()
{
    setFontSize(ui->fontSize->value());
    setSeparateKeyboard(ui->separateKeyboard->isChecked());
    emit settingsSaved();
}

void Settings::on_buttonBox_rejected()
{
    this->hide();
}
