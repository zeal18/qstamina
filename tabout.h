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
