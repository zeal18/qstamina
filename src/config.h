#ifndef CLONFIG_H
#define CLONFIG_H

#include <QApplication>
#include <QString>
#include <QSettings>

class Config
{
public:
    Config();
    QString lastLayoutFile() const;
    void setLastLayoutFile(const QString &lastLayoutFile);

    int fontSize() const;
    void setFontSize(int fontSize);

    bool separateKeyboard() const;
    void setSeparateKeyboard(bool separateKeyboard);

private:
    QSettings *m_settings;
    QString m_lastLayoutFile;
    int m_fontSize;
    bool m_separateKeyboard;
};

#endif // CLONFIG_H
