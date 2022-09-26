
#include <QApplication>

#include "gasanibutton.h"

int main( int argc, char **argv )
{
    QApplication app( argc, argv );

    GasAniButton *b = new GasAniButton( );
    b->setGeometry( 10, 10, 32, 32 );
    b->show();

    return app.exec();
}
