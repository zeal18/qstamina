#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>
#include <QLabel>
#include <QFrame>
#include <QDebug>
#include <QRegExp>
#include <QColor>

namespace Ui {
class Keyboard;
}

class Keyboard : public QWidget
{
    Q_OBJECT
    
public:
    explicit Keyboard(QWidget *parent = 0);
    ~Keyboard();

    void loadKeyboard(QString);
    void updateKeyboard(QString);
    
private:
    Ui::Keyboard *ui;

    bool m_unionLetters;
    QLabel *m_lastSelectedSymbol;
};

#endif // KEYBOARD_H
