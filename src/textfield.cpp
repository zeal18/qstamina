#include "textfield.h"

TextField::TextField(QWidget *parent) :
    QWidget(parent)
{
}

void TextField::keyPressed(QString key)
{
}

void TextField::setText(QString text)
{
}

QString TextField::nextSymbol()
{
}

void TextField::reset()
{
}

void TextField::start()
{
}

void TextField::stop()
{
}


int TextField::rightSymbols()
{
    return m_rightSymbols;
}

int TextField::countSymbols()
{
    return m_countSymbols;
}

int TextField::wrongSymbols()
{
    return m_wrongSymbols;
}
