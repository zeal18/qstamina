#ifndef TRESULTS_H
#define TRESULTS_H

#include <QDialog>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsPathItem>
#include <QGraphicsScene>

#include <qmath.h>

namespace Ui {
class TResults;
}

class TResults : public QDialog
{
    Q_OBJECT
    
public:
    explicit TResults(QWidget *parent = 0);
    ~TResults();

    void setErrors(QString);
    void setRights(QString);
    void setTime(QString);
    void setSpeed(QString);
    void drawGraph(QList<int>,QList<float>);
    
private:
    Ui::TResults *ui;
};

#endif // TRESULTS_H
