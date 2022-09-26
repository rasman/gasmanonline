#ifndef GASMCHN_H
#define GASMCHN_H

#include <QWidget>
#if QT_VERSION >= 0x050700
#	include <QtUiPlugin/QDesignerExportWidget>
#else
#	include <QtDesigner/QDesignerExportWidget>
#endif

//QDESIGNER_WIDGET_EXPORT
class GasMchn : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( QColor GasColorCKT READ GetGasColorCKT WRITE SetGasColorCKT )
    Q_PROPERTY( QColor GasColorALV READ GetGasColorALV WRITE SetGasColorALV )
    Q_PROPERTY( QColor GasColorART READ GetGasColorART WRITE SetGasColorART )
    Q_PROPERTY( QColor GasColorVRG READ GetGasColorVRG WRITE SetGasColorVRG )
    Q_PROPERTY( QColor GasColorMUS READ GetGasColorMUS WRITE SetGasColorMUS )
    Q_PROPERTY( QColor GasColorFAT READ GetGasColorFAT WRITE SetGasColorFAT )
    Q_PROPERTY( QColor GasColorVEN READ GetGasColorVEN WRITE SetGasColorVEN )

    // Text of labels under tubes
    Q_PROPERTY( QString GasLabelCKT READ GetGasLabelCKT WRITE SetGasLabelCKT )
    Q_PROPERTY( QString GasLabelALV READ GetGasLabelALV WRITE SetGasLabelALV )
    Q_PROPERTY( QString GasLabelART READ GetGasLabelART WRITE SetGasLabelART )
    Q_PROPERTY( QString GasLabelVRG READ GetGasLabelVRG WRITE SetGasLabelVRG )
    Q_PROPERTY( QString GasLabelMUS READ GetGasLabelMUS WRITE SetGasLabelMUS )
    Q_PROPERTY( QString GasLabelFAT READ GetGasLabelFAT WRITE SetGasLabelFAT )
    Q_PROPERTY( QString GasLabelVEN READ GetGasLabelVEN WRITE SetGasLabelVEN )

    // Values of labels under tubes
    Q_PROPERTY( QString GasValueCKT READ GetGasValueCKT WRITE SetGasValueCKT )
    Q_PROPERTY( QString GasValueALV READ GetGasValueALV WRITE SetGasValueALV )
    Q_PROPERTY( QString GasValueART READ GetGasValueART WRITE SetGasValueART )
    Q_PROPERTY( QString GasValueVRG READ GetGasValueVRG WRITE SetGasValueVRG )
    Q_PROPERTY( QString GasValueMUS READ GetGasValueMUS WRITE SetGasValueMUS )
    Q_PROPERTY( QString GasValueFAT READ GetGasValueFAT WRITE SetGasValueFAT )
    Q_PROPERTY( QString GasValueVEN READ GetGasValueVEN WRITE SetGasValueVEN )

    Q_PROPERTY( int CKT READ GetCKT WRITE SetCKT )
    Q_PROPERTY( int ALV READ GetALV WRITE SetALV )
    Q_PROPERTY( int ART READ GetART WRITE SetART )
    Q_PROPERTY( int VRG READ GetVRG WRITE SetVRG )
    Q_PROPERTY( int MUS READ GetMUS WRITE SetMUS )
    Q_PROPERTY( int FAT READ GetFAT WRITE SetFAT )
    Q_PROPERTY( int VEN READ GetVEN WRITE SetVEN )
    Q_PROPERTY( int MAC READ GetMAC WRITE SetMAC )
signals:
    void Resized();
public:
    GasMchn(QWidget *parent = 0);
    // Attributes
public:
    short GetCKT() const;
    void SetCKT(short);
    short GetALV() const;
    void SetALV(short);
    short GetART() const;
    void SetART(short);
    short GetVRG() const;
    void SetVRG(short);
    short GetMUS() const;
    void SetMUS(short);
    short GetFAT() const;
    void SetFAT(short);
    short GetVEN() const;
    void SetVEN(short);

    QColor GetGasColorCKT() const;
    void SetGasColorCKT( const QColor & );
    QColor GetGasColorALV() const;
    void SetGasColorALV( const QColor & );
    QColor GetGasColorART() const;
    void SetGasColorART( const QColor & );
    QColor GetGasColorVRG() const;
    void SetGasColorVRG( const QColor & );
    QColor GetGasColorMUS() const;
    void SetGasColorMUS( const QColor & );
    QColor GetGasColorFAT() const;
    void SetGasColorFAT( const QColor & );
    QColor GetGasColorVEN() const;
    void SetGasColorVEN( const QColor & );

    QString GetGasLabelCKT() const;
    void SetGasLabelCKT( const QString & );
    QString GetGasLabelALV() const;
    void SetGasLabelALV( const QString & );
    QString GetGasLabelART() const;
    void SetGasLabelART( const QString & );
    QString GetGasLabelVRG() const;
    void SetGasLabelVRG( const QString & );
    QString GetGasLabelMUS() const;
    void SetGasLabelMUS( const QString & );
    QString GetGasLabelFAT() const;
    void SetGasLabelFAT( const QString & );
    QString GetGasLabelVEN() const;
    void SetGasLabelVEN( const QString & );

    QString GetGasValueCKT() const;
    void SetGasValueCKT( const QString & );
    QString GetGasValueALV() const;
    void SetGasValueALV( const QString & );
    QString GetGasValueART() const;
    void SetGasValueART( const QString & );
    QString GetGasValueVRG() const;
    void SetGasValueVRG( const QString & );
    QString GetGasValueMUS() const;
    void SetGasValueMUS( const QString & );
    QString GetGasValueFAT() const;
    void SetGasValueFAT( const QString & );
    QString GetGasValueVEN() const;
    void SetGasValueVEN( const QString & );

    short GetMAC() const;
    void SetMAC(short);
    // Position for Injection( Liquid ) in global screen coordinates
    QPoint GetInjectPoint();
    // Rectangles of tubes in local coordinates
    QRect GetCKTRect() const { return m_rcCompart[CKT]; }
    QRect GetALVRect() const { return m_rcCompart[ALV]; }
    QRect GetARTRect() const { return m_rcCompart[ART]; }
    QRect GetVRGRect() const { return m_rcCompart[VRG]; }
    QRect GetMUSRect() const { return m_rcCompart[MUS]; }
    QRect GetFATRect() const { return m_rcCompart[FAT]; }
    QRect GetVENRect() const { return m_rcCompart[VEN]; }

    void hideNumerics( bool set ); // Hide/show labels and values under tubes
    QSize minimumSizeHint () const;
protected:
    void paintEvent(QPaintEvent * );
    void resizeEvent ( QResizeEvent * );

protected:
        enum SETTING_SUBSCR
    {
        CKT,
        ALV,
        ART,
        VRG,
        MUS,
        FAT,
        VEN,
        NEXT_SETTING_SUBSCR
    };

    // Storage for Get/Set properties
    short m_iSetting[NEXT_SETTING_SUBSCR];
    short m_iOldSetting[NEXT_SETTING_SUBSCR];
    short m_propMac;

    QRect m_rcCompart[NEXT_SETTING_SUBSCR];
    QColor * m_pColor[NEXT_SETTING_SUBSCR];
    static int CPOS[NEXT_SETTING_SUBSCR];
    static int CSIZ[NEXT_SETTING_SUBSCR];
    int m_iMargin;
    bool m_hideNumerics;

    QColor m_propGasColorCKT;
    QColor m_propGasColorALV;
    QColor m_propGasColorART;
    QColor m_propGasColorVRG;
    QColor m_propGasColorMUS;
    QColor m_propGasColorFAT;
    QColor m_propGasColorVEN;

    QString m_propGasLabelCKT;
    QString m_propGasLabelALV;
    QString m_propGasLabelART;
    QString m_propGasLabelVRG;
    QString m_propGasLabelMUS;
    QString m_propGasLabelFAT;
    QString m_propGasLabelVEN;

    QString m_propGasValueCKT;
    QString m_propGasValueALV;
    QString m_propGasValueART;
    QString m_propGasValueVRG;
    QString m_propGasValueMUS;
    QString m_propGasValueFAT;
    QString m_propGasValueVEN;

private:
    void RecalcArea(const QRect& rcDraw);
    QRect m_rcLast;
    void PaintGasmchn( QPainter& dc, short nCompart);
    void PaintLabels( QPainter& dc ); // Paint labels under tubes
    inline void GetMidpoints(QRect *rec, QPoint *top, QPoint *bot);

};
#endif
