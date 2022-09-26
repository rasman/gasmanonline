#ifndef GASGRAPH_H
#define GASGRAPH_H

#include <QWidget>
#include <QEvent>
#if QT_VERSION >= 0x050700
#	include <QtUiPlugin/QDesignerExportWidget>
#else
#	include <QtDesigner/QDesignerExportWidget>
#endif
#include <QDebug>

#define MAXFN	6	/* There are 6 functions max */

enum {
    XLABEL_EVERY,
    XMIN,
    XSIZE,
    XTICK_EVERY,
    YLABEL_EVERY,
    YMIN,
    YSIZE,
    YTICK_EVERY,
    NEXT_SETTING_SUBSCR
};

#define PARAM(idx) (m_propSettings.rgfSetting[idx])

enum aGasGraphType
{
    GRAPHTYPE_IARM,
    GRAPHTYPE_OVERLAY,
    GRAPHTYPE_OTHER
};

class GasGraph;

// Custom event - use to get data from GasOverlay
class GasGraphEvent: public QEvent
{
public:
    GasGraphEvent( GasGraph *graph, float * params ):QEvent( (QEvent::Type)(QEvent::User + 2) ), p(params), r(graph) { setAccepted( true ); }
    float * data() const { return p;}
    GasGraph *receiver() const { return r; }
private:
    float * p;
    GasGraph *r;
};

//QDESIGNER_WIDGET_EXPORT
class GasGraph : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( bool ScrollEnb READ GetScrollEnb WRITE SetScrollEnb )
    Q_PROPERTY( bool Loading READ GetLoading WRITE SetLoading )
    Q_PROPERTY( QColor Color0 READ GetColor0 WRITE SetColor0 )
    Q_PROPERTY( QColor Color1 READ GetColor1 WRITE SetColor1 )
    Q_PROPERTY( QColor Color2 READ GetColor2 WRITE SetColor2 )
    Q_PROPERTY( QColor Color3 READ GetColor3 WRITE SetColor3 )
    Q_PROPERTY( QColor Color4 READ GetColor4 WRITE SetColor4 )
    Q_PROPERTY( int LMarginPct READ GetLMarginPct WRITE SetLMarginPct )
    Q_PROPERTY( int RMarginPct READ GetRMarginPct WRITE SetRMarginPct )
    Q_PROPERTY( int TMarginPct READ GetTMarginPct WRITE SetTMarginPct )
    Q_PROPERTY( int BMarginPct READ GetBMarginPct WRITE SetBMarginPct )
    Q_PROPERTY( int XPhySize READ GetXPhySize WRITE SetXPhySize )
    Q_PROPERTY( int YPhySize READ GetYPhySize WRITE SetYPhySize )
    Q_PROPERTY( int XPhyZero READ GetXPhyZero WRITE SetXPhyZero )
    Q_PROPERTY( int YPhyZero READ GetYPhyZero WRITE SetYPhyZero )
    Q_PROPERTY( double Origin READ GetOrigin WRITE SetOrigin )
    Q_PROPERTY( double MAC READ GetMac WRITE SetMac )
    Q_PROPERTY( int Mask READ GetMask WRITE SetMask )
    Q_PROPERTY( int LabelChar0 READ GetLabelChar0 WRITE SetLabelChar0 )
    Q_PROPERTY( int LabelChar1 READ GetLabelChar1 WRITE SetLabelChar1 )
    Q_PROPERTY( int LabelChar2 READ GetLabelChar2 WRITE SetLabelChar2 )
    Q_PROPERTY( int LabelChar3 READ GetLabelChar3 WRITE SetLabelChar3 )
    Q_PROPERTY( int LabelChar4 READ GetLabelChar4 WRITE SetLabelChar4 )
    Q_PROPERTY( int LineLabelType READ GetLineLabelType WRITE SetLineLabelType )
    Q_PROPERTY( int LineWidth READ GetLineWidth WRITE SetLineWidth )
    Q_PROPERTY( int Vals READ GetVals WRITE SetVals )
    Q_PROPERTY( int XLabelType READ GetXLabelType WRITE SetXLabelType )
    Q_PROPERTY( int YLabelType READ GetYLabelType WRITE SetYLabelType )
    Q_PROPERTY( QColor PaperColor READ GetPaperColor WRITE SetPaperColor )
    Q_PROPERTY( double XMin READ GetXMin WRITE SetXMin )
    Q_PROPERTY( double XSize READ GetXSize WRITE SetXSize )
    Q_PROPERTY( double XLabelEvery READ GetXLabelEvery WRITE SetXLabelEvery )
    Q_PROPERTY( double XTickEvery READ GetXTickEvery WRITE SetXTickEvery )
    Q_PROPERTY( double XValEvery READ GetXValEvery WRITE SetXValEvery )
    Q_PROPERTY( double XVal READ GetXVal WRITE SetXVal )
    Q_PROPERTY( double YMin READ GetYMin WRITE SetYMin )
    Q_PROPERTY( double YSize READ GetYSize WRITE SetYSize )
    Q_PROPERTY( double YLabelEvery READ GetYLabelEvery WRITE SetYLabelEvery )
    Q_PROPERTY( double YTickEvery READ GetYTickEvery WRITE SetYTickEvery )
    Q_PROPERTY( int FontHeight READ GetFontHeight WRITE SetFontHeight )
public:
    GasGraph(QWidget *parent = 0);
    // Attributes
public:
    bool GetLoading();
    void SetLoading(bool);
    bool GetScrollEnb();
    void SetScrollEnb(bool);
    QColor GetColor0();
    void SetColor0( const QColor &);
    QColor GetColor1();
    void SetColor1( const QColor &);
    QColor GetColor2();
    void SetColor2( const QColor &);
    QColor GetColor3();
    void SetColor3( const QColor &);
    QColor GetColor4();
    void SetColor4( const QColor &);
    short GetLMarginPct();
    void SetLMarginPct(short);
    short GetRMarginPct();
    void SetRMarginPct(short);
    short GetTMarginPct() ;
    void SetTMarginPct(short);
    short GetBMarginPct() ;
    void SetBMarginPct(short);
    short GetXPhySize();
    void SetXPhySize(short);
    short GetYPhySize();
    void SetYPhySize(short);
    short GetXPhyZero();
    void SetXPhyZero(short);
    short GetYPhyZero();
    void SetYPhyZero(short);
    float GetOrigin();
    void SetOrigin(float);
    float GetMac();
    void SetMac(float);
    float GetXVal();
    void SetXVal(float);
    float GetXValEvery();
    void SetXValEvery(float);
    short GetMask();
    void SetMask(short);
    short GetLabelChar0();
    void SetLabelChar0(short);
    short GetLabelChar1();
    void SetLabelChar1(short);
    short GetLabelChar2();
    void SetLabelChar2(short);
    short GetLabelChar3();
    void SetLabelChar3(short);
    short GetLabelChar4();
    void SetLabelChar4(short);
    short GetLineLabelType();
    void SetLineLabelType(short);
    short GetLineWidth();
    void SetLineWidth(short);
    short GetVals();
    void SetVals(short);
    short GetXLabelType();
    void SetXLabelType(short);
    short GetYLabelType();
    void SetYLabelType(short);
    QColor GetPaperColor();
    void SetPaperColor( const QColor &);
    float GetXLabelEvery();
    void SetXLabelEvery(float);
    float GetXMin();
    void SetXMin(float);
    float GetXSize();
    void SetXSize(float);
    float GetXTickEvery();
    void SetXTickEvery(float);
    float GetYLabelEvery();
    void SetYLabelEvery(float);
    float GetYMin();
    void SetYMin(float);
    float GetYSize();
    void SetYSize(float);
    float GetYTickEvery();
    void SetYTickEvery(float);
    QColor GetBackColor();
    void SetBackColor(const QColor &);
    short GetFontHeight();
    void SetFontHeight(short);

    int zoom;
    void setZoom(int z){ zoom = z;onZoom();}
    int getZoom(){return zoom;}
    bool line;

    aGasGraphType type;
    void setType(aGasGraphType a) {
        type = a;
    }
    aGasGraphType getType(){return type;}

    inline void setReceiver( QObject * r ) { receiver = r; } // Set object to receive a GasGraphEvent
    virtual QSize minimumSizeHint () const;
    void renderToImage(QImage* image);
signals:
    void beginDrawLines();
    void endDrawLines();

private:
    bool m_propLoading;
    bool m_propScrollEnb;
    short m_propLMarginPct;
    short m_propRMarginPct;
    short m_propTMarginPct;
    short m_propBMarginPct;
    short m_propXPhySize;
    short m_propYPhySize;
    short m_propXPhyZero;
    short m_propYPhyZero;
    float m_propOrigin;
    float m_propMac;
    float m_propXVal;
    float m_propXValEvery;
    short m_propMask;
    short m_propLineLabelType;
    short m_propLineWidth;
    short m_propVals;
    short m_propXLabelType;
    short m_propYLabelType;
    QColor m_propPaperColor;
    QColor m_propBackColor;
    short m_propFontHeight;

    QColor m_propColor[MAXFN];		//pen color for all
    short m_propLabelChar[MAXFN];
    union
    {
        struct _settings
        {
            float XLabelEvery;
            float XMin;
            float XSize;
            float XTickEvery;
            float YLabelEvery;
            float YMin;
            float YSize;
            float YTickEvery;
        } sSettings;
        float rgfSetting[sizeof(struct _settings)/sizeof(float)];
    }m_propSettings;

    short m_cxLabelL, m_cxLabelR;
    short m_cxTickLen, m_cyTickLen;
    bool m_bHaveLast;
    int m_iBrushOrg;
    float m_fpHighX;
    float m_fpLowX;
    float m_fpLineLabelLast[MAXFN];
    float m_fpYValOld[MAXFN];
    float m_fpXValLast;
    float m_ySize;
    QObject * receiver; // Receiver of GasGraphEvent ( GasOverlay )
    QPaintDevice *paintDevice;
    float m_fXLabelEverySet;
    float m_fXTickEverySet;
    float m_fYLabelEverySet;
    float m_fYTickEverySet;

private:
    QPaintDevice *setPaintDevice(QPaintDevice *device);
    void forcePaint() { paintEvent(Q_NULLPTR); }
    void MakeLabel(short iLabelType, QString& buf, float f, float range, bool round=true);
    void SetPhysLoc( QPainter &pdc, const QRect & rect);
    void ReactToParam(void);
    void FPtoDP( QPainter &pdc, bool bConstrain,
                float fX, float fY,
                float *pfXRnd, float *pfYRnd,
                QPoint* pptLP, QPoint* pptDP);
    void SetMapping( QPainter & pdc);
    void RationalizeParams(void);
    void DrawMAC( QPainter & pdc, const QRect & prc );
    float GetLeft(float fXVal);
    void DrawXAxis( QPainter & pdc);
    void DrawYAxis(QPainter & pdc);
    QPoint m_point;
    QRect m_rect;
    QRect m_rect2;
private:
    bool PaintIfNecessary(float fLeft);
    void drawLine(QPainter & pdc);
private:
    void MoveGraphTo( QPainter & pdc, float fXVal);
protected:
    void mouseReleaseEvent ( QMouseEvent * event );
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent * event);

public slots:
    void scrollMoved(int x){PARAM(YMIN) = (float)x/10.0f; update();}
public:
    void onZoom();
signals:
    void zoomChanged();

};
#endif
