#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QSettings>

#include "config.h"

namespace Ui {
class SettingsForm;
}

class SettingsForm : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsForm(Config *config, QWidget *parent = 0);
    ~SettingsForm();
signals:
    void settingsSaved();
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_enableSound_toggled(bool checked);

private:
    Ui::SettingsForm *ui;
    Config *m_config;
};

#endif // SETTINGS_H
