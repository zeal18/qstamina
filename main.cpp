#include <QtGui/QApplication>
#include "tstamina.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TStamina w;
    w.show();
    
    return a.exec();
}
