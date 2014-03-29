#include "config.h"

Config::Config()
{
    m_settings = new QSettings(qApp->applicationName(),qApp->organizationName());
    m_lastLayoutFile = m_settings->value("lastLayoutFile",QLocale::system().name()+".ltf").toString();
    m_fontSize = m_settings->value("fontSize",QVariant(12)).toInt();
    m_separateKeyboard = m_settings->value("separateKeyboard",QVariant(false)).toBool();
}
QString Config::lastLayoutFile() const
{
    return m_lastLayoutFile;
}

void Config::setLastLayoutFile(const QString &lastLayoutFile)
{
    m_lastLayoutFile = lastLayoutFile;
    m_settings->setValue("lastLayoutFile",lastLayoutFile);
}
int Config::fontSize() const
{
    return m_fontSize;
}

void Config::setFontSize(int fontSize)
{
    m_fontSize = fontSize;
    m_settings->setValue("fontSize",fontSize);
}
bool Config::separateKeyboard() const
{
    return m_separateKeyboard;
}

void Config::setSeparateKeyboard(bool separateKeyboard)
{
    m_separateKeyboard = separateKeyboard;
    m_settings->setValue("separateKeyboard",separateKeyboard);
}



