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

#include "results.h"
#include "ui_results.h"

Results::Results(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Results)
{
    ui->setupUi(this);
    ui->graph->setGeometry(0,0,450,200);
}

Results::~Results()
{
    delete ui;
}
void Results::setErrors(QString errors)
{
    ui->lblErrors->setText(errors);
}

void Results::setRights(QString rights)
{
    ui->lblAmount->setText(rights);
}

void Results::setTime(QString time)
{
    ui->lblTime->setText(time);
}

void Results::setSpeed(QString speed)
{
    ui->lblSpeed->setText(speed);
}

void Results::drawGraph(QList<int> speed, QList<float> avgSpeed)
{
    int maxSpeed = 0;
    int i = 0;
    for( i = 0; i < speed.count(); i++ )
    {
        if( speed.at(i) > maxSpeed )
            maxSpeed = speed.at(i);
    }
    if( speed.count() != 0 && maxSpeed != 0 )
    {
        int leftMargin = 30;
        int graphWidth = ui->graph->width() - leftMargin;
        int graphHeight = ui->graph->height() - 30;
        qreal scaleY = (qreal)graphHeight / (qreal)(maxSpeed + 1);
        qDebug()<<"Scale Y: "<<scaleY;
        qreal scaleX = (qreal)graphWidth / (qreal)speed.count();
        qDebug()<<"Scale X: "<<scaleX<<"/"<<speed.count();

        int gridXStep = qCeil((qreal)(20/scaleX));
        qDebug()<<"Grid x step"<<gridXStep;

        QGraphicsScene *graphicsScene = new QGraphicsScene;

        QGraphicsPathItem *graphicsPath;
        QPainterPath painterPath;
        QPainterPath avgSpeedPath;
        QPainterPath gridXPath;
        QPainterPath gridXText;
        QPainterPath gridYPath;
        QPainterPath gridYText;
        painterPath.moveTo(leftMargin,graphHeight);
        avgSpeedPath.moveTo(leftMargin,graphHeight);

        qreal x = 0;
        for( i = (maxSpeed + 1); i >= 0; i-- )
        {
            gridYPath.moveTo(0,i * scaleY);
            gridYPath.lineTo(ui->graph->width(),i * scaleY);
            QString yText;
            yText.setNum((maxSpeed + 1) - i);

            gridXText.addText(4,i * scaleY - 3,QFont("Arial",8),yText);
        }
        for( i = 0; i < speed.count(); i++ )
        {
            //qDebug()<<"x % gridXStep "<<((int)x % gridXStep);
            if( (int)x % gridXStep == 0 )
            {
                gridXPath.moveTo(x * scaleX + leftMargin,0);
                gridXPath.lineTo(x * scaleX + leftMargin,ui->graph->height());
                QString xText;
                xText.setNum((int)x);
                int textMargin = 4;
                if( ((int)x / gridXStep) % 2 == 0 )
                {
                    textMargin = 18;
                }
                gridXText.addText(x * scaleX + leftMargin + 3,ui->graph->height() - textMargin,QFont("Arial",8),xText);
            }
            painterPath.lineTo(x * scaleX + leftMargin,graphHeight - ((qreal)speed.at(i) * scaleY));
            avgSpeedPath.lineTo(x * scaleX + leftMargin,graphHeight - ((qreal)avgSpeed.at(i) * scaleY));
            x++;
        }
        painterPath.lineTo(ui->graph->width(),graphHeight);
        avgSpeedPath.lineTo(ui->graph->width(),graphHeight);

        graphicsScene->setBackgroundBrush(QBrush(QColor(200, 200, 200)));
        ui->graph->setRenderHint(QPainter::Antialiasing);

        ui->graph->setScene(graphicsScene);
        ui->graph->setSceneRect(0,0,ui->graph->width(),ui->graph->height());
        graphicsPath = graphicsScene->addPath(gridXPath,QPen(QColor(100, 100, 100), 1, Qt::DashLine,
                                                Qt::FlatCap, Qt::MiterJoin),QColor(122, 163, 39, 0));
        graphicsPath = graphicsScene->addPath(gridXText,QPen(QColor(100, 100, 100), 1, Qt::SolidLine,
                                                Qt::FlatCap, Qt::MiterJoin),QColor(100, 100, 100));
        graphicsPath = graphicsScene->addPath(gridYPath,QPen(QColor(100, 100, 100), 1, Qt::DashLine,
                                                Qt::FlatCap, Qt::MiterJoin),QColor(122, 163, 39, 0));
        graphicsPath = graphicsScene->addPath(painterPath,QPen(QColor(79, 106, 25), 1, Qt::SolidLine,
                                                Qt::FlatCap, Qt::MiterJoin),QColor(122, 163, 39, 200));
        graphicsPath = graphicsScene->addPath(avgSpeedPath,QPen(QColor(106, 79, 25), 2, Qt::DashLine,
                                                Qt::FlatCap, Qt::MiterJoin),QColor(163, 122, 39,0));
        graphicsPath->setX(0);
        graphicsPath->setY(0);
    }

}
