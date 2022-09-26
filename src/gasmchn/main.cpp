
#include <QApplication>

#include "gasmchn.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget w;
    w.resize(600, 600);
    GasMchn *mchn = new GasMchn( &w );
    mchn->SetCKT( 10 );
    mchn->SetALV( 15 );
    mchn->SetART( 5 );
    mchn->SetVRG( 20 );
    mchn->SetMUS( 40 );
    mchn->SetFAT( 55 );
    mchn->SetVEN( 34 );
    mchn->SetMAC( 10 );
    //gauge->setAttribute( Qt::WA_DeleteOnClose ,true );
    mchn->setGeometry(100, 100, 1000,150);
    w.show();
    return app.exec();
}
