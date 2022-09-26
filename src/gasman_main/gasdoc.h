#ifndef GASDOC_H
#define GASDOC_H

#include <QObject>
#include <QSize>
#include <QPoint>
#include <QTime>
#include <QElapsedTimer>
#include <QTextStream>
#include <QColor>
#include "gaslicensemanager.h"
#include <QtXml/QDomDocument>
#include "gasparser.h"
#include "gas.h"
#include "gasanes.h"
#include "gasevent.h"
#include "gasglobal.h"

class GasMainWindow;
class QTimer;

class GasDoc : public QObject
{
	Q_OBJECT
public:
	GasDoc( QWidget * parent );
	~GasDoc();

	bool newDocument();

	void m_gasArrayDebug(int val);
	void rewind();
	void fastFwd();
	void zeroTimer();
	void clearAll();
	void copyData();
	void defaultAgentColors();
	void disableUptake( bool set );
	void disableReturn( bool set );
	void enableVapor( bool set );
	void changePatient();
	void setBookmark();
	void unitDose();
	void setCost();
	void adjustDel();
	bool duplicateAgents();
	void setColorToDefault(const int nGas);
	void changeColor(int nGas, const QString& color);
	void colorSwap(int first, int second);

	void Serialize( QDataStream &stream, bool storing );

	bool isCondition();

	void setBkPoint();	

    QDomDocument toXml(const QByteArray& ba, const QString& externalImageName, const QString& iconName,
		                 bool graphs, bool printout, bool wantResults);

	inline bool isEnabledVapor() const { return m_bVapEnb; }
	inline bool isDisabledReturn() const { return !m_bRtnEnb; }
	inline bool isDisabledUptake() const { return !m_bUptEnb; }

	inline QString GetDescription() const { return description; }
	inline void SetDescription(const QString& text) {
		description = text;
		updateTitle();
		emit modified();
	}
	inline GasAnesthetic *GetAgent( int nGas ) const
		{ return (GasAnesthetic*)m_anesArray.at( m_gasArray.at( nGas )->m_nAgent ); }
	inline float GetDEL( int nGas ) const
		{ return m_gasArray.at( nGas )->m_fDEL; }
	inline float GetUnitDose( int nGas ) const
		{ return m_gasArray.at( nGas )->m_fUnitDose; }
	inline float GetUptake( int nGas ) const
		{ return m_gasArray.at( nGas )->m_fResults[UPT]; }
	inline float GetDelivered( int nGas ) const
		{ return m_gasArray.at( nGas )->m_fResults[DEL]; }
	inline float GetCKT( int nGas ) const
		{ return m_gasArray.at( nGas )->m_fResults[CKT]; }
	inline float GetALV( int nGas ) const
		{ return m_gasArray.at( nGas )->m_fResults[ALV]; }
	inline float GetART( int nGas ) const
		{ return m_gasArray.at( nGas )->m_fResults[ALV]; }
	inline float GetVRG( int nGas ) const
		{ return m_gasArray.at( nGas )->m_fResults[VRG]; }
	inline float GetMUS( int nGas ) const
		{ return m_gasArray.at( nGas )->m_fResults[MUS]; }
	inline float GetFAT( int nGas ) const
		{ return m_gasArray.at( nGas )->m_fResults[FAT]; }
	inline float GetVEN( int nGas ) const
		{ return m_gasArray.at( nGas )->m_fResults[VEN]; }
	inline float GetUptakeCost( int nGas )
		{ return m_gasArray.at( nGas )->m_fResults[UPT] * GetCostRatio( nGas ); }
	inline float GetDeliveredCost( int nGas )
		{ return m_gasArray.at( nGas )->m_fResults[DEL] * GetCostRatio( nGas ); }
	inline QString GetCircuit() const { return m_strCircuit; }
	inline int GetSpeed() const { return m_nSpeed; }
	inline int GetCurrentSpeed() const { return m_theState == RUNNING_STATE && m_bSpeedup ? AFAP : m_nSpeed; }
	inline float GetTime() const { return (float)(m_dwTime/1000.0F); }
	inline float GetHighTime() const {return (float)(m_dwHighTime/1000.0F); }
	inline bool IsOver() const { return m_dwTime >= m_dwHighTime; }
	inline quint32 GetHighCalcTime() { return m_dwCalcTime; }
	inline float GetFGF() const { return m_fFGF; }
	inline float GetVA() const { return m_fVA; }
	inline float GetCO() const { return m_fCO; }
	inline float GetWeight() const { return m_fWeight; }
	inline AState GetState() const { return m_theState; }

	float GetVA( float fMin );
	float GetCO( float fMin );
	float GetFGF( float fMin, int nGas );
	float GetDEL( float fMin, int nGas, bool *pbInjected = 0, float fLast = 0.0F );
	float GetCKT( float fMin, int nGas );
	float GetALV( float fMin, int nGas );
	float GetART( float fMin, int nGas );
	float GetVRG( float fMin, int nGas );
	float GetMUS( float fMin, int nGas );
	float GetFAT( float fMin, int nGas );
	float GetVEN( float fMin, int nGas );
	float GetUptake( float fMin, int nGas );
	float GetUptakeCost( float fMin, int nGas );
	float GetDelivered( float fMin, int nGas );
	float GetDeliveredCost( float fMin, int nGas );
	
	void SetWeight( QWidget *that, const float &fWeight );
	void SetDEL( QWidget *that, int nGas, const float &DEL );
	void SetFGF( QWidget *that, const float &FGF );
	void SetVA( QWidget *that, const float &VA );
	void SetCO( QWidget *that, const float &CO );
	void SetCircuit( QWidget *that, const QString &circuit );
	void SetSpeed( QWidget *that, const short &speed );
	void SetAgent( QWidget *that, int nGas, const QString &agent );
	void AddAgent(const QString agent);
	GasChildren DeleteAgent(int gasNum);
	void Inject();
	void Flush();
	
	void ChangeState( QWidget *that, const AState theState );
	bool CheckChange( bool bTrunc = false, bool *pStat = NULL );	//Change future?
	bool CanInject( const int nGas ) const;
	void StepSimulation( long nMSec );								//Step view
	static void ReadProfile();
	bool ExtendResultSet();											// Compute more data

	void addChildWindow( GasChildWindow * window );					//Add the child window to the list of children
	void removeChildWindow( GasChildWindow * window );				//Remove the child window from the list of children
	QString currentFile() { return m_curFile; }
	QString title();												//title of the document
	void updateTitle();												//updates title bar
	
	void UpdateAllViews( QWidget *pSender = 0, int info = 0, GasUpdateEvent::AnEvent type = GasUpdateEvent::UPD_NONE );
	inline const GasChildren &gasChildren() { return  childWindows; }
	bool isModified();
	
	bool maybeSave();
	bool save();
    bool saveAs();
	bool saveFile( const QString &fileName, bool setCurrent = true );	//save the data into the file
	bool loadFile( const QString &fileName );							//load the data from the file
	void send();														//Sends a message with the document as an attachment
	bool close();
	
	float getOp(QString & op);

	//setting in panel
	float maxDEL;
	float nowDEL;

	float maxFGF;
	float nowFGF;

	float maxVA;
	float nowVA;

	float maxCO;
	float nowCO;
	//
protected:
	void Truncate();							// Discard future results/samples
	void InitDocument();						// Set default values
	void ResetState(const GasSample& samp);		// Set state from some sample
	void DeleteContents();						// Clear out document
	
#ifdef EULAR
	void ComputeTerms( float fCO, float fWeight, int nAgent,
	                  COMP_ARRAY &fEffectiveFlowTo, COMP_ARRAY &fdtOverEffectiveVolume );
	bool Calc( GasSample &samp, int nInjections, bool bFlush, RES_ARRAY& fPressure,			// Calculate one breath
	          COMP_ARRAY &fEffectiveFlowTo, COMP_ARRAY &fdtOverEffectiveVolume,
	          float fTotUptake, float fUptake, int nVernier );
	inline float CalcUptake( GasSample &samp, RES_ARRAY &fResults );
#else
	void ComputeTerms( float fFGF, float fVA, float fCO, float fWeight, int nAgent,
	                  COMP_ARRAY &fEffectiveFlowTo, COMP_ARRAY  (&fExp)[MAX_VERNIER] );
	// Calculate one breath
	bool Calc( GasSample &samp, int nInjections, bool bFlush, RES_ARRAY& fPressure,
	          COMP_ARRAY& fEffectiveFlowTo, COMP_ARRAY& fExp, quint32 dwTime,
	          float fTotUptake, float fUptake, int nVernier );
	float CalcUptake( GasSample &samp, RES_ARRAY &fResults, COMP_ARRAY& fExp, int nVernier );
#endif
	void CallViews();										// Tell views requested time's here
	int SampAt( quint32 dwTime );							// Locate the sample @ time dwTime
	RES_ARRAY &ResultsAt( quint32 dwTime, int nGas );		// Obtain results at time dwTime
	float GetCostRatio( int nGas );

private:
	void SetTimeVariables( quint16 dt );
	RES_ARRAY &ResultsFromHistoryAt( quint32 dwTime, int nGas );
	QVector<float> pctMacForTargetsAtTime(QList<int>& targets, quint32 dwTime);
	bool shouldBreakAtTime(quint32 dwTime);
	inline quint32 GetDwTime( float fMin ) {
		quint32 dwt = quint32( fMin * 60000.0F );
		Q_ASSERT( dwt <= m_dwHighTime + 100L );
		return qMin( dwt, m_dwHighTime );
	}
	void SerializeMSec_dx( QDataStream &stream, bool storing  );
	void StopOnIdle();
	void RunOnIdle();
	void MakeProgress();
	void setCurrentFile( const QString &fileName );
	void AddDefaultAgentColor(const QString &agent);
	QString getDefaultAgentColor(const QString &agent);
	QString getDefaultAgentColor(const int nGas) const;
	EventList SampComp( GasSample &s1, GasSample &s2 );
	QDomElement createDomElement(
		QDomDocument& doc, 
		const QString& name, 
		const GasAttributeMap& attributes = GasAttributeMap(), 
		const QList<QDomElement>& children = QList<QDomElement>(
		));

	QDomElement paramsToXml(QDomDocument& doc);
	QDomElement patientToXml(QDomDocument& doc);
	QDomElement weightToXml(QDomDocument& doc);
	QDomElement volumesToXml(QDomDocument& doc);
	QDomElement flowsToXml(QDomDocument& doc);
	QDomElement agentsToXml(QDomDocument& doc);
	QDomElement agentToXml(QDomDocument& doc, const GasAnesthetic& anes);
	QDomElement settingsToXml(QDomDocument& doc, bool wantResults);
	inline int getSampleCount() const{ return m_gasArray.first()->m_pSampArray->size(); }
	QDomElement settingToXml(QDomDocument& doc, GasSample& sample);
	QDomElement agentSettingsToXml(QDomDocument& doc, quint32 time);
	QDomElement agentSettingToXml(QDomDocument& doc, int gasNum, quint32 time);
	QDomElement resultsToXml(QDomDocument& doc, int& nSamp, int nSamps);
	QDomElement resultToXml(QDomDocument& doc, quint32 time);
	QDomElement agentResultToXml(QDomDocument& doc, int gasNum, float fMin);
	QDomElement compartmentsToXml(QDomDocument& doc, int gasNum, float fMin);
	QDomElement compartmentToXml(QDomDocument& doc, QString compartmentName, float val);

private slots:
	void idle();

signals:
  void updateControl();
  void updatePanel();
  void agentChanged(int nGas, const QString& agent);
  void allCleared();
	void modified( bool m = true );
	void stateChanged(AState state);
	void titleChanged(QString newTitle);
	void noViewsOfAgent(int nGas);
	void agentColorsChanged();
	 
// Attributes
public:
	// Due to the vagaries of segmentation, we generate a separate type
	// of pointer to a set of (big) arrays for results
	GasArray m_gasArray;					// Array of arrays of results
	QList<QString> colors;					// Chosen colors for above array
	GasAnesArray m_anesArray;				// Array of anesthetic parameters
	int m_nActiveGas;						// Gas# from last active view
	quint32 m_dwSched;						// Last scheduling # computed by app
	bool m_bOpened;							// From a file
	bool m_bClosed;							// going away
	int m_nBookmarks;
	quint32	m_dwBookmarks[MAX_BKMK];		// We stop when we hit one
	QList<int> m_targetLists[MAX_COMPART];	// compartment->percent MAC targets
	float m_target[MAX_COMPART][2];			// MAC low and high targets (breakpoints)
	QList<int> m_targetCompartments;		// Compartments where targets have been set
	int m_nTempmarks;
	quint32 m_dwTempmarks[MAX_BKMK];
	// Patient Defaults
	static float m_fDfltVA;
	static float m_fDfltCO;
	static float m_fDfltWeight;
	static COMP_ARRAY m_fDfltVolume;		// volume by compartment
	static COMP_ARRAY m_fDfltRatio;			// proportion of CO to compartment
	// Program Defaults
	static int m_nBeep;						// -2 off, -1 beep, 0-4 system sounds
	// Simulation Defaults
	static int m_nDfltSpeed;				// default initial speed
	static QString m_szDfltCircuit;
	static float m_fDfltFGF;
	// A Document is saved with the view paramters of view that was active
	// when it was saved:
	quint16 save_nActiveGas;
	quint16 save_HIGH_DEL;
	quint16 save_bShowUptakeCost;
	quint16 save_bShowMl;
	quint16 save_HIGH_FGF;
	quint16 save_HIGH_CO;
	quint16 save_HIGH_VA;
	quint16 save_nScaleMinutes;				// Last choice of x-axis in minutes
	quint16 save_nSpeed;
 
	enum aSerializationFlag		
	{
		SER_NONE = 0,
		SER_COLORS = 1,
		SER_DESCRIPTION = SER_COLORS << 1,
		SER_LOCAL_AGENT = SER_DESCRIPTION << 1,
		SER_NEW_HEADER = SER_LOCAL_AGENT << 1,
		SER_INITIAL_STATE = SER_NEW_HEADER << 1,
		SER_TARGETS = SER_INITIAL_STATE << 1
	};

	QString cond;
	quint32 getMaxTime(){return m_dwMaxTime;}
protected:
	quint16/*almostconst*/ m_cMSec_dx;		// milliseconds between calcs
	quint32 m_dwHighTime;					//Highest dwTime
	quint32 m_dwNextDisplayTime;			//Next desired time
	quint32 m_dwTime;						//Current time
	quint32 m_dwCalcTime;					//Highest calculated time
	quint32/*almostconst*/ m_dwMaxTime;		// probably 20 hours in msecs
	quint32 m_dwCalcStart;					//Lowest calculated time in primary buffer
	quint32 m_dwOldStart;					//Beginning of buffered time
	quint32 m_dwOldNext;					//End of bufferd time as we know it
	bool m_bBreak;							//TRUE iff m_dwNewTime is bookmarked
	float m_fdt;							//m_cMSec_dx/60k = dt in minutes
	quint32 m_dwSampTime;					//msec between samples
	short m_nSampIdx[MAX_SAMP];				// Index into sample array by interval
	short m_nLastSampAt;					// Optimization storage for SampAt()
	quint32 m_dwLastSampAtEnd;
	quint32 m_dwLastSampAtTime;
	//The Document keeps track of the current settings
	QString	m_strCircuit;					//Open, semi, or closed
	float m_fFGF;							//Current FGF setting
	float m_fVA;							//Current VA setting
	float m_fCO;							//Current CO setting
	float m_fWeight;						//Current(!) Patient weight
	bool m_bRtnEnb;							//Return Enabled
	bool m_bUptEnb;							//Uptake Enabled
	bool m_bVapEnb;							//Vaporization Enabled
	bool m_bFlush;							//User hit flush button
	int m_nSpeed;							//1 for 1x, 2 for 2x,...
	bool m_bSpeedup;						//Temporary AFAP
	AState m_theState;						//Running, Stopped, Initial...
	COMP_ARRAY m_fVolume;					//Volume by compartment
	COMP_ARRAY m_fRatio;					//Proportion of CO to compartment

private:
	quint32 m_SerializationFlags;

	QString description;
    QElapsedTimer m_dwProgress;
	quint32 m_dwAvgSched;					//Last computed avg scheduling #
	bool started;
    QTimer *timer;							//Perform idle processing
	QList< GasChildWindow *> childWindows;	//List of children connected with this document
	QString m_curFile;						//Path to a file where data are stored
	bool isUntitled;						//Check if the document is untitled

	QWidget *getDialogParent();
public:	
	QStringList op;
	QStringList si;
	float oldval;
	float expv;
private:
	friend class GasSample;
};

#endif
