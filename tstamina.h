#ifndef TSTAMINA_H
#define TSTAMINA_H

#include <QMainWindow>

namespace Ui {
class TStamina;
}

class TStamina : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit TStamina(QWidget *parent = 0);
    ~TStamina();
    
private:
    Ui::TStamina *ui;
};

#endif // TSTAMINA_H
