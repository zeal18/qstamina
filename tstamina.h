#ifndef TSTAMINA_H
#define TSTAMINA_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDir>
#include <QSettings>
#include <QTimer>
#include <QTime>
#include <QLabel>
#include <QList>
#include <QRegExp>

#include <QDebug>
#include <QKeyEvent>

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
    QMenu *lessonsMenu;
    QMenu *layoutsMenu;
    QSettings *generalSettings;

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
    float speed;
    QTimer *timer;
    QMenuBar *mainMenu;
private slots:
    void lessonChoosed();
    void layoutChoosed();
    void timeout();
};

#endif // TSTAMINA_H
