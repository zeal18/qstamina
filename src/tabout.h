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

#ifndef TABOUT_H
#define TABOUT_H

#include <QDialog>

namespace Ui {
class TAbout;
}

class TAbout : public QDialog
{
    Q_OBJECT
    
public:
    explicit TAbout(QWidget *parent = 0);
    ~TAbout();
    
private:
    Ui::TAbout *ui;
};

#endif // TABOUT_H
