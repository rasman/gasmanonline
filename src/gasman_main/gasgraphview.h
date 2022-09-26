#ifndef GASGRAPHVIEW_H
#define GASGRAPHVIEW_H

#include "ui_gasgraphview.h"
#include <QTextStream>
#include <QBuffer>
#include <QtCore/QSemaphore>
#include "gasviewcommon.h"
#include <QDebug>
 
class GasDoc;
class GasGraph;
class GasChildWindow;

class GasGraphView : public GasViewCommon, private Ui::GasGraphView
{
	Q_OBJECT
public:
	
	GasGraphView( GasDoc *doc, GasChildWindow * childWindow, QWidget *parent = 0 );

	void InitialUpdate(int nGas);				//Initializes the graph view
	
	static bool tryGetScaleString(const int scaleMinutes, QString& scaleString);
	static bool tryGetScaleMinutes(const QString& scaleString, int& scaleMinutes);
	static void ReadProfile();
	void DrawGraphs( bool needReapint = false );

	void enableScrolling( bool set );
	inline bool isEnabledScrolling() const { return m_bScrollEnb; }

	inline GasGraph *IARMGraph() const { return m_IARMGraph; }
	void SetIARMmax( int nMax );
	void SetDELmax( int nMax );
	void SetFGFmax( int nMax );
	void SetVAmax(int nMax );
	void SetCOmax( int nMax );

	QImage toImage();
	QByteArray toArray();
	bool tryGetGas(int& nGas) {nGas = m_nGas; return true;}
	void resetGas(const int nGas) {m_nGas = nGas;}
private:
	GasChildWindow * m_childwindow;
	QSemaphore m_scaleChangeSemaphore;
protected:
	virtual void customEvent( QEvent *evnt );
	void keyPressEvent(QKeyEvent * e);
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	 
	void wheelEvent( QWheelEvent * event )  ;
public slots:
	void ChangeScale( const QString &scale );
	 
	void onZoomChanged();
	void onDrawFAT(int state);
	void onDrawMUS(int state);
	void onDrawVRG(int state);
	void onDrawALV(int state);
	void onDrawCKT(int state);
	void onCollapse();
	void hideCollapse();
private slots:
	void scrollMoved( int value );
 
public:
	QString m_strAgent;
	int m_nScaleMinute;						// Length of graph in minutes
	 
	// Program defaults
	static int m_nLineLabelType;				// 0=off, 1=on, 2=auto
	static int m_nLineWidth;
	static int m_nScrollPercent;
	static bool m_bGraphpaper;
	// View Defaults
	static bool m_bDfltScrollEnb;
	static int m_nDfltScaleMinutes;
	static QString m_szScale;
	inline GasDoc * doc() { return gasdoc;}

private:
	void DisplayExtraData();
	void UpdateData( bool bSaveAndValidate = true );
	void SetGraphMax( GasGraph* pctl, int nMax );
	void DrawGraph( GasGraph* pctl, float fXVal, int nPoints, bool needReapint );
	void EnableAutoScroll( bool bHow );
	void EnableScroll();
	void DisableScroll();
public:
	void RescaleScroll();
	void SetGraphOrigin();
	void SetTempmark();
	void UnsetTempmark();
	int getZoom();
	void setZoom(int z);
private:
	void OnYValReqGraphDEL(float * X, float * Y, float * lastX);
	void OnYValReqGraphFGF(float * X, float * Y, float * lastX);
	void OnYValReqGraphIARM(float * X, float * Y, float * lastX);
	void setYScrollBar();
private:
	float m_fMAC;								// MAC from anesthetic
	QString	m_strScale;
	bool m_bScrollEnb;							// Whether user wants autoscroll
	 
	QString m_strCircuit;
	QSize m_sizeOrig;							//Size at birth
	QColor m_colorPaper;						//...the color of paper
	quint32 m_dwTempmark;						//Bookmark at the end of the graph
	GasDoc *gasdoc;								//Pointer to the main document
	bool m_bToImage;							// TRUE iff in print loop (not drawing to screen)
	bool m_bVaCo;								// Iff m_bToImage, controls type of graph output
	bool m_bColl;
	int m_nGas;									// 0=primary, 1=secondary,...
};

#endif
