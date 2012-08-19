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
    void keyReleaseEvent(QKeyEvent * event);
    void buildMainMenu();
    void loadLesson(QString lesson);
    void endLesson();
    void updateKeyboard();

    float time;
    unsigned int typeErrors;
    float typeRights;
    float speed;
    QTimer *timer;
    QMenuBar *mainMenu;
private slots:
    void lessonChoosed();
    void timeout();
};

#endif // TSTAMINA_H
