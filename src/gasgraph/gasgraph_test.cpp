#include "gasgraph_test.h"

TestForm::TestForm(): QWidget()
{
    setupUi( this );
}

void TestForm::on_hScrollBar_valueChanged ( int value )
{
    gasGraph->SetXVal( value );
}

void TestForm::customEvent ( QEvent * event )
{
    if ( event->type() == QEvent::User + 1 ) {
        GasGraphEvent * gasGraphEvent = static_cast< GasGraphEvent * >( event );
        float * pf = gasGraphEvent->data();
        float *y = pf + 1;
        int rnd = rand()%10;
        *y = rnd;
    }
}
