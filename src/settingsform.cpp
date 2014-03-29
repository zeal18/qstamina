#include "settingsform.h"
#include "ui_settingsform.h"

SettingsForm::SettingsForm(Config *config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);

    m_config = config;
    ui->fontSize->setValue(config->fontSize());
    ui->separateKeyboard->setChecked(config->separateKeyboard());
}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::on_buttonBox_accepted()
{
    m_config->setFontSize(ui->fontSize->value());
    m_config->setSeparateKeyboard(ui->separateKeyboard->isChecked());
    emit settingsSaved();
}

void SettingsForm::on_buttonBox_rejected()
{
    this->hide();
}
