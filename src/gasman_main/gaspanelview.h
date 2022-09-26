
#ifndef GASPANELVIEW_H
#define GASPANELVIEW_H

#include <QWidget>
#include <QTime>
#include <QSemaphore>
#include "gasviewcommon.h"
#include "gasevent.h"

class QLineEdit;
class QIntValidator;
class GasDoc;
class GasSharedDataView;
class GasGauge;
class GasChildWindow;
class Ui_GasPanelView;
class QToolButton;

class GasPanelView : public GasViewCommon
{
	Q_OBJECT
public:
	GasPanelView( GasDoc *doc, GasChildWindow * childWindow, QWidget *parent = 0 );
	~GasPanelView();

	static void ReadProfile();

	void InitialUpdate(int nGas);		//Initializes the Panel View

	void stop();						//stop simulation
	void start();						//start simulation

	void hideNumerics( bool set );
	void showCost( bool set );
	void showMl( bool set );
	void changeAgent();

	inline bool isHiddenNumerics() const { return hiddenNumerics; }
	inline bool isShownCost() const { return m_bShowUptakeCost; }
	inline bool isShownMl() const { return m_bShowMl; }

	int highDELValue() const; 
	int highFGFValue() const;
	int highVAValue() const;
	int highCOValue() const;

	void setHighDELValue( int i );
	
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	QString m_strAgent;

	bool tryGetGas(int& nGas) {nGas = m_nGas; return true;}
	void resetGas(const int nGas) {m_nGas = nGas;}
	bool markedForDeath(){ return m_nGas < 0; }
protected:
	virtual void customEvent( QEvent *evnt );
	virtual void paintEvent( QPaintEvent *evnt );
	virtual bool eventFilter( QObject *obj, QEvent *evnt );
	virtual void resizeEvent( QResizeEvent * );

public slots:
	void run();
	void Srynge();
	void ChangeAgent( const QString &agent );
	void ChangeCircuit( const QString &circuit );
	void ChangeSpeed( const QString &speed );
	void ChangeWeight( double weight_old );
	void updateData();
	// set the speed when stoped,don't need to stop the sim.
	void SetSpeed(const QString &speed);
private slots:
	void gasmchnGeometryUpdated();
	void changed( QWidget *widget = 0, QToolButton *tb = 0 );
	void verify( const QString & );
	void maxChanged();
	void upDownValue();
	void flush();
	void gasgaugeChangeStart();
	void gasgaugeChangeGauge( short value );
	void gasgaugeChangeStop();
	void nextStep();

public:
	void StopForChange();
	void StopTicker();
	void StartTicker();

private:
	QString FormatToFMT_DEL(const float x);
	bool FetchCKT();
	bool FetchALV();
	bool FetchART();
	bool FetchVRG();
	bool FetchMUS();
	bool FetchFAT();
	bool FetchVEN();
	bool FetchTime();
	bool FetchUptake();
	bool FetchDelivered();
	bool CheckAgentChange( QString const &strAgent, const int nGas );
	bool FetchVAOut();						// NOT GetVA(), gets EFFECTIVE VA 	
	bool FetchFGFOut();						// NOT GetFGF(), gets EFFECTIVE FGF 
	void FetchData( const bool bInitialize = false );
	void SetAnesParams();
	void SetDELHIGH();
	void UpdateData( bool bSaveAndValidate = true );
	void DisplayExtraData();
	void SetGauge( GasGauge *pctl, int cMax );
	void setLeEnabled( QLineEdit *le, bool f );
	void mousePressEvent ( QMouseEvent * event );
	void LeChanged( QWidget *pWidget, GasCallbackEvent<GasPanelView> *pCallbackEvent );

	QSemaphore LeChangeSemaphore;

public:
	// View Defaults
	static int	m_nDfltHIGH_FGF;		// Defaults from INI
	static int	m_nDfltHIGH_VA;
	static int	m_nDfltHIGH_CO;
	static int	m_nDfltShowCost;
	static int	m_nDfltShowMl;
	static int	m_nDfltShowControl;

	int m_nLastAgent;							// Used to retain MAXDEL if new=last

	bool m_bShowUptakeCost;	// On if uptake display should be $
	bool m_bShowMl;			// On if uptake/delivered volume should be ml

private:
	Ui_GasPanelView * ui;
	QIntValidator *ivDEL;
	float MAX_DEL;
	QString	FMT_DEL;							//Turns out to be VARIABLE!!
	float m_fMAC;								// Read from the doc.  We draw the line here
	short m_cnMAC;								// the percent of graph version of the above
	short m_cnCKT;
	float m_fLastCKT;
	short m_cnALV;
	float m_fLastALV;
	short m_cnART;
	float m_fLastART;
	short m_cnVRG;
	float m_fLastVRG;
	short m_cnMUS;
	float m_fLastMUS;
	short m_cnFAT;
	float m_fLastFAT;
	short m_cnVEN;
	float m_fLastVEN;
	float m_fLastTime;
	float m_fLastUptake;
	float m_fLastDelivered;
	quint32 m_dwTickTimer;						// Desired virtual time if timer running
	int m_nTickSize;							// msec to advance each tick
	QString m_strSpeed;
	QString m_strCircuit;
	GasDoc *gasdoc;								//pointer to main document
	bool hiddenNumerics;
	bool m_gaugeStarted;
	quint32 m_dwTickCount;
	QTime tickCounter;
	QTimer *stepTimer;
	double koefWidthMchn;
	int m_nGas;									// 0=primary, 1=secondary,...
};

#endif
