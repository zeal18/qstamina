#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT
    
public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

    QString lastLayoutFile();
    void setLastLayoutFile(QString);

    int fontSize();
    void setFontSize(int);
signals:
    void settingsSaved();
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::Settings *ui;

    QSettings *m_settings;
    QString m_lastLayoutFile;
    int m_fontSize;
};

#endif // SETTINGS_H
