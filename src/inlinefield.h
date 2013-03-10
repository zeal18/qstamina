#ifndef INLINEFIELD_H
#define INLINEFIELD_H

#include "textfield.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>

class InlineField : public TextField
{
public:
    InlineField(QWidget *parent = 0);
    void keyPressed(QString key);
    void setText(QString text);
    QString nextSymbol();
    void reset();
private:
    QLabel *m_newText;
    QLabel *m_oldText;
    QHBoxLayout *m_layout;

};

#endif // INLINEFIELD_H
