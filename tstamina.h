/*
#    This file is part of QStamina
#
#    Aleksey Lezhoev <lezhoev@gmail.com>
#    Ideas and lessons: http://stamina.ru
#
#    QStamina is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#     any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TSTAMINA_H
#define TSTAMINA_H

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

#include "tresults.h"
#include "tabout.h"

namespace Ui {
class TStamina;
}

class TStamina : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit TStamina(QWidget *parent = 0);
    ~TStamina();
    
private:
    Ui::TStamina *ui;

    QString currentLayout;
    bool unionLetters;
    bool lessonStarted;
    QMenu *lessonsMenu;
    QMenu *layoutsMenu;
    QSettings *generalSettings;

    QStringList helloSounds;

    void keyPressEvent(QKeyEvent * event);
    void loadLessonsMenu();
    void loadLayoutMenu();
    void loadLesson(QString);
    void loadLayout(QString);
    void endLesson();
    void loadKeyboard(QString);
    void updateKeyboard();

    float time;
    unsigned int typeErrors;
    float typeRights;
    int typeLastSecond;
    float speed;
    QTimer *timer;
    QMenuBar *mainMenu;

    bool lessonLoaded;
    QString lessonTitle;
    QString lessonContent;

    QList<int> speedBySecond;
    QList<float> avgSpeedBySecond;
private slots:
    void lessonChoosed();
    void layoutChoosed();
    void timeout();
    void on_pushButton_released();
    void aboutTriggered();
};

#endif // TSTAMINA_H
