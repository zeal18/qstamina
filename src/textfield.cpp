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

#include "textfield.h"

TextField::TextField(QWidget *parent) :
    QWidget(parent)
{
}

void TextField::keyPressed(QString)
{
}

void TextField::setText(QString)
{
}

QString TextField::nextSymbol()
{
    return QString("");
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

void TextField::setFontPixelSize(int size)
{
    m_fontPixelSize = size;
}

void TextField::setEnableSound(bool enabled)
{
    //m_enableSound = enabled;
}

void TextField::resizeEvent(QResizeEvent *)
{
}
