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

#ifndef RESULTS_H
#define RESULTS_H

#include <QDialog>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsPathItem>
#include <QGraphicsScene>

#include <qmath.h>

namespace Ui {
class Results;
}

class Results : public QDialog
{
    Q_OBJECT
    
public:
    explicit Results(QWidget *parent = 0);
    ~Results();

    void setErrors(QString);
    void setRights(QString);
    void setTime(QString);
    void setSpeed(QString);
    void drawGraph(QList<int>,QList<float>);
    
private:
    Ui::Results *ui;
};

#endif // RESULTS_H
