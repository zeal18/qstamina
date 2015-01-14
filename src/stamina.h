/*
*  This file is part of QStamina
*
*  Copyright (C) 2012 Aleksey Lezhoev <lezhoev@gmail.com>
*
*  QStamina is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 2 or (at your option)
*  version 3 of the License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STAMINA_H
#define STAMINA_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QTimer>
#include <QTime>
#include <QLabel>
#include <QList>
#include <QRegExp>

#include <QDebug>
#include <QKeyEvent>

#include <QMessageBox>

#include <QStandardPaths>

#include "config.h"
#include "results.h"
#include "about.h"
#include "textfield.h"
#include "settingsform.h"
#include "keyboard.h"
#include "lessongenerator.h"
#include "inlinefield.h"
#include "sounds.h"

namespace Ui {
class Stamina;
}

class Stamina : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Stamina(QWidget *parent = 0);
    ~Stamina();
    
private:
    Ui::Stamina *ui;

    Config *m_config;
    Sounds *m_sounds;

    bool m_lessonStarted;
    QMenu *m_lessonsMenu;
    QMenu *m_layoutsMenu;
    QMenu *m_generatorMenu;
    SettingsForm *m_settings;

    Keyboard *m_keyboard;

    TextField *m_textfield;

    void keyPressEvent(QKeyEvent * event);
    void loadLessonsMenu();
    void loadLayoutMenu();
    void loadGeneratorMenu();
    void loadGeneratedLessons();
    void loadLesson(Lesson *);
    void loadCurrentLayout();
    void endLesson();

    float m_time;
    int m_typeLastSecond;
    float m_speed;
    QTimer *m_timer;
    QMenuBar *m_mainMenu;

    bool m_lessonLoaded;

    QList<int> m_speedBySecond;
    QList<float> m_avgSpeedBySecond;
private slots:
    void lessonChoosed();
    void generatedlessonChoosed();
    void layoutChoosed();
    void timeout();
    void on_pushButton_released();
    void aboutTriggered();
    void settingsTriggered();
    void settingsSaved();
    void generatorTriggered();
};

#endif // STAMINA_H
