
#include "gasanibutton.h"

#if QT_VERSION >=0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif


GasAniButton::GasAniButton( QWidget *parent )
    : QAbstractButton( parent )
{
    setFixedSize( QSize( 32, 32 ) );		//Set size for the icon
}

void GasAniButton::paintEvent( QPaintEvent * /*evnt*/ )
{
    QPainter p( this );

    //Draw the icon on the button
    if ( isDown() )
        p.drawPixmap( rect(), srynge_down );
    else
        p.drawPixmap( rect(), srynge_up );

    if ( !hasFocus() ) return;

    //Draw dotted rectangle if button has focus
    p.setPen( Qt::DotLine );
    p.drawRect( rect().x(), rect().y(), rect().width() - 1, rect().height() - 1 );
}
