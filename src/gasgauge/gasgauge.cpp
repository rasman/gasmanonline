
#include "gasgauge.h"

#if QT_VERSION >=0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif


int GasGauge::mulDiv( int p1, int p2, int p3 )
{
    qint64 i64 = p1 * p2;
    return i64 / p3;
}

GasGauge::GasGauge(QWidget *parent)
    : QWidget(parent)
{
    m_iMax = 100;
    m_iValue = 50;
    m_iTickEvery = 10;
    m_iLTickEvery = 50;
    m_clrBarColor = QColor(0,0,0xFF);

    m_bCapturing = false;
}

void GasGauge::paintEvent(QPaintEvent * /*event*/)
{
    int i, j, k;
    QPainter pdc( this );
    QRect rcBounds = this->rect();//event->rect();
    QRect rcBar = rcBounds;

    // Outline compartment in black

    QPen ppenOld = pdc.pen();
    pdc.setPen( Qt::black );

    // Background of compartment is white

    QBrush pbrOld = pdc.brush();
    pdc.setBrush( Qt::white );

    // Draw and fill compartment

    rcBar.setLeft( rcBar.left() + ( rcBar.width() / 2) );
    rcBar.adjust( 0, 0, -1, -1 );
    pdc.drawRect( rcBar );
    //pdc.drawRect( rcBar.x(), rcBar.y(), rcBar.width() - 1, rcBar.height() -1 );
    rcBar.adjust( 1, 1, 0, 0 );
    pdc.setBrush( pbrOld );

    // Draw the tick marks

    if (m_iTickEvery) {
        for ( i = 0; i<=m_iMax; i+= m_iTickEvery) {
            j = mulDiv( rcBar.height() + 1, m_iMax - i, m_iMax );
            j += rcBar.top() - 1;
            k = i%m_iLTickEvery? ( rcBounds.left() + rcBar.left() )/2 : rcBounds.left();
            pdc.drawLine( k, j , rcBar.left() - 1 , j );
        }
    }
    pdc.setPen( ppenOld );

    // Draw in the bar

    i = m_iValue;
    m_iValue = 0;
    MoveGaugeTo( &pdc, rcBar, i);
}

void GasGauge::MoveGaugeTo( QPainter *pdc, QRect& rcBar, int iValue)
{
    QRect rc;
    int mNew, mOld;
    mOld = LtoD(rcBar, m_iValue);
    if (iValue < 0) iValue = 0; else if (iValue > m_iMax) iValue = m_iMax;
    mNew = LtoD(rcBar, iValue);

    if (mNew < mOld)	// Did it go up?
    {
        rc.setTop( mNew ); rc.setBottom( mOld - 1 );
        rc.setLeft( rcBar.left() ); rc.setRight( rcBar.right() );
        QLinearGradient gradient(QPointF( rc.left() - 1 , 0), QPointF( rc.right(), 0));
        gradient.setSpread( QGradient::RepeatSpread );
        gradient.setColorAt(0.0, m_clrBarColor );
        gradient.setColorAt(0.3, Qt::white);
        gradient.setColorAt(0.6, m_clrBarColor );
        gradient.setColorAt(0.7, m_clrBarColor );
        gradient.setColorAt(1.0, m_clrBarColor.QColor::darker(300));
        pdc->fillRect( rc, QBrush( gradient ) );
        //pdc->fillRect( rc, QBrush( m_clrBarColor ) );
    }
    else if (mNew > mOld) // or down?
    {
        rc.setBottom( mNew -1 ); rc.setTop( mOld );
        rc.setLeft( rcBar.left() ); rc.setRight( rcBar.right() );
        pdc->fillRect( rc, QBrush( Qt::white ) );
    }
    m_iValue = iValue;
}

int GasGauge::LtoD( QRect& rcBar, int iValue )
{
    int nY;
    if (iValue < 0) iValue = 0; else if (iValue > m_iMax) iValue = m_iMax;
    nY = mulDiv( rcBar.bottom() + 1 - rcBar.top() , m_iMax - iValue, m_iMax);
    return nY + rcBar.top();
}

int GasGauge::DtoL(QRect& rcBar, int nY)
{
    if (nY < rcBar.top()) nY = rcBar.top();
    else if (nY > rcBar.bottom() +1 ) nY = rcBar.bottom() + 1 ;
    return mulDiv(m_iMax, rcBar.bottom() +1 -nY, rcBar.bottom() +1 - rcBar.top() );
}

void GasGauge::RationalizeParams()
{
    // sizes are positive

    if (m_iMax <= 0) m_iMax = 10;	// Arbitrarily...

    // tick_every <= size; ltick_every % tick_every == 0

    if (m_iTickEvery != 0)	{
        if (m_iTickEvery < 0 || m_iTickEvery > m_iMax)
            m_iTickEvery = m_iMax;
        if (m_iLTickEvery < 0 || m_iLTickEvery%m_iTickEvery != 0)
            m_iLTickEvery = m_iTickEvery;
    }
    update();
}

void GasGauge::mousePressEvent ( QMouseEvent * event )
{
    if ( event->button() == Qt::LeftButton ) {
        if (!m_bCapturing) {
            QRect rcBar = rect();
            rcBar.setLeft( rcBar.left() + ( rcBar.width() / 2) );
            rcBar.adjust( 1, 1, -1, -1 );
            if ( rcBar.contains( event->pos() ) ) {
                grabMouse();
                m_bCapturing = true;
                m_bStartStatus = false;
                emit FireGasgaugeStart();
                if ( m_bStartStatus )
                    m_iOffset = LtoD(rcBar, m_iValue) - event->pos().y();
                else
                {
                    releaseMouse();
                    m_bCapturing = false;
                }
            }
        }
    }
}

void GasGauge::mouseReleaseEvent ( QMouseEvent * event )
{
    if ( event->button() == Qt::LeftButton ) {
        if ( m_bCapturing ) {
            releaseMouse();
            m_bCapturing = false;
            emit FireGasgaugeStop();
        }
    }
}

void GasGauge::mouseMoveEvent ( QMouseEvent * event )
{
    if (m_bCapturing) {
        int iNewValue;
        QRect rcBar = rect();
        rcBar.setLeft( rcBar.left() + ( rcBar.width() / 2) );
        rcBar.adjust( 1, 1, -1, -1 );
        iNewValue = DtoL(rcBar, event->pos().y() + m_iOffset);
        if (iNewValue != m_iValue) {
            short iNV = (short)iNewValue;
            m_iValue = iNewValue;
            emit FireGasgaugeChange(iNV);
            update();
        }
    }
}

short GasGauge::GetMax()
{
    return m_iMax;
}

void GasGauge::SetMax(short iMax)
{
    m_iMax = iMax;
    RationalizeParams();
}

short GasGauge::GetValue()
{
    return m_iValue;
}

void GasGauge::SetValue(short iValue)
{
    m_iValue = iValue;
    update();
}

short GasGauge::GetTickEvery()
{
    return m_iTickEvery;
}

void GasGauge::SetTickEvery(short iTickEvery)
{
    m_iTickEvery = iTickEvery;
    RationalizeParams();
}

short GasGauge::GetLTickEvery()
{
    return m_iLTickEvery;
}

void GasGauge::SetLTickEvery(short iLTickEvery)
{
    m_iLTickEvery = iLTickEvery;
    RationalizeParams();
}

QColor GasGauge::GetBarColor()
{
    return m_clrBarColor;
}

void GasGauge::SetBarColor( QColor clrBarColor)
{
    m_clrBarColor = clrBarColor;
    update();
}

bool GasGauge::GetStartStatus()
{
    return m_bStartStatus;
}

void GasGauge::SetStartStatus(bool bStatus)
{
    m_bStartStatus = bStatus;
}
