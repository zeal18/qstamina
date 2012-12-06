#include "tresults.h"
#include "ui_tresults.h"

TResults::TResults(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TResults)
{
    ui->setupUi(this);
    ui->graph->setGeometry(0,0,450,200);
}

TResults::~TResults()
{
    delete ui;
}
void TResults::setErrors(QString errors)
{
    ui->lblErrors->setText(errors);
}

void TResults::setRights(QString rights)
{
    ui->lblAmount->setText(rights);
}

void TResults::setTime(QString time)
{
    ui->lblTime->setText(time);
}

void TResults::setSpeed(QString speed)
{
    ui->lblSpeed->setText(speed);
}

void TResults::drawGraph(QList<int> speed, QList<float> avgSpeed)
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
