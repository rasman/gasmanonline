#ifndef GASGAUGE_H
#define GASGAUGE_H


#include <QWidget>
#include <QColor>
#if QT_VERSION >= 0x050700
#	include <QtUiPlugin/QDesignerExportWidget>
#else
#	include <QtDesigner/QDesignerExportWidget>
#endif

//QDESIGNER_WIDGET_EXPORT
class GasGauge : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( int Max READ GetMax WRITE SetMax )
    Q_PROPERTY( int Value READ GetValue WRITE SetValue )
    Q_PROPERTY( int TickEvery READ GetTickEvery WRITE SetTickEvery )
    Q_PROPERTY( int LTickEvery READ GetLTickEvery WRITE SetLTickEvery )
    Q_PROPERTY(QColor BarColor READ GetBarColor WRITE SetBarColor )
public:
    GasGauge(QWidget *parent = 0);

    short GetMax();
    void SetMax(short nNewValue);
    short GetValue();
    void SetValue(short nNewValue);
    short GetTickEvery();
    void SetTickEvery(short nNewValue);
    short GetLTickEvery();
    void SetLTickEvery(short nNewValue);
    QColor GetBarColor();
    void SetBarColor(QColor nNewValue);
    bool GetStartStatus();
    void SetStartStatus(bool bStatus);
    static int mulDiv( int p1, int p2, int p3 );
signals:
    void FireGasgaugeStart();
    void FireGasgaugeChange(short Value);
    void FireGasgaugeStop();
// Implementation

protected:
        // Storage for Get/Set properties
    short m_iMax;
    short m_iValue;
    short m_iTickEvery;
    short m_iLTickEvery;
    QColor m_clrBarColor;
    bool m_bCapturing;
    bool m_bStartStatus;

protected:
    void paintEvent(QPaintEvent *event);
    void MoveGaugeTo( QPainter *pdc, QRect& rcBar, int iValue);
private:
    //CDC* m_pDC;
    int m_iOffset;
    //int DtoL(CRect& rcBar, int nY);
    int LtoD( QRect& rcBar, int iValue );
    int DtoL(QRect& rcBar, int nY);
    void RationalizeParams();
    void mousePressEvent ( QMouseEvent * event );
    void mouseReleaseEvent ( QMouseEvent * event );
    void mouseMoveEvent ( QMouseEvent * event );
};

#endif
