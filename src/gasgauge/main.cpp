
#include <QApplication>

#include "gasgauge.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget w;
    w.resize(600, 600);
    GasGauge *gauge = new GasGauge( &w );
    //gauge->setAttribute( Qt::WA_DeleteOnClose ,true );
    gauge->setGeometry(100, 100,50,100);
    w.show();
    return app.exec();
}
