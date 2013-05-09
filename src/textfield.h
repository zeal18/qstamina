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

#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include <QWidget>
#include <QDebug>

class TextField : public QWidget
{
    Q_OBJECT
public:
    explicit TextField(QWidget *parent = 0);
    virtual void keyPressed(QString key);
    virtual void setText(QString text);
    virtual QString nextSymbol();
    virtual void reset();
    virtual void start();
    virtual void stop();
    int rightSymbols();
    int countSymbols();
    int wrongSymbols();

    virtual void setFontPixelSize(int);
protected:
    QString m_text;
    int m_typeRights;
    int m_width;
    int m_height;
    int m_rightSymbols;
    int m_countSymbols;
    int m_wrongSymbols;

    int m_fontPixelSize;

    virtual void resizeEvent(QResizeEvent *);
signals:
    void noMoreText();
public slots:
    
};

#endif // TEXTFIELD_H
