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

#include "inlinefield.h"

InlineField::InlineField(QWidget *parent) :
    TextField(parent)
{
    m_width = 620;
    m_height = 30;
    m_rightSymbols = 0;
    m_countSymbols = 0;
    m_wrongSymbols = 0;

    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    m_newText = new QLabel(this);
    m_oldText = new QLabel(this);
    m_layout->addWidget(m_oldText);
    m_layout->addWidget(m_newText);
    m_oldText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_newText->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_oldText->setStyleSheet("font-size: 16px; Background: #ccc");
    m_newText->setStyleSheet("font-size: 16px;");
    this->setMinimumHeight(m_height);
    this->setMinimumWidth(m_width);
    parent->setMinimumHeight(m_height);
    parent->setMinimumWidth(m_width);
    m_newText->setMinimumHeight(m_height);
    m_newText->setMinimumWidth((int) (m_width / 2));
    m_oldText->setMinimumHeight(m_height);
    m_oldText->setMinimumWidth((int) (m_width / 2));
}

void InlineField::keyPressed(QString key)
{
    qDebug()<<"InlineField::keyPressed: "<<key;
    if( key == "Backspace")
    {

    } else {
        if( key == "Space" )
            key = " ";
        if (m_newText->text().left(1) == key)
        {
            m_oldText->setText(m_oldText->text()+key);
            m_newText->setText(m_newText->text().right(m_newText->text().size()-1));
            m_rightSymbols++;
        } else {
            m_wrongSymbols++;
        }
    }

}

void InlineField::setText(QString text)
{
    //qDebug()<<"InlineField::setText: "<<text;
    m_text = text;
    reset();
}

QString InlineField::nextSymbol()
{
    //qDebug()<<"InlineField::getNextSymbol: ";
    return m_newText->text().left(1);
}

void InlineField::reset()
{
    m_oldText->setText("");
    m_newText->setText(m_text);
    m_countSymbols = m_text.size();
    m_rightSymbols = 0;
    m_wrongSymbols = 0;
}
