
#include "gasgraph.h"

#if QT_VERSION >=0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif
#include <math.h>


static int cxPhyZ, cyPhyZ, cxPhySize, cyPhySize,
           cxLogZ, cyLogZ, cxLogSize, cyLogSize, cxLogMax, cyLogMax,
           sgnY, sgnX;

GasGraph::GasGraph(  QWidget *parent ) : QWidget( parent ),
    m_propLoading(false),
    m_propScrollEnb(false),
    m_propLMarginPct(10),
    m_propRMarginPct(5),
    m_propTMarginPct(10),
    m_propBMarginPct(10),
    m_propOrigin(0.0F),
    m_propMac(0.0F),
    m_propXVal(0.0F),
    m_propXValEvery(1.0F),
    m_propMask(0),
    m_propLineLabelType(0),
    m_propLineWidth(1),
    m_propVals(1),
    m_propXLabelType(1),
    m_propYLabelType(0),
    m_propPaperColor( QColor(0xC0,0xC0,0xC0)),
    m_propFontHeight(12),
    receiver( 0 ),
    zoom(1),
    line(false)
{
    paintDevice = this;

    int i;

    m_propColor[0] = QColor(0xFF, 0xFF, 0);
    m_propColor[1] = QColor(0x80, 0, 0);
    m_propColor[2] = QColor(0xFF, 0, 0);
    m_propColor[3] = QColor(0, 0x80, 0);
    m_propColor[4] = QColor(0, 0xFF, 0);

    for (i = 0; i<NEXT_SETTING_SUBSCR; i++)
        PARAM(i) = 0.0F;
    for (i = 0; i<MAXFN; i++)
        m_propLabelChar[i] = (short)('1' + i);

    m_propXPhySize = 0;
    m_propYPhySize = 0;
    m_propXPhyZero = 0;
    m_propYPhyZero = 0;

    RationalizeParams();

    setType(GRAPHTYPE_OTHER);
    setMouseTracking(true);
}

bool GasGraph::GetLoading()
{
    return m_propLoading;
}

void GasGraph::SetLoading(bool propVal)
{
    m_propLoading = propVal;
    if (!m_propLoading)
        update();
}

bool GasGraph::GetScrollEnb()
{
    return m_propScrollEnb;
}

void GasGraph::SetScrollEnb(bool propVal)
{
    m_propScrollEnb = propVal;
}

QColor GasGraph::GetColor0()
{
    return m_propColor[0];
}

void GasGraph::SetColor0(const QColor & propVal)
{
    m_propColor[0] = propVal;
    ReactToParam();
}

QColor GasGraph::GetColor1()
{
    return m_propColor[1];
}

void GasGraph::SetColor1(const QColor & propVal)
{
    m_propColor[1] = propVal;
    ReactToParam();
}

QColor GasGraph::GetColor2()
{
    return m_propColor[2];
}

void GasGraph::SetColor2(const QColor & propVal)
{
    m_propColor[2] = propVal;
    ReactToParam();
}

QColor GasGraph::GetColor3()
{
    return m_propColor[3];
}

void GasGraph::SetColor3(const QColor & propVal)
{
    m_propColor[3] = propVal;
    ReactToParam();
}

QColor GasGraph::GetColor4()
{
    return m_propColor[4];
}

void GasGraph::SetColor4(const QColor & propVal)
{
    m_propColor[4] = propVal;
    ReactToParam();
}

short GasGraph::GetLMarginPct()
{
    return m_propLMarginPct;
}

void GasGraph::SetLMarginPct(short propVal)
{
    m_propLMarginPct = propVal;
    ReactToParam();
}

short GasGraph::GetRMarginPct()
{
    return m_propRMarginPct;
}

void GasGraph::SetRMarginPct(short propVal)
{
    m_propRMarginPct = propVal;
    ReactToParam();
}

short GasGraph::GetTMarginPct()
{
    return m_propTMarginPct;
}

void GasGraph::SetTMarginPct(short propVal)
{
    m_propTMarginPct = propVal;
    ReactToParam();
}

short GasGraph::GetBMarginPct()
{
    return m_propBMarginPct;
}

void GasGraph::SetBMarginPct(short propVal)
{
    m_propBMarginPct = propVal;
    ReactToParam();
}

short GasGraph::GetXPhySize()
{
    return m_propXPhySize;
}

void GasGraph::SetXPhySize(short propVal)
{
    m_propXPhySize = propVal;
}

short GasGraph::GetYPhySize()
{
    return m_propYPhySize;
}

void GasGraph::SetYPhySize(short propVal)
{
    m_propYPhySize = propVal;
}

short GasGraph::GetXPhyZero()
{
    return m_propXPhyZero;
}

void GasGraph::SetXPhyZero(short propVal)
{
    m_propXPhyZero = propVal;
}

short GasGraph::GetYPhyZero()
{
    return m_propYPhyZero;
}

void GasGraph::SetYPhyZero(short propVal)
{
    m_propYPhyZero = propVal;
}

float GasGraph::GetOrigin()
{
    return m_propOrigin;
}

void GasGraph::SetOrigin(float propVal)
{
    m_propOrigin = propVal;
    ReactToParam();
}

float GasGraph::GetMac()
{
    return m_propMac;
}

void GasGraph::SetMac(float propVal)
{
    m_propMac = propVal;
    ReactToParam();
}

float GasGraph::GetXVal()
{
    return m_propXVal;
}

void GasGraph::SetXVal(float propVal)
{
    m_propXVal = propVal;
    update();
}

float GasGraph::GetXValEvery()
{
    return m_propXValEvery;
}

void GasGraph::SetXValEvery(float propVal)
{
    m_propXValEvery = propVal;
    RationalizeParams();
}

short GasGraph::GetMask()
{
    return m_propMask;
}

void GasGraph::SetMask(short propVal)
{
    m_propMask = propVal;
}

short GasGraph::GetLabelChar0()
{
    return m_propLabelChar[0];
}

void GasGraph::SetLabelChar0(short propVal)
{
    m_propLabelChar[0] = propVal;
    RationalizeParams();
}

short GasGraph::GetLabelChar1()
{
    return m_propLabelChar[1];
}

void GasGraph::SetLabelChar1(short propVal)
{
    m_propLabelChar[1] = propVal;
    RationalizeParams();
}

short GasGraph::GetLabelChar2()
{
    return m_propLabelChar[2];
}

void GasGraph::SetLabelChar2(short propVal)
{
    m_propLabelChar[2] = propVal;
    RationalizeParams();
}

short GasGraph::GetLabelChar3()
{
    return m_propLabelChar[3];
}

void GasGraph::SetLabelChar3(short propVal)
{
    m_propLabelChar[3] = propVal;
    RationalizeParams();
}

short GasGraph::GetLabelChar4()
{
    return m_propLabelChar[4];
}

void GasGraph::SetLabelChar4(short propVal)
{
    m_propLabelChar[4] = propVal;
    RationalizeParams();
}

short GasGraph::GetLineLabelType()
{
    return m_propLineLabelType;
}

void GasGraph::SetLineLabelType(short propVal)
{
    m_propLineLabelType = propVal;
    RationalizeParams();
}

short GasGraph::GetLineWidth()
{
    return m_propLineWidth;
}

void GasGraph::SetLineWidth(short propVal)
{
    m_propLineWidth = propVal;
    RationalizeParams();
}

short GasGraph::GetVals()
{
    return m_propVals;
}

void GasGraph::SetVals(short propVal)
{
    m_propVals = propVal;
    RationalizeParams();
}

short GasGraph::GetXLabelType()
{
    return m_propXLabelType;
}

void GasGraph::SetXLabelType(short propVal)
{
    m_propXLabelType = propVal;
    RationalizeParams();
}

short GasGraph::GetYLabelType()
{
    return m_propYLabelType;
}

void GasGraph::SetYLabelType(short propVal)
{
    m_propYLabelType = propVal;
    RationalizeParams();
}

QColor GasGraph::GetPaperColor()
{
    return m_propPaperColor;
}

void GasGraph::SetPaperColor(const QColor & propVal)
{
    m_propPaperColor = propVal;
    RationalizeParams();
}

float GasGraph::GetXLabelEvery()
{
    return PARAM(XLABEL_EVERY);
}

void GasGraph::SetXLabelEvery(float propVal)
{
    m_fXLabelEverySet = propVal;
    //PARAM(XLABEL_EVERY) = propVal;
    ReactToParam();
}

float GasGraph::GetXMin()
{
    return PARAM(XMIN);
}

void GasGraph::SetXMin(float propVal)
{
    PARAM(XMIN) = propVal;
    ReactToParam();
}

float GasGraph::GetXSize()
{
    return PARAM(XSIZE);
}

void GasGraph::SetXSize(float propVal)
{
    PARAM(XSIZE) = propVal;
    ReactToParam();
}

float GasGraph::GetXTickEvery()
{
    return PARAM(XTICK_EVERY);
}

void GasGraph::SetXTickEvery(float propVal)
{
    m_fXTickEverySet = propVal;
    //PARAM(XTICK_EVERY) = propVal;
    ReactToParam();
}

float GasGraph::GetYLabelEvery()
{
    return PARAM(YLABEL_EVERY);
}

void GasGraph::SetYLabelEvery(float propVal)
{
    m_fYLabelEverySet = propVal;
    //PARAM(YLABEL_EVERY) = propVal;
    ReactToParam();
}

float GasGraph::GetYMin()
{
    return PARAM(YMIN);
}

void GasGraph::SetYMin(float propVal)
{
    PARAM(YMIN) = propVal;
    ReactToParam();
}

float GasGraph::GetYSize()
{

    return PARAM(YSIZE);
}

void GasGraph::SetYSize(float propVal)
{


    PARAM(YSIZE) = propVal;

    if(getType() == GRAPHTYPE_IARM)
        m_ySize = propVal/*/zoom*/;
    else
        m_ySize = propVal;


    ReactToParam();
}

float GasGraph::GetYTickEvery()
{
    return PARAM(YTICK_EVERY);
}

void GasGraph::SetYTickEvery(float propVal)
{
    m_fYTickEverySet = propVal;
    //PARAM(YTICK_EVERY) = propVal;
    ReactToParam();
}

QColor GasGraph::GetBackColor()
{
    return m_propBackColor;
}

void GasGraph::SetBackColor(const QColor & propVal)
{
    m_propBackColor = propVal;
    RationalizeParams();
}

short GasGraph::GetFontHeight()
{
    return m_propFontHeight;
}

void GasGraph::SetFontHeight(short propVal)
{
    m_propFontHeight = propVal;
    ReactToParam();
}

void GasGraph::ReactToParam(void)
{
    RationalizeParams();

    if (!m_propLoading)
        update();

}

void GasGraph::RationalizeParams(void)
{

    float *pf, f1, f2;
    int i;

    // int ytic = PARAM(YTICK_EVERY);

    if (m_propVals > MAXFN) m_propVals = MAXFN;
    pf = m_propSettings.rgfSetting;

    // sizes are positive

    if (pf[XSIZE] <= 0) pf[XSIZE] = 100.0F;	// Arbitrarily...
    if (pf[YSIZE] <= 0) pf[YSIZE] = 10.0F;

    // start with requested
    PARAM(XLABEL_EVERY) = m_fXLabelEverySet;
    PARAM(XTICK_EVERY) = m_fXTickEverySet;
    PARAM(YLABEL_EVERY) = m_fYLabelEverySet;
    PARAM(YTICK_EVERY) = m_fYTickEverySet;

    // tick_every <= label_every <= size

    for (i = 0; i <= YMIN-XMIN; i += YMIN-XMIN)	// NOTE: Structs are PARALLEL
    {

#		define PARAMb(idx) pf[i+X##idx]

        if (PARAMb(LABEL_EVERY)!=0.0F)	//Want labels?
        {
            if (   PARAMb(LABEL_EVERY) > PARAMb(SIZE)
                || PARAMb(LABEL_EVERY) < PARAMb(SIZE)/10.0F
               )
                PARAMb(LABEL_EVERY) = PARAMb(SIZE);
            if (PARAMb(TICK_EVERY)==0.0F)	//if so, must have ticks
                PARAMb(TICK_EVERY) = PARAMb(LABEL_EVERY);
        }
        if (PARAMb(TICK_EVERY)!=0.0F)	//Want or need ticks?
        {
            f1 = (float)fabs(fmod(PARAMb(LABEL_EVERY), PARAMb(TICK_EVERY)));
            f2 = PARAMb(TICK_EVERY) - f1;
            f1 = qMin(f1,f2);
            if (   PARAMb(TICK_EVERY) > PARAMb(LABEL_EVERY)
                || PARAMb(TICK_EVERY) < PARAMb(LABEL_EVERY)/50.0F
                || f1 > PARAMb(TICK_EVERY)/1000.0F
               )
                PARAMb(TICK_EVERY) =  PARAMb(LABEL_EVERY)/5.0f;
        }

#		undef PARAMb

    }

    // 0 < value_every <= tick_every (if there IS a tick_every)

    if (   (pf[XTICK_EVERY]!= 0.0F && m_propXValEvery > pf[XTICK_EVERY]) // parenthesis not in original code
        || m_propXValEvery > pf[XSIZE]
        || m_propXValEvery <= 0
       )
    {
        if (pf[XTICK_EVERY]!=0.0F)
            m_propXValEvery = pf[XTICK_EVERY];
        else
            m_propXValEvery = pf[XSIZE];
    }

    // 5 <= Margin% <= 75 left and bottom

    if ( m_propBMarginPct < 5 )  m_propBMarginPct = 5;
    if ( m_propLMarginPct < 5 )  m_propLMarginPct = 5;
    if ( m_propBMarginPct > 75 ) m_propBMarginPct = 75;
    if ( m_propLMarginPct > 75 ) m_propLMarginPct = 75;

    // 0 <= Margin% <= 50 top and right

    if ( m_propTMarginPct < 0 )  m_propTMarginPct = 0;
    if ( m_propRMarginPct < 0 )  m_propRMarginPct = 0;
    if ( m_propTMarginPct > 90 - m_propBMarginPct )
        m_propTMarginPct = 90 - m_propBMarginPct;
    if ( m_propRMarginPct > 90 - m_propLMarginPct )
        m_propRMarginPct = 90 - m_propLMarginPct;
    if ( m_propTMarginPct > 50 ) m_propTMarginPct = 50;
    if ( m_propRMarginPct > 50 ) m_propRMarginPct = 50;

    // left >= min

    if (m_propOrigin < pf[XMIN]) m_propOrigin = pf[XMIN];


}
//---------------------------------------------------------------------------
// Remember the device placement (physical). Sometimes, this is set
// by the caller to some physical area on, let's say, a printer
// *before* drawing, by setting corresponding properties. The caller
// clues us to this by sending a special WM_PAINT message with a
// pointer to a pre-set DC in it. When we notice the special message,
// this routine is skipped and we use the physical area given in
// the properties to draw a graph.
//---------------------------------------------------------------------------
void GasGraph::SetPhysLoc( QPainter &/*pdc*/, const QRect & rect)
{
    m_propXPhyZero = (short)rect.left();
    m_propYPhyZero = (short)rect.top();
    m_propXPhySize = (short)rect.width();
    m_propYPhySize = (short)rect.height();
}

//---------------------------------------------------------------------------
// Map floating point to identical float, logical, and physical coordinates
// constrained to the interior of the graph iff bConstrain
//---------------------------------------------------------------------------

void GasGraph::FPtoDP( QPainter &/*pdc*/, bool bConstrain,
                float fX, float fY,
                float *pfXRnd, float *pfYRnd,
                QPoint* pptLP, QPoint* pptDP)
{

    QPoint pt[3];

    pt[0].setX( (int)floor(cxLogSize * (fX-m_propOrigin)/**zoom*//PARAM(XSIZE) + sgnX*0.5F)+cxLogZ );
    pt[0].setY( (int)ceil (cyLogSize * (fY- PARAM(YMIN))/m_ySize /*PARAM(YSIZE)*/ + sgnY*0.5F)+cyLogZ );
    pt[1].setX( cxLogMax );
    pt[1].setY( cyLogMax );
    pt[2].setX( cxLogZ );
    pt[2].setY( cyLogZ );
    //pdc->LPtoDP((LPPOINT)&pt, 3);
    if (bConstrain)
    {
        if (pt[0].x() <= pt[2].x())			//stay right of left border
            pt[0].setX( pt[2].x() + 1 );
        else if (pt[0].x() >= pt[1].x() )	//stay left of right border
            pt[0].setX( pt[1].x() - 1 );
        if (pt[0].y() <= pt[1].y() )			//stay below top border
            pt[0].setY( pt[1].y() + 1 );
        else if (pt[0].y() >= pt[2].y() )	//stay above bottom border
            pt[0].setY( pt[2].y() - 1 );
    }
    if (pptDP)	//Want copies of device coordinates?
    {
        pptDP->setX( pt[0].x() );
        pptDP->setY( pt[0].y() );
    }
    if (!(pptLP || pfXRnd || pfYRnd)) return;
    if (pptLP)	//Want copies of logical coordinates?
    {
        pptLP->setX( pt[0].x() );
        pptLP->setY( pt[0].y() );
    }
    if (!(pfXRnd || pfYRnd)) return;
    //Wants copies of exact float coordinates.
    if (pfXRnd) *pfXRnd = /*(*/(PARAM(XSIZE)/*/zoom)*/*(pt[0].x()-cxLogZ))/cxLogSize + m_propOrigin;
    if (pfYRnd) *pfYRnd = ( /*PARAM(YSIZE)*/ m_ySize*(pt[0].y()-cyLogZ))/cyLogSize + PARAM(YMIN);


}

//---------------------------------------------------------------------------
// We do not change device scales and mapping mode.  This is because we are
// called both to output to a window and to a physical area on a printed
// page.  Hence, the physical origin is not constant and we do our own
// mapping.  Besides, changing the DC really screws up print preview.
//
//    logical (0,cyLogMax)
//                  ^
//                  |+
//                  |        +
//                  -----------------> logical (cxLogMax,0)
//  logical (cxLogZ,cyLogZ)
//---------------------------------------------------------------------------
void GasGraph::SetMapping( QPainter &/*pdc*/)
{
    QPoint pt1, pt2;
    int cnX, cnY;

    // Save physical parameters in global space

    cxPhyZ    = m_propXPhyZero;    cyPhyZ    = m_propYPhyZero;
    cxPhySize = m_propXPhySize;    cyPhySize = m_propYPhySize;

    // Compute (fXmin,fYmin) in device coordinates
    pt1.setX( (int)(((long)cxPhySize*(m_propLMarginPct)     - 50)/100) + cxPhyZ );
    pt1.setY( (int)(((long)cyPhySize*(100-m_propBMarginPct) + 50)/100) + cyPhyZ );

    // Use 1/4 of the margin for a short tick

    m_cyTickLen = (short)((pt1.x() - cxPhyZ)/6);
    m_cxTickLen = (short)((cyPhySize+cyPhyZ - pt1.y())/4);

    // Use user% of the area for the graph.

    pt2 = pt1;
    pt2.setX((int)(((long)cxPhySize*(m_propLMarginPct)     - 50)/100) + cxPhyZ );
    cxLogZ = pt1.x(); cyLogZ = pt1.y();
    cnX = 100 - m_propLMarginPct - m_propRMarginPct;
    cnY = 100 - m_propTMarginPct - m_propBMarginPct;
    pt2.setX( pt2.x() +(int)(((long)cxPhySize*cnX + 50)/100) );
    pt2.setY( pt2.y() - (int)(((long)cyPhySize*cnY + 50)/100) );

    cxLogMax = pt2.x(); cyLogMax = pt2.y();
    cxLogSize = cxLogMax - cxLogZ;
    cyLogSize = cyLogMax - cyLogZ;
    sgnX = (cxLogSize<0?-1:1);
    sgnY = (cyLogSize<0?-1:1);
}

//---------------------------------------------------------------------------
// Routines to label the X Axis
//---------------------------------------------------------------------------

//Helper function to convert a number to a label

void GasGraph::MakeLabel(short iLabelType, QString& buf, float f, float range, bool round)
{
    int hh, mm, ss;
    int prec;

    switch (iLabelType)
    {
        case 1:
            hh = (int)(f/60.0F); mm = (int)(f-hh*60.0F); ss = (int)((f-(hh*60.0F+mm))*60.0F);
            if( round || ss == 0 )
                buf = QString("%1:%2").arg( hh, 2).arg( mm, 2, 10, QLatin1Char('0') );
            else
                buf = QString("%1:%2:%3").arg( hh, 2).arg( mm, 2, 10, QLatin1Char('0') ).arg( ss, 2, 10, QLatin1Char('0') );
            break;
        default:
            prec = range < 5.0F ? ( round ? 1 : 2 ) : range < 50.0F ? ( round ? 0 : 1 ) : 0;
            buf = QString("%1").arg(f, 0, 'f', prec);
            while( prec-- && buf.endsWith('0') )
                buf.chop(1);
            if ( buf.endsWith('.') )
                buf.chop(1);
    }
}

void GasGraph::DrawYAxis(QPainter & pdc)
{
        float fYLE = PARAM(YLABEL_EVERY); // how many minutes to label once
        if(getType() == GRAPHTYPE_IARM)
            fYLE  = floor(fYLE/*/ zoom*/);


                                          // fYTE: how many minutes one tick stands for
        float f,fYTE,f1;
        int i;
        Qt::BGMode nOldMode;

    if ((fYTE=PARAM(YTICK_EVERY)) != 0.0F)
    {

        float fBottom = PARAM(YMIN)  ;
        float fEnd = fBottom + m_ySize;//PARAM(YSIZE);///zoom  ;


        QPoint pt1;
        pt1.setX( cxLogZ );
        nOldMode = pdc.backgroundMode();
        pdc.setBackgroundMode( Qt::TransparentMode );

        for (f  = (float)ceil(fBottom/fYTE) * fYTE; f <= fEnd; f += fYTE) {

            pt1.setY( (int)(  cyLogSize * (f-fBottom)  /  /*PARAM(YSIZE)*/ m_ySize )+cyLogZ );

            QPoint pt2 = pt1;
            f1 = (float)fabs(fmod(f,fYLE));
            i = (f1<fYTE/2||fYLE-f1<fYTE/2 ? 2:1);
            pt2 -= QPoint(  m_cyTickLen, 0 );

            pdc.drawLine(pt1, pt2);
            if ( i == 2 ) {
                QString buf;

                MakeLabel(m_propYLabelType, buf, f, m_ySize);

                QFontMetrics fm( pdc.font() );

                QRect rc = fm.boundingRect(buf);
                int w = rc.width() + 1;
                int h = rc.height() + 1;

                pdc.drawText( pt2.x() - sgnX*w, pt2.y() + sgnY*h/2, w, h, Qt::AlignTop|Qt::AlignRight, buf );
            }
        }
        pdc.setBackgroundMode(nOldMode);
    }

}
void GasGraph::DrawXAxis( QPainter& pdc)
{
    QPoint pt1, pt2;
    float f, f1, fEnd, fXTE, fXLE;
    int i, j, w, h, cxMax, cxMin;
    QString buf;
    QRect brc;

    // Just return if no labels

    if ((fXTE=PARAM(XTICK_EVERY))==0.0F) return;   // minutes one tick stands for

    // Initialize the label margins.

    cxMin = cxLogZ;  // the left bound of the graph
    cxMax = cxLogMax; // the right bound

    // Loop drawing short and labeled long ticks
    Qt::BGMode nOldMode = pdc.backgroundMode();
    pdc.setBackgroundMode( Qt::TransparentMode );

    fEnd = m_propOrigin + PARAM(XSIZE);// /zoom; // the last time in the graph


    fXLE = PARAM(XLABEL_EVERY);// /zoom; // the time between two labeled long ticks;
    fXLE = floor(fXLE/*/zoom*/);
    if(fXLE ==0)
        fXLE = 1;

    //m_propOrigin the beginning time in the graph
    for (f  = (float)ceil(m_propOrigin/fXTE) * fXTE; f <= fEnd; f += fXTE) {
        FPtoDP(pdc, false, f, 0.0F, NULL, NULL, &pt1, &pt2);
        pt1.setY( (int)ceil (  sgnY*0.5F)+cyLogZ);
        pt2.setY((int)ceil (  sgnY*0.5F)+cyLogZ);
        f1 = (float)fabs(fmod(f,fXLE));
        i = (f1<fXTE/2||fXLE-f1<fXTE/2 ? 2:1);	//1->tick,
        pt2 += QPoint( 0, i*m_cxTickLen );					//2->tick/label

        pdc.drawLine( pt1, pt2 );	// Draw the tick
        if ( i == 2 )				// ...then label if necessary
        {
            MakeLabel(m_propXLabelType, buf, f, PARAM(XSIZE));

            QFontMetrics fm( pdc.font() );

            brc = fm.boundingRect(buf);
            w = brc.width() + 1;
            h = brc.height() + 1;
            i = w*sgnX;
            j = pt2.x()  - i/2;
            int off = 5;

            pdc.drawText( j-off, pt2.y() , w, h, Qt::AlignTop, buf );
            if (j*sgnX < cxMin*sgnX)   cxMin = j;	// remember left- and right-most
            if ((j+i)*sgnX > cxMax*sgnX) cxMax = j+i;
        }
    }
    pdc.setBackgroundMode(nOldMode);

    // save the outer bounds so we can scroll

    m_cxLabelL = cxMin;
    m_cxLabelR = cxMax;
}

void GasGraph::DrawMAC( QPainter & pdc, const QRect & prc )
{
    float	fMax;
    QPoint	pt;
    QRect   rc;
    int		nHeight;
    // If MAC is off the chart, don't draw

    fMax = PARAM(YMIN) + m_ySize;// PARAM(YSIZE);


    if (m_propMac <= PARAM(YMIN) || m_propMac >= fMax)
        return;

    // Find the device Y coordinate for MAC

    FPtoDP(pdc, true, 0.0F, m_propMac, NULL, NULL, NULL, &pt);

    // Find the device Y coordinates for top and bottom of graph paper

    rc.setTop( cyLogMax + 1 ); rc.setBottom( cyLogZ );

    // Form a rectangle one above, one below but stay on the paper
    if (rc.bottom() - rc.top() > 300)
    {//printer pattern
        nHeight = 3;
    }
    else if (rc.bottom() - rc.top() > 150)
    {//printer pattern
        nHeight = 2;
    }
    else
    {//screen pattern
        nHeight = 1;
    }

    if ( pt.y()-nHeight/2 > rc.top() ) rc.setTop( pt.y() - nHeight/2 );
    if (rc.top() > rc.bottom()-nHeight) rc.setTop( rc.bottom() - nHeight );
    rc.setBottom( rc.top() + nHeight );
    rc.setLeft( prc.left() ); rc.setRight( prc.right() );

    // Create a pattern to dither the MAC line and draw it in
    {
        Qt::BGMode nOldMode = pdc.backgroundMode();
        pdc.setBackgroundMode( Qt::TransparentMode );
        QPen ppenOld = pdc.pen();
        QPen p;
        p.setColor( QColor(0,0,0) ); p.setWidth( rc.height() ); p.setStyle( Qt::DotLine );
        pdc.setPen( p );
        QPainterPath path;
        path.moveTo(rc.left(), pt.y());
        path.lineTo(rc.right(), pt.y());
        pdc.drawPath( path );
        pdc.setPen( ppenOld );
        pdc.setBackgroundMode( nOldMode );
    }
}
// Helper function to repaint and return true iff we can't shift

bool GasGraph::PaintIfNecessary(float fLeft)
{

    float fChange;

    fChange = fLeft - m_propOrigin;
    if (fabs(fChange) >= (PARAM(XSIZE)/*/zoom*/))	// Change too big to scroll?
    {
        m_propOrigin = fLeft;
        //update();
        return true;
    }
    return false;
}

// Helper function to compute the proper lefthand domain value
float GasGraph::GetLeft(float fXVal)
{
    float fLeft;

    if (!m_propScrollEnb)
        fLeft = m_propOrigin;
    else
        fLeft = fXVal - PARAM(XSIZE);// /zoom;
    if (fLeft < PARAM(XMIN)) fLeft = PARAM(XMIN);
    return fLeft;
}

QPaintDevice *GasGraph::setPaintDevice(QPaintDevice *device) {
    QPaintDevice *dev = paintDevice;
    paintDevice = device;
    return dev;
}

void GasGraph::renderToImage(QImage* image)
{
    QPaintDevice *pDevice = setPaintDevice(image);
    forcePaint();
    setPaintDevice(pDevice);
}

void GasGraph::paintEvent(QPaintEvent * /*event*/)
{
    // float     fBottom, f, f1,fEnd, fYTE, fYLE;
    float f;
    int i;
    QRect clipRec;
//	Qt::BGMode nOldMode;
    Q_ASSERT( paintDevice );
    QPainter pdc( paintDevice );
    QFont logFont;
    logFont.setPixelSize( m_propFontHeight );
    logFont.setFamily( "Times" );
    pdc.setFont( logFont );

//	if(type ==1)
//		m_ySize = PARAM(YSIZE);// /zoom;
//	else
        m_ySize = PARAM(YSIZE);
    if ( paintDevice == this )
        SetPhysLoc( pdc, this->rect() );

    SetMapping(pdc);

    m_bHaveLast = false;			// No valid endpoints

    f = m_propXVal+PARAM(XSIZE)*2;
    for (i=0; i<MAXFN; i++)			// ...nor line labels
        m_fpLineLabelLast[i] = f;
    m_fpLowX = m_fpHighX = 0.0F;	// ...nor visible graph
    m_iBrushOrg=0;




    //Erase the background
    QRect rc( m_propXPhyZero, m_propYPhyZero, m_propXPhyZero+m_propXPhySize, m_propYPhyZero+m_propYPhySize);
    pdc.fillRect( rc, pdc.background() );

    QPen ppenOld = pdc.pen();
    pdc.setPen( Qt::black );

    //Color the graphing area as requested
    pdc.setBrush( m_propPaperColor );
    rc.setRight( cxLogMax - 1 ); rc.setBottom( cyLogZ - 1 );
    rc.setLeft( cxLogZ ); rc.setTop( cyLogMax );
    QBrush pbrOld = pdc.brush();
    pdc.drawRect( rc );
    pdc.setBrush(pbrOld);
    rc.adjust( 1, 1, -1, -1 );
    DrawMAC(pdc, rc);
    // Compute leftmost X value

    m_propOrigin = GetLeft(m_propXVal);

    // Draw the horizontal scale

        DrawXAxis(pdc);

    // Draw the vertical scale

        DrawYAxis(pdc);




     if(line){
        drawLine(pdc);
     }

     pdc.setRenderHint( QPainter::Antialiasing );
     pdc.setRenderHint( QPainter::TextAntialiasing );

     pdc.setPen(ppenOld);			// Restore old pen
     MoveGraphTo(pdc, m_propXVal);	// Now draw lines

}
void GasGraph::drawLine(QPainter & pdc)
{
    int x = m_point.x();

    if(x < cxLogZ){
        x = cxLogZ ;// +1;

    }
    if(x> cxLogMax){
        x = cxLogMax;//-1;

    }

    int y = m_point.y();
    if(y< cyLogMax ){
        y = cyLogMax;//+1;
    }
    if(y> cyLogZ){
        y = cyLogZ;// -1;

    }

    QPen pen = pdc.pen();
    QPen pen2;

    pen2.setColor(Qt::blue);
    pen2.setWidth(0);
    pdc.setPen(pen2);

    pdc.setOpacity(0.5f);
    pdc.drawLine(x ,cyLogMax+1, x,cyLogZ);
    pdc.drawLine(cxLogZ, y, cxLogMax, y);
    pdc.setOpacity(1.0);

    float nowX =  (float) (x-cxLogZ) * ( PARAM(XSIZE) /* / zoom */ ) / (cxLogMax-cxLogZ ) + m_propOrigin;
    float nowY = (float) (cyLogZ-y) * ( m_ySize) / (cyLogZ-cyLogMax ) + PARAM(YMIN);

    if(nowX < 0)
        nowX = 0;
    if(nowY < 0)
        nowY = 0;

    QString buf, buf2;
    QFontMetrics fm( pdc.font() );
    int labWidth, labHeight, labPos;

    MakeLabel(m_propXLabelType, buf, nowX, PARAM(XSIZE), false);
    QRect bounds = fm.boundingRect(buf);
    labWidth = bounds.width() + 2; labHeight = bounds.height() + 2;
    labPos = qBound( cxLogZ, x - labWidth/2, cxLogMax - labWidth);
    QRect rect(labPos, cyLogZ+1, labWidth, labHeight);

    MakeLabel(m_propYLabelType, buf2, nowY, m_ySize, false);
    bounds = fm.boundingRect(buf2);
    labWidth = bounds.width() + 2; labHeight = bounds.height() + 2;
    labPos = qBound( cyLogMax, y - labHeight/2, cyLogZ - labHeight);
    QRect rect2(cxLogZ - labWidth - 1, labPos, labWidth, labHeight);

    pdc.setPen(Qt::red);

    pdc.drawRect(rect);
    pdc.drawRect(rect2);

    pdc.setPen(Qt::black);

    pdc.drawText(rect, Qt::AlignHCenter|Qt::AlignTop,buf);
    pdc.drawText(rect2, Qt::AlignRight|Qt::AlignVCenter, buf2);
    pdc.setPen(pen);
}

void GasGraph::MoveGraphTo( QPainter & pdc, float fXVal)
{
    QPoint prev[ MAXFN ];
    QRect rc, rcFill;
    QPoint ptLP;
    QBrush brFill;
    QPen pen;
    QPen hpen;
    bool bPenUp=true, bStatus=false, bVisible;
    float f, f2, fLeft, fChange, fXL, fXR, fXX, fX, fY, fXParam, fYParam, fSign=0.0F, fIncr, fPrevXParam;
    float fFrom[2], fTo[2];
    int i, nDomain, nDomains, nVal, nEndPoints;//, iState;

    // Compute graph shift

    fLeft = GetLeft(fXVal);
    if (PaintIfNecessary(fLeft))
        return;

    //ptLP.x gets old logical coordinates of new origin

    FPtoDP(pdc, false, fLeft, PARAM(YMIN), &fX, NULL, &ptLP, NULL);
    if ( cxLogZ!=ptLP.x() )
    {// shift graph
        fChange = fX - m_propOrigin;
        m_propOrigin = fLeft = fX;
    }
    else
    //no shift necessary
        fChange = 0.0F;

    // compute graph area, excluding left & top border
    // rc <- logical left/top,right/bottom
    // fXL <- first logical point on left
    // fXR <- first logical point not on right
    // fXX <- logical fXVal

    fX = fLeft + PARAM(XSIZE);// /zoom;
    fY = PARAM(YMIN) + m_ySize;//  PARAM(YSIZE);
    QPoint p1,p2;
    FPtoDP(pdc, true, fLeft, fY, &fXL, NULL, &p1, NULL);
    FPtoDP(pdc, false, fX, PARAM(YMIN), &fXR, NULL, &p2, NULL);
    FPtoDP(pdc, true, fXVal, fY, &fXX, NULL, NULL, NULL);
    rc = QRect( p1,p2 );

    // scroll if we have to.
    if (cxLogZ!=ptLP.x()) {// shift graph
        i = rcFill.width();	//Device units scrolled
        if (fChange < 0.0) i = 0 - i;	//...in vector form
        m_iBrushOrg = (m_iBrushOrg-i+800)%8;
        // say remaining graph (if any) is shorter
        if (m_fpLowX < m_fpHighX)
        {
            if (m_fpLowX  < fXL) m_fpLowX  = fXL;
            if (m_fpHighX > fXR) m_fpHighX = fXR;
        }
        // fill exposed area
        //pdc->DPtoLP(&rcFill);
        pdc.fillRect( rcFill, QBrush(m_propPaperColor));
        DrawMAC(pdc, rcFill);
    }

    if (!m_propVals) return;	// Return if nothing to draw
    // Figure out domain segments: fFrom[0] to fTo[0], fFrom[1] to fTo[1],...

    nDomains = 0;
    if (fChange >= 0.0F || m_fpLowX >= m_fpHighX)
    {// what's left is on the left, or nothing left
        fSign = 1.0F;
        if (m_fpLowX < m_fpHighX)
        {// there is something left after shifting
            if (m_fpHighX <= fXX-m_propXValEvery)
            {// there is a gap to the right
                fFrom[nDomains]	 = m_fpHighX;
                fTo[nDomains]	 = fXX;
                nDomains		+= 1;
            }
        }
        else if (fXL <= fXX-m_propXValEvery)
        {// there's nothing left, but there's something to draw
            m_fpHighX	= m_fpLowX = fFrom[0] = fXL;
            fTo[0]		= fXX;
            nDomains	+= 1;
        }
    }
    else if (fXL <= m_fpLowX-m_propXValEvery)
    {// what's left is on the right; draw right to left
        fSign = -1.0F;
        fFrom[nDomains]	 = m_fpLowX;
        fTo[nDomains]	 = fXL;
        nDomains		+= 1;
    }

    emit beginDrawLines();

    if (!nDomains) {
        emit endDrawLines();
        return;		// The graph is already there
    }

    // Now put up the values (draw the graph)
    pdc.setClipRegion( rc );
    nEndPoints = 0;
    fIncr = fSign * m_propXValEvery;



    for (nVal=0; nVal<m_propVals; nVal++) {// for each dataset.................................

        if (1<<nVal & m_propMask)
            continue;	// Ignore datasets as requested
        hpen = pdc.pen();

        pen.setColor( m_propColor[nVal] );
        pen.setWidth( m_propLineWidth );
        pdc.setPen( pen );

        for (nDomain = 0; nDomain < nDomains; nDomain++)
        {//for all domains...
            for (bPenUp = true,
                 fX=fFrom[nDomain];
                 fSign*(fX-fTo[nDomain]-fIncr/2.0F)<0;
                 fX+=fIncr
                )
            {// for all x in the domain
                FPtoDP(pdc, true, fX, 0.0F, &fXParam, NULL, NULL, NULL);
                if (fXParam > fXVal) fXParam = fXVal;
                if (fXParam < fLeft) fXParam = fLeft;
                goto PLOTPOINT;
PLOTPOINTRTN:	              ;
                if (bVisible)
                {
                    if (fXParam < m_fpLowX)  m_fpLowX  = fXParam;
                    if (fXParam > m_fpHighX) m_fpHighX = fXParam;
                }
            }//end for each x value in domain
        }//end for each domain in this dataset
        if (!(bPenUp || bStatus))
        {//if we have an endpoint for this dataset (visible or not)
            nEndPoints++;
            m_fpYValOld[nVal] = fYParam;	//Remember endpoint
            if (m_propVals == 1)
                m_fpYValOld[nVal+1] = fXX;
        }
        pdc.setPen(hpen);
    }//end for all datasets

    if (nEndPoints == m_propVals)	// iff we have all endpoints...
    {
        m_bHaveLast = true;
        m_fpXValLast = fXParam;
    }
    else 							// that's not ALL that's wrong...
        m_bHaveLast = false;

    emit endDrawLines();

    return;

PLOTPOINT:;
/**///remember X; changing it signifies:
/**///			(-1.0) -> discontinuity
/**///			(-2.0) -> marker
/**/fXX = fXParam;
/**/if (bPenUp) fPrevXParam = fXParam;
/**/bStatus = false;	// No error
/**/if (m_bHaveLast && m_fpXValLast==fXParam)
/**/{
/**/	fYParam = m_fpYValOld[nVal];	// Already got f(fX)
/**/	if (m_propVals == 1)
/**/		fXX = m_fpYValOld[nVal+1];
/**/}
/**/else
/**/{
/**/	float fparams[3]
/**/		= {fXParam, (float)nVal, fPrevXParam};	// Ask for f(fXParam)
        Q_ASSERT( receiver );
        GasGraphEvent evnt( this, (float *)fparams );
        qApp->sendEvent( receiver, &evnt );
/**/	fXParam = fparams[0]; fYParam = fparams[1]; fPrevXParam = fparams[2];
/**/	SetMapping(pdc);
/**/	if (fXParam != -1.0F)
/**/	{// Call went OK and we were given a result
/**/		if (fYParam < PARAM(YMIN))
/**/			fYParam = PARAM(YMIN);
/**/		else
/**/		{
/**/			fY = PARAM(YMIN) + m_ySize;// PARAM(YSIZE);
/**/			if (fYParam > fY) fYParam = fY;
/**/		}
/**/	}
/**/	f = fXParam; fXParam = fXX; fXX = f;	// put original x back
/**/}
/**/if (fXX != -1.0F)
/**/{// Call went OK and we were given a result
/**/	fPrevXParam = fXParam;
/**/	FPtoDP(pdc, true, fXParam, fYParam, NULL, &fYParam, &ptLP, NULL);
/**/	bVisible = pdc.clipRegion().contains(ptLP);
/**/	if (bPenUp)
/**/	{
/**/		prev[ nVal ] = ptLP;
/**/		bPenUp = false;
/**/	} 	else {
    //		if( ( prev[nVal].y()<cyLogZ-1 && ptLP.y()<cyLogZ-1 )||( prev[nVal].y()>cyLogMax+1 && ptLP.y()>cyLogMax+1 ) ){

        //	if( (prev[nVal].y() != (cyLogZ-1) && prev[nVal].y()!= (cyLogMax +1))|| (   ptLP.y() != (cyLogZ-1)&& ptLP.y()!= (cyLogMax+1)) )
/**/			pdc.drawLine( prev[ nVal ], ptLP );
    //		}
                prev[ nVal ] = ptLP;
        }
/**/
/**/	f = PARAM(XSIZE)/1.5F;
/**/	f2 = fXParam - nVal*PARAM(XSIZE)/20.F;
/**/	if (//If line labelling is on
/**/           m_propLineLabelType && m_propVals > 1
/**/	    //and we're black & white if labelling automatically
/**/	    && (m_propLineLabelType!=2 || pdc.device()->colorCount()<3)
/**/	    //and we're within fIncr of the spot for this nVal
/**/	    && f2-floor(f2/f)*f <= m_propXValEvery*1.2F
/**/	    //and one third away from our last line label
/**/	    && fabs(fXParam - m_fpLineLabelLast[nVal]) > f/2.0F
/**/	   )
/**/	{
/**/		QFontMetrics tm( pdc.font() );
/**/		int i;
/**/		QRect rect = tm.boundingRect( QChar( m_propLabelChar[nVal] ) );
/**/		QSize xyzzy = rect.size();
/**/		i = xyzzy.width()*sgnX*(int)fSign;
/**/		Qt::BGMode nOldMode = pdc.backgroundMode();
/**/		pdc.setBackgroundMode( Qt::TransparentMode );

//			if( ptLP.y() != (cyLogZ-1) && ptLP.y()!= (cyLogMax+1) )
/**/			pdc.drawText( ptLP.x() - i, ptLP.y()+sgnY*xyzzy.height(), QChar( m_propLabelChar[nVal] ) );


/**/		pdc.setBackgroundMode( nOldMode );
/**/		m_fpLineLabelLast[nVal] = fXParam;
/**/	}
/**/	if (//If we should place a marker
/**/	       fXX == -2.0F && m_propVals == 1
/**/	    //and we are one thirtieth from our last marker
/**/	    && ((f2 = (float)fabs(fXParam - m_fpLineLabelLast[nVal])) > f/20.0F || f2 == 0.0F)
/**/	   )
/**/	{
/**/		QFontMetrics tm( pdc.font() );
/**/		int i;
/**/		QSize xyzzy = tm.boundingRect("^").size();
/**/		i = xyzzy.width()*sgnX*(int)fSign;
/**/		Qt::BGMode nOldMode = pdc.backgroundMode();
/**/		pdc.setBackgroundMode( Qt::TransparentMode );
/**/		pdc.drawText( ptLP.x() - i/2, cyLogZ+sgnY*xyzzy.height()/2, QString("^"));
/**/		pdc.setBackgroundMode( nOldMode );
/**/	}
/**/}
/**/else
/**/{
/**/	bVisible = false;
/**/	bPenUp = true;
/**/}
    goto PLOTPOINTRTN;
}

// Calculate the minimum size
QSize GasGraph::minimumSizeHint () const
{
    // Bottom horizontal and left vertical labels must not be truncated
    // We know percent of space for these labels ( m_propBMarginPct,m_propLMarginPct)
    // The Graph size is calculated from the font size of the labels and this percent in common size.
    QPainter pdc(paintDevice);
    QFontMetrics fm( pdc.font() );
    int h = int( ( ( fm.height()* 1.5 ) * 100 ) / m_propBMarginPct );
    int b = int( ( ( fm.height()* 1.5 ) * 100 ) / m_propLMarginPct );
    return QSize( b, h );
}

void GasGraph::mouseReleaseEvent ( QMouseEvent * event )
{
    if(event->button()== Qt::RightButton  && getType()!=GRAPHTYPE_OTHER  ){
        //zoom =zoom %3 +1;
        onZoom();
    }

    if(event->button()== Qt::LeftButton && getType()!=GRAPHTYPE_OTHER){
        line = !line;
        m_point = event->pos();
        update();
    }

}
void GasGraph::onZoom()
{
//	update();
//	emit zoomChanged();
}
void GasGraph::mouseMoveEvent(QMouseEvent * event)
{

    if(getType()!=GRAPHTYPE_OTHER && line){
        m_point = event->pos();
        update();
    }
}

