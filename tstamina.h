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
