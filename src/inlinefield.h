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

#ifndef INLINEFIELD_H
#define INLINEFIELD_H

#include "textfield.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QResizeEvent>
#include <QSound>

class InlineField : public TextField
{
public:
    InlineField(QWidget *parent = 0);
    void keyPressed(QString key);
    void setText(QString text);

    QString nextSymbol();
    void reset();
    void setFontPixelSize(int);
protected:
    virtual void resizeEvent(QResizeEvent *);
private:
    QLabel *m_newText;
    QLabel *m_oldText;
    QHBoxLayout *m_layout;
    QSound *m_typeSound;
    QSound *m_errorSound;
    QSound *m_finishSound;


};

#endif // INLINEFIELD_H
