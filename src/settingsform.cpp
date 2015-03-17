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
    ui->enableSound->setChecked(config->enableSound());
    ui->volumeSlider->setValue( (int) ( config->volume() * 100 ) );
}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::on_buttonBox_accepted()
{
    m_config->setFontSize(ui->fontSize->value());
    m_config->setSeparateKeyboard(ui->separateKeyboard->isChecked());
    m_config->setEnableSound(ui->enableSound->isChecked());
    m_config->setVolume( (qreal) ui->volumeSlider->value() / 100.0 );
    emit settingsSaved();
}

void SettingsForm::on_buttonBox_rejected()
{
    this->hide();
}

void SettingsForm::on_enableSound_toggled(bool checked)
{
    ui->volumeSlider->setEnabled( checked );
    ui->volumeSliderLabel->setEnabled( checked );
}
