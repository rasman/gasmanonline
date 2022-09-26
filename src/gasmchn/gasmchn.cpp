
#include "gasmchn.h"

#if QT_VERSION >=0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

int GasMchn::CPOS[ GasMchn::NEXT_SETTING_SUBSCR ] =
    {6, 32, 44, 54, 66, 78, 92};
//int GasMchn::CPOS[ GasMchn::NEXT_SETTING_SUBSCR ] =
//	{1, 27, 40, 52, 66, 78, 92};
int GasMchn::CSIZ[ GasMchn::NEXT_SETTING_SUBSCR] =
    {8, 8, 2, 8, 8, 8, 2};



GasMchn::GasMchn( QWidget *parent )  : QWidget(parent),
    m_propMac(0),
    m_hideNumerics( false ),
    m_propGasColorCKT(QColor(0,0xFF,0)),
    m_propGasColorALV(QColor(0,0x80,0)),
    m_propGasColorART(QColor(0xFF,0,0)),
    m_propGasColorVRG(QColor(0xFF,0x80,0x80)),
    m_propGasColorMUS(QColor(0x80,0,0)),
    m_propGasColorFAT(QColor(0xFF,0xFF,0)),
    m_propGasColorVEN(QColor(0,0,0xFF))
{
    for (int i = 0; i<NEXT_SETTING_SUBSCR; i++ )
        m_iSetting[i] = 0;
    m_propMac = 0;
    m_iMargin = 0;
    m_rcLast = QRect(-1,-2,-3,-4);

    m_pColor[CKT] = &m_propGasColorCKT;
    m_pColor[ALV] = &m_propGasColorALV;
    m_pColor[ART] = &m_propGasColorART;
    m_pColor[VRG] = &m_propGasColorVRG;
    m_pColor[MUS] = &m_propGasColorMUS;
    m_pColor[FAT] = &m_propGasColorFAT;
    m_pColor[VEN] = &m_propGasColorVEN;

    m_propGasLabelCKT = tr("CKT");
    m_propGasLabelALV = tr("ALV");
    m_propGasLabelART = tr("ART");
    m_propGasLabelVRG = tr("VRG");
    m_propGasLabelMUS = tr("MUS");
    m_propGasLabelFAT = tr("FAT");
    m_propGasLabelVEN = tr("VEN");

    m_propGasValueCKT = "0.00";
    m_propGasValueALV = "0.00";
    m_propGasValueART = "0.00";
    m_propGasValueVRG = "0.00";
    m_propGasValueMUS = "0.00";
    m_propGasValueFAT = "0.00";
    m_propGasValueVEN = "0.00";
}
QSize GasMchn::minimumSizeHint () const
{
    return QSize( 310, 150 );
}
void GasMchn::paintEvent(QPaintEvent * /*event*/)
{
    QPainter pdc( this );
    pdc.setRenderHint( QPainter::Antialiasing );
    //RecalcArea( this->rect() );
    PaintLabels( pdc );
    PaintGasmchn( pdc, -1);
}

void GasMchn::RecalcArea(const QRect& rcDraw)
{
    QRect rect;
    long left, i, cx, cy;

    if (rcDraw == m_rcLast)
        return;
    m_rcLast = rect = rcDraw;

    // reserve 15% top and bottom margin on client

    cy = rect.height();
    QFontMetrics fm( font() );
    m_iMargin = (int)(cy*15)/100;
    rect.adjust( 0, m_iMargin, 0, -m_iMargin - ( m_hideNumerics? 0 : fm.height() * 2 ) );

    // put the chambers where we're told to

    left = rect.left();
    cx = rect.width();
    for (i=0; i<NEXT_SETTING_SUBSCR; i++)
    {
        rect.setLeft( (int)((left*100L + CPOS[i]*cx)/100) );
        rect.setRight( (int)((rect.left()*100L + CSIZ[i]*cx)/100) );
        m_rcCompart[i] = rect;
    }
}

inline void GasMchn::GetMidpoints( QRect *rec, QPoint *top, QPoint *bot )
{
    int x = (rec->left() + rec->right()  )/2;
    top->setX( x );
    bot->setX( x ) ;
    bot->setY( rec->bottom() + 2 );
    top->setY( rec->top() );
}

void GasMchn::PaintGasmchn( QPainter& dc, short nCompart)
{
    QPen ppenOld;
    QBrush pbrOld;
    QPoint top, bot;
    QRect rc, rcALV;
    int i, j, m, MAC;
    int lev1, lev2, lev3;
    QPainterPath path,pathA,pathV;
    if (nCompart >=0) {	// Can we just redraw one compartment?

        // Figure the area that changed

        rc = m_rcCompart[nCompart];
        rc.adjust( 1, 1, -1, -1 );
        j = m_iSetting[nCompart];
        m = ( (100 - j)*rc.bottom() + j*rc.top() )/100;
        j = m_propMac;
        MAC = ( (100 - j)*rc.bottom() + j*rc.top() )/100;
        if (m < m_iOldSetting[nCompart]) { // Did it go up?
            rc.setTop( m ); rc.setBottom( m_iOldSetting[nCompart] );
            dc.fillRect( rc, QBrush( *m_pColor[nCompart] ));
        }
        else if (m > m_iOldSetting[nCompart]) // or down?
        {
            rc.setTop( m_iOldSetting[nCompart] );
            rc.setBottom( m );
            dc.fillRect( rc, QBrush( Qt::white ));
        }

        m_iOldSetting[nCompart] = m;
        if (   (nCompart==ALV || nCompart==VRG) && MAC >= rc.top() && MAC <  rc.bottom() ) {
            Qt::BGMode nOldMode = dc.backgroundMode();
            dc.setBackgroundMode( Qt::TransparentMode );
            ppenOld = dc.pen();
            QPen p;
            p.setColor( QColor(0,0,0) ); p.setWidth( 1 ); p.setStyle( Qt::DotLine );
            dc.setPen( p );
            QPainterPath path;
            path.moveTo(rc.left(), MAC);
            path.lineTo(rc.right(), MAC);
            dc.drawPath( path );
            dc.setPen( ppenOld );
            dc.setBackgroundMode( nOldMode );
        }
        return;
    }
    // Outline all compartments in black
    ppenOld = dc.pen();
    dc.setPen(Qt::black);

    pbrOld = dc.brush();
    dc.setBrush( Qt::white );

    // Draw and fill each compartment

    for (i=0; i<NEXT_SETTING_SUBSCR; i++)
    {
        rc = m_rcCompart[i];
        dc.drawRect(rc);
        rc.adjust( 1, 1, -1, -1 );
        j = m_iSetting[i];
        m = ( (100 - j)*rc.bottom() + j*rc.top() )/100;
        m_iOldSetting[i] = m;
        j = m_propMac;
        MAC = ( (100 - j)*rc.bottom() + j*rc.top() )/100;
        if (MAC < rc.top() || MAC >= rc.bottom() ) MAC = 0;
        if (m < rc.bottom() ) {
            rc.setTop( m );
            //dc.fillRect( rc, QBrush( *m_pColor[i] ) );
            QLinearGradient gradient(QPointF( rc.left()- 1, 0 ), QPointF( rc.right(), 0));
            gradient.setSpread( QGradient::RepeatSpread );
            gradient.setColorAt(0.0, *m_pColor[i] );
            gradient.setColorAt(0.3, Qt::white);
            gradient.setColorAt(0.6, *m_pColor[i] );
            gradient.setColorAt(0.7, *m_pColor[i] );
            gradient.setColorAt(1.0, (*m_pColor[i]).QColor::darker(300));
            dc.fillRect( rc, QBrush( gradient ) );

        }
        if ( MAC!=0 && (i==ALV || i==VRG)) {
            Qt::BGMode nOldMode = dc.backgroundMode();
            dc.setBackgroundMode( Qt::TransparentMode );
            ppenOld = dc.pen();
            QPen p;
            p.setColor( QColor(0, 0, 0) ); p.setWidth( 2 ); p.setStyle( Qt::DotLine );
            dc.setPen( p );
            path.moveTo(rc.left(), MAC);
            path.lineTo(rc.right(), MAC);
            dc.drawPath( path );
            dc.setPen( ppenOld );
            dc.setBackgroundMode( nOldMode );
        }
    }
    dc.setBrush( pbrOld);	// Restore old brush
    // Now draw circulatory system

#define GMP(x) GetMidpoints(m_rcCompart + x , &top, &bot);
    GMP(ALV);
    lev1 = top.y() - m_iMargin/2;				// Top margin
    lev2 = bot.y() + m_iMargin/4;				// Bot inside margin
    lev3 = bot.y() + (m_iMargin*2)/3;			// Bot outside margin

    rc.setBottom( (121*bot.y() - 21*top.y())/100 - 3 );	// Curve to left
    rcALV = m_rcCompart[ALV];				// of ALV
    rc.setRight( rcALV.left() + (121*(bot.y()-top.y()))/100 - 2 );
    i = (141*(bot.y() - top.y() ))/100;
    rc.setTop( rc.bottom() - i + 5 );
    rc.setLeft( rc.right() - i  );
    dc.drawArc( rc, 135 *16, 90*16 );

    i = (bot.y() - top.y())/8;						// Wishbone
    rc.setRight( rc.left() );
    rc.setLeft( m_rcCompart[CKT].right() );
    rc.setTop( rcALV.top() );
    rc.setBottom( rcALV.bottom() );
    j = (rc.right() + rc.left())/2;
    m = (rc.top() + rc.bottom())/2;
    path.moveTo( rc.left() + 2, rc.top() + i );
    path.lineTo(j, m);
    path.lineTo(rc.left() + 2, rc.bottom() - i );
    j += ( (i * (j-rc.left() ))/(m-rc.top() - i ) ) / 2;
    m = (2*m - i)/2;
    path.moveTo( rc.left() + 2, rc.top());
    path.lineTo( j, m);

    path.lineTo( rc.right(), m);
    path.moveTo( rc.right(), m+i);
    path.lineTo( j, m+i );
    path.lineTo( rc.left() + 2, rc.bottom() );
    dc.drawPath( path );
    QPen penA;
    penA.setStyle( Qt::SolidLine ); penA.setWidth( 3 ); penA.setColor( *m_pColor[ART] );
    QPen penV;
    penV.setStyle( Qt::SolidLine ); penV.setWidth( 3 ); penV.setColor( *m_pColor[VEN] );

    dc.setPen( penA );							//   -------------
    pathA.moveTo(top.x(), top.y()-1);			//   |           |
    pathA.lineTo(top.x(), lev1);				//  ALV         ART
    GMP(ART);
    pathA.lineTo(top.x(), lev1);
    pathA.lineTo(top.x(), top.y()-1);

    pathA.moveTo(bot.x(), bot.y());				//  ART         FAT
    pathA.lineTo(bot.x(), lev2 );				//   |           |
    GMP(FAT);									//   -------------
    pathA.lineTo(bot.x(), lev2);
    pathA.lineTo(bot.x(), bot.y());
    dc.drawPath( pathA );

    dc.setPen(penV);							//        |
    pathV.moveTo(top.x(), top.y()-1);			//       FAT
    pathV.lineTo(top.x(), lev1);

    GMP(MUS);									//        |
    pathV.moveTo(top.x(), top.y()-1);			//       MUS
    pathV.lineTo(top.x(), lev1);
    dc.drawPath( pathV );

    dc.setPen(penA);							//       MUS
    pathA.moveTo(bot.x(), bot.y() );			//        |
    pathA.lineTo(bot.x(), lev2 );

    GMP(VRG);									//   -------------
    pathA.moveTo(bot.x(), bot.y() );			//   |           |
    pathA.lineTo(bot.x(), lev2 );				//  VRG         VEN
    dc.drawPath( pathA );
    dc.setPen(penV);							//   |
    pathV.moveTo(top.x(), top.y()-1);
    pathV.lineTo(top.x(), lev1);
    GMP(VEN);
    pathV.lineTo(top.x(), lev1);
    pathV.lineTo(top.x(), top.y()-1);

    pathV.moveTo(bot.x(), bot.y());				//  ALV         VEN
    pathV.lineTo(bot.x(), lev3);				//   |           |
    GMP(ALV);									//   -------------
    pathV.lineTo(bot.x(), lev3);
    pathV.lineTo(bot.x(), bot.y());
    dc.drawPath( pathV );

#undef GMP
    dc.setPen(ppenOld);	// Restore old pen
    dc.end();
}

QPoint GasMchn::GetInjectPoint()
{
    QRect rcCKT = m_rcCompart[CKT];	// of CKT
    QRect rcALV = m_rcCompart[ALV];	// of ALV
    int dx = ( rcALV.left() - rcCKT.right() ) / 4;
    int dy = ( rcCKT.bottom() - rcCKT.top() ) / 4;
    QPoint injectPoint = QPoint( rcCKT.right() + dx, rcCKT.top() + dy  );
    return mapToGlobal( injectPoint );
    //return mapToGlobal( m_injectPoint );
}
/////////////////////////////////////////////////////////////////////////////
// CGasmchn properties

short GasMchn::GetCKT() const
{
    return m_iSetting[CKT];
}

void GasMchn::SetCKT(short propVal)
{
    m_iSetting[CKT] = propVal;
    update();
}

short GasMchn::GetALV() const
{
    return m_iSetting[ALV];
}

void GasMchn::SetALV(short propVal)
{
    m_iSetting[ALV] = propVal;
    update();
}

short GasMchn::GetART() const
{
    return m_iSetting[ART];
}

void GasMchn::SetART(short propVal)
{
    m_iSetting[ART] = propVal;
    update();
}

short GasMchn::GetVRG() const
{
    return m_iSetting[VRG];
}

void GasMchn::SetVRG(short propVal)
{
    m_iSetting[VRG] = propVal;
    update();
}

short GasMchn::GetMUS() const
{
    return m_iSetting[MUS];
}

void GasMchn::SetMUS(short propVal)
{
    m_iSetting[MUS] = propVal;
    update();
}

short GasMchn::GetFAT() const
{
    return m_iSetting[FAT];
}

void GasMchn::SetFAT(short propVal)
{
    m_iSetting[FAT] = propVal;
    update();
}

short GasMchn::GetVEN() const
{
    return m_iSetting[VEN];
}

void GasMchn::SetVEN(short propVal)
{
    m_iSetting[VEN] = propVal;
    update();
}

QColor GasMchn::GetGasColorCKT() const
{
    return m_propGasColorCKT;
}

void GasMchn::SetGasColorCKT( const QColor &propVal)
{
    m_propGasColorCKT = propVal;
}

QColor GasMchn::GetGasColorALV() const
{
    return m_propGasColorALV;
}

void GasMchn::SetGasColorALV( const QColor &propVal)
{
    m_propGasColorALV = propVal;
}

QColor GasMchn::GetGasColorART() const
{
    return m_propGasColorART;
}

void GasMchn::SetGasColorART( const QColor &propVal)
{
    m_propGasColorART = propVal;
}

QColor GasMchn::GetGasColorVRG() const
{
    return m_propGasColorVRG;
}

void GasMchn::SetGasColorVRG( const QColor &propVal)
{
    m_propGasColorVRG = propVal;
}

QColor GasMchn::GetGasColorMUS() const
{
    return m_propGasColorMUS;
}

void GasMchn::SetGasColorMUS( const QColor &propVal)
{
    m_propGasColorMUS = propVal;
}

QColor GasMchn::GetGasColorFAT() const
{
    return m_propGasColorFAT;
}

void GasMchn::SetGasColorFAT( const QColor &propVal)
{
    m_propGasColorFAT = propVal;
}

QColor GasMchn::GetGasColorVEN() const
{
    return m_propGasColorVEN;
}

void GasMchn::SetGasColorVEN( const QColor &propVal)
{
    m_propGasColorVEN = propVal;
}

QString GasMchn::GetGasLabelCKT() const
{
    return m_propGasLabelCKT;
}

void GasMchn::SetGasLabelCKT( const QString &propVal )
{
    m_propGasLabelCKT = propVal;
}

QString GasMchn::GetGasLabelALV() const
{
    return m_propGasLabelALV;
}

void GasMchn::SetGasLabelALV( const QString &propVal )
{
    m_propGasLabelALV = propVal;
}

QString GasMchn::GetGasLabelART() const
{
    return m_propGasLabelART;
}

void GasMchn::SetGasLabelART( const QString &propVal )
{
    m_propGasLabelART = propVal;
}

QString GasMchn::GetGasLabelVRG() const
{
    return m_propGasLabelVRG;
}

void GasMchn::SetGasLabelVRG( const QString &propVal )
{
    m_propGasLabelVRG = propVal;
}

QString GasMchn::GetGasLabelMUS() const
{
    return m_propGasLabelMUS;
}

void GasMchn::SetGasLabelMUS( const QString &propVal )
{
    m_propGasLabelMUS = propVal;
}

QString GasMchn::GetGasLabelFAT() const
{
    return m_propGasLabelFAT;
}
void GasMchn::SetGasLabelFAT( const QString &propVal )
{
    m_propGasLabelFAT = propVal;
}

QString GasMchn::GetGasLabelVEN() const
{
    return m_propGasLabelVEN;
}

void GasMchn::SetGasLabelVEN( const QString &propVal )
{
    m_propGasLabelVEN = propVal;
}


QString GasMchn::GetGasValueCKT() const
{
    return m_propGasValueCKT;
}

void GasMchn::SetGasValueCKT( const QString &propVal )
{
    m_propGasValueCKT = propVal;
}

QString GasMchn::GetGasValueALV() const
{
    return m_propGasValueALV;
}

void GasMchn::SetGasValueALV( const QString &propVal )
{
    m_propGasValueALV = propVal;
}

QString GasMchn::GetGasValueART() const
{
    return m_propGasValueART;
}

void GasMchn::SetGasValueART( const QString &propVal )
{
    m_propGasValueART = propVal;
}

QString GasMchn::GetGasValueVRG() const
{
    return m_propGasValueVRG;
}

void GasMchn::SetGasValueVRG( const QString &propVal )
{
    m_propGasValueVRG = propVal;
}

QString GasMchn::GetGasValueMUS() const
{
    return m_propGasValueMUS;
}

void GasMchn::SetGasValueMUS( const QString &propVal )
{
    m_propGasValueMUS = propVal;
}

QString GasMchn::GetGasValueFAT() const
{
    return m_propGasValueFAT;
}
void GasMchn::SetGasValueFAT( const QString &propVal )
{
    m_propGasValueFAT = propVal;
}

QString GasMchn::GetGasValueVEN() const
{
    return m_propGasValueVEN;
}

void GasMchn::SetGasValueVEN( const QString &propVal )
{
    m_propGasValueVEN = propVal;
}

short GasMchn::GetMAC() const
{
    return m_propMac;
}

void GasMchn::SetMAC(short propVal)
{
    m_propMac = propVal;
    update();
}

void GasMchn::resizeEvent ( QResizeEvent * e )
{
    QWidget::resizeEvent( e );
    RecalcArea( this->rect() );
    emit Resized();
}

// Paint labels in white space under tubes
void GasMchn::PaintLabels( QPainter& dc )
{
    if ( m_hideNumerics )
        return;
    int xl,yl,xv,yv;
    QRect rec,bRectL,bRectV;
    QPoint pos;
    QFontMetrics fm( font() );
#define DRAW(X)	rec = Get##X##Rect(); \
        pos = rec.bottomLeft(); \
        bRectL = fm.boundingRect( m_propGasLabel##X ); \
        xl = pos.x() + ( rec.width() - bRectL.width() ) / 2; \
        yl = this->rect().height() - 3; \
        dc.drawText( xl, yl , m_propGasLabel##X ); \
        \
        bRectV = fm.boundingRect( m_propGasValue##X ); \
        xv = pos.x() + ( rec.width() - bRectV.width() ) / 2; \
        yv = yl - bRectL.height() - 3; \
        dc.drawText( xv, yv , m_propGasValue##X );

    DRAW( CKT )
    DRAW( ALV )
    DRAW( ART )
    DRAW( VRG )
    DRAW( MUS )
    DRAW( FAT )
    DRAW( VEN )
#undef DRAW
}

void GasMchn::hideNumerics( bool set )
{
    m_hideNumerics = set;
    m_rcLast = QRect();
    update();
}
