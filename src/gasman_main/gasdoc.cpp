#include <limits>
#include "gasdoc.h"
#if QT_VERSION >=0x050000
#include <QtWidgets>
#include <QMessageBox>
#include <QtMultimedia/QSound>
#else
#include <QtGui>
#endif
#include <math.h>
#ifdef Q_OS_WIN
	#include <qt_windows.h>
	#include <mapi.h>
#endif
#include "math.h"
#include "gasapplication.h"
#include "gaschildwindow.h"
#include "gasmainwindow.h"
#include "gaspanelview.h"
#include "gasgraphview.h"
#include "gasdosedlg.h"
#include "gascostdlg.h"
#include "gasdeldlg.h"
#include "gaspatientdlg.h"
#include "gasbookmarkdlg.h"
#include "gasbkpoint.h"
float GasDoc::m_fDfltVA;
float GasDoc::m_fDfltCO;
float GasDoc::m_fDfltWeight;
int GasDoc::m_nBeep;
int GasDoc::m_nDfltSpeed;
float GasDoc::m_fDfltFGF;
QString GasDoc::m_szDfltCircuit;
COMP_ARRAY	GasDoc::m_fDfltVolume;
COMP_ARRAY	GasDoc::m_fDfltRatio;

//STATS
int gTotVernier[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/////////////////////////////////////////////////////////////////////////////
// GasDoc construction/destruction
GasDoc::GasDoc( QWidget * parent )
	: QObject( parent ), m_bOpened( false ), m_bClosed( false ), description(""), isUntitled( false )
{
	m_gasArray.append( new Gas( this ) );
	m_dwAvgSched = 0;
	started = false;
	m_SerializationFlags = SER_NONE;

    timer = new QTimer( this );
	connect( timer, SIGNAL( timeout() ), this, SLOT( idle() ) );
	save_nScaleMinutes = (quint16)GasGraphView::m_nDfltScaleMinutes;
 
	op.clear();
	si.clear();
}

GasDoc::~GasDoc()
{
	StopOnIdle();
	close();
}

QWidget *GasDoc::getDialogParent()
{
	QObject *p = childWindows.empty() ? parent() : childWindows[0];
	return qobject_cast<QWidget*>( p );
}


bool GasDoc::newDocument()
{
	static int sequenceNumber = 1;
	isUntitled = true;
	m_curFile = tr("Untitled%1.gas").arg(sequenceNumber++);

	DeleteContents();
	InitDocument();

	if ( m_gasArray.at(0)->m_nAgent == -1 ) {// Couldn't read in any valid agents
		QMessageBox::critical( getDialogParent(), tr( "Gas Man\xC2\xAE "), tr( "<p>Can't read anesthetic parameters from GASMAN.INI" ) );
		return false;
	}
	return true;
}

// Remove any contents and any indications that there ARE contents
void GasDoc::DeleteContents()
{
	colors.clear();
	m_nActiveGas = 0;

	// Lose any 2nd gasses

	for ( int i = 1; i < m_gasArray.size();  )
		delete m_gasArray.takeAt( i );
	
	UpdateAllViews( 0, 0, GasUpdateEvent::UPD_NGAS );

	// Lose primary results
	m_gasArray.at( 0 )->DeleteContents();

	// Lose any old anesthetics
	m_anesArray.clear();

	// Set everything to 'just starting and empty'

	m_theState = INITIAL_STATE;
	emit stateChanged(m_theState);
	//ChangeState(0, INITIAL_STATE);
	m_dwNextDisplayTime = 0;
	m_dwTime = 0;
	m_dwHighTime = 0;
	m_dwCalcTime = 0;
	m_dwCalcStart = 0;
	m_dwOldStart = 0;
	m_dwOldNext = 0;
	m_nLastSampAt = -1;
	m_nBookmarks = 0;
	for (int i = 0; i < MAX_COMPART; ++i)
		m_targetLists[i].clear();
	save_nActiveGas = NGS + 100;
}

void GasDoc::ReadProfile()
{
	float fSum;
	QString str;

	m_nBeep = gasApp->ReadProfile( "Defaults", "Click", 0 ).toInt();

	str = gasApp->ReadProfile( "Defaults", "Speed", QString() ).toString();
	if ( !QString::compare( str, "AFAP", Qt::CaseInsensitive ) )
		m_nDfltSpeed = AFAP;
	else
		m_nDfltSpeed = str.toInt();
	
	switch ( m_nDfltSpeed )
	{
	case 1: case 2: case 5: case 10: case 20: case 60: case 120: case AFAP:
		break;
	default:
		m_nDfltSpeed = 20; break;
	}

	str = gasApp->ReadProfile( "Defaults", "Circuit", "Open" ).toString();
	if ( str[0] == 's' || str[0] == 'S' )
		m_szDfltCircuit = tr( "Semi-closed" );
	else if ( str[0] == 'i' || str[0] == 'I' )
		m_szDfltCircuit = tr( "Ideal" );
	else if ( str[0] == 'c' || str[0] == 'C' )
		m_szDfltCircuit = tr( "Closed" );
	else
		m_szDfltCircuit = tr( "Open", "open circuit" );

	m_fDfltFGF = gasApp->ReadProfile( "Defaults", "FGF", -1.0 ).toDouble();	
	if ( m_fDfltFGF < 0.0F || m_fDfltFGF > MAX_FGF ) m_fDfltFGF = INIT_SEMI_FGF;

	m_fDfltVA = gasApp->ReadProfile( "Defaults", "VA", -1.0 ).toDouble();
	if ( m_fDfltVA < 0.0F || m_fDfltVA > MAX_VA ) m_fDfltVA = STD_VA;

	m_fDfltCO = gasApp->ReadProfile( "Defaults", "CO", -1.0 ).toDouble();
	if ( m_fDfltCO < 0.0F || m_fDfltCO > MAX_CO ) m_fDfltCO = STD_CO;

	m_fDfltWeight = gasApp->ReadProfile( "Defaults", "Weight", 0.0 ).toDouble();
	if ( m_fDfltWeight < MIN_WEIGHT || m_fDfltWeight > MAX_WEIGHT ) m_fDfltWeight = STD_WEIGHT;

	// Read in the compartment volumes
	
	m_fDfltVolume[CKT] = gasApp->ReadProfile( "Volumes", "CKT", 0.0 ).toDouble();
	if ( m_fDfltVolume[CKT] <= 0.0 || m_fDfltVolume[CKT] > 100.0 ) m_fDfltVolume[CKT] = 8.0F;

	m_fDfltVolume[ALV] = gasApp->ReadProfile( "Volumes", "ALV", 0.0 ).toDouble();
	if ( m_fDfltVolume[ALV] <= 0.0 || m_fDfltVolume[ALV] > 100.0 ) m_fDfltVolume[ALV] = 2.5F;

	m_fDfltVolume[VRG] = gasApp->ReadProfile( "Volumes", "VRG", 0.0 ).toDouble();
	if ( m_fDfltVolume[VRG] <= 0.0 || m_fDfltVolume[VRG] > 100.0 ) m_fDfltVolume[VRG] = 6.0F;
	
	m_fDfltVolume[MUS] = gasApp->ReadProfile( "Volumes", "MUS", 0.0 ).toDouble();
	if ( m_fDfltVolume[MUS] <= 0.0 || m_fDfltVolume[MUS] > 1000.0 ) m_fDfltVolume[MUS] = 33.0F;

	m_fDfltVolume[FAT] = gasApp->ReadProfile( "Volumes", "FAT", 0.0 ).toDouble();
	if ( m_fDfltVolume[FAT] <= 0.0 || m_fDfltVolume[FAT] > 100.0 ) m_fDfltVolume[FAT] = 14.5F;

	m_fDfltVolume[VEN] = gasApp->ReadProfile( "Volumes", "VEN", 0.0 ).toDouble();
	if ( m_fDfltVolume[VEN] <= 0.0 || m_fDfltVolume[VEN] > 10.0 ) m_fDfltVolume[VEN] = 1.0F;

	// Read in the CO ratio
	
	fSum = 0.0F;
	m_fDfltRatio[VRG] = gasApp->ReadProfile( "Ratio", "VRG", -1.0 ).toDouble();
	if ( m_fDfltRatio[VRG] >= 0.0 )
		fSum += m_fDfltRatio[VRG];

	m_fDfltRatio[MUS] = gasApp->ReadProfile( "Ratio", "MUS", -1.0 ).toDouble();
	if ( m_fDfltRatio[MUS] >= 0.0 )
		fSum += m_fDfltRatio[MUS];
	
	m_fDfltRatio[FAT] = gasApp->ReadProfile( "Ratio", "FAT", -1.0 ).toDouble();
	if ( m_fDfltRatio[FAT] >= 0.0 )
		fSum += m_fDfltRatio[FAT];

	if ( fSum < 1.001 && fSum > 0.999F ) {
		if ( m_fDfltRatio[VRG] < 0.0F ) m_fDfltRatio[VRG] = 0.0F;
		if ( m_fDfltRatio[MUS] < 0.0F ) m_fDfltRatio[MUS] = 0.0F;
		if ( m_fDfltRatio[FAT] < 0.0F ) m_fDfltRatio[FAT] = 0.0F;
	}
	else if ( fSum == 100.0 ) {
		if ( m_fDfltRatio[VRG] < 0.0F ) m_fDfltRatio[VRG] = 0.0F; else m_fDfltRatio[VRG] /= 100.0F;
		if ( m_fDfltRatio[MUS] < 0.0F ) m_fDfltRatio[MUS] = 0.0F; else m_fDfltRatio[MUS] /= 100.0F;
		if ( m_fDfltRatio[FAT] < 0.0F ) m_fDfltRatio[FAT] = 0.0F; else m_fDfltRatio[FAT] /= 100.0F;
	}
	else {
		m_fDfltRatio[VRG] = 0.76F;
		m_fDfltRatio[MUS] = 0.18F;
		m_fDfltRatio[FAT] = 0.06F;
	}
	m_fDfltRatio[CKT] = m_fDfltRatio[ALV] = m_fDfltRatio[VEN] = 1.0F;
}

// Set default parameters for the document.  Should DeleteContents() first
void GasDoc::InitDocument()
{
	// Someday, we may change the granularity.  Certainly, we will change
	// the anesthetic list.  Therefore, these get saved in the simulation
	m_anesArray.ReadProfile();			// Get anesthetic parameters

	if ( m_anesArray.size() ) {	// There are anesthetics in the INI file
		QString str = gasApp->ReadProfile( "Defaults", "Agent", QString() ).toString();
		int i = m_anesArray.LookupSection( str );
		if ( i < 0 ) i = 0;
		m_gasArray.at(0)->SetAgent( m_anesArray[i]->m_strName );
		m_gasArray.at(0)->SetDEL( m_anesArray[i]->m_fDefDEL );
		m_gasArray.at(0)->SetInitialResults();
		AddDefaultAgentColor( m_anesArray[i]->m_strName );
	}
	else //No valid anesthetics!!
		return;										// Catch this later

	SetTimeVariables(DT);

	m_fWeight = m_fDfltWeight;
	double factor = (double)m_fWeight / STD_WEIGHT;
	factor = sqrt( sqrt( factor*factor*factor ) );
	m_bRtnEnb = m_bUptEnb = true;
	m_bFlush = false;
	m_bVapEnb = false;
	m_nSpeed = m_nDfltSpeed;

	m_strCircuit = m_szDfltCircuit;

	if ( m_strCircuit[0] == tr( "Semi-closed" ).at(0) || m_strCircuit[0] == tr( "Ideal" ).at(0) )
		m_fFGF = m_fDfltFGF;
	else if ( m_strCircuit[0] == tr( "Closed" ).at(0) )
		m_fFGF = STD_FGF * factor;
	else
		m_fFGF = 10.0;

	m_fVA = m_fDfltVA * factor;
	m_fCO = m_fDfltCO * factor;

	// Read in the compartment volumes and ratios
	
	for ( int i = 0; i < MAX_COMPART; ++i ) {
		m_fVolume[i] = m_fDfltVolume[i];
		m_fRatio[i] = m_fDfltRatio[i];
	}
}

void GasDoc::SetTimeVariables( quint16 dt )
{
	m_cMSec_dx = dt;								// Set time variables
	m_fdt = m_cMSec_dx / 60000.0F;
	m_dwSampTime = (quint32)SAMP_EVERY * dt;

	quint32 nBreathsPerHour = (60*60*1000 + dt - 1)/dt;
	quint32 nSamplesPerHour = nBreathsPerHour/SAMP_EVERY + MAX_ADJ_PER_HOUR;
	quint32 nMaxHours = MAX_SAMP/nSamplesPerHour;
	if( nMaxHours > MAX_HOURS )
		nMaxHours = MAX_HOURS;
	m_dwMaxTime = nMaxHours * 3600L * 1000L;
}

bool GasDoc::duplicateAgents()
{
	QList<QString> agents;
	for(int i = 0; i < m_gasArray.size(); ++i)
	{
		agents << m_gasArray[i]->m_strAgent;
	}
    const QSet<QString>strVe(agents.begin(),agents.begin());
    return (agents.size() != agents.size()); // return (agents.size() != agents.toSet().size());
}

// Reset the state from some sample
void GasDoc::ResetState(const GasSample& samp)
{
	m_theState = INITIAL_STATE;
	emit stateChanged(m_theState);
	m_dwNextDisplayTime = m_dwTime = 0;

	// Set the circuit type

	switch ( samp.m_uCktType )
	{
	case OPEN_CKT:
		m_strCircuit = tr( "Open", "open circuit" ); break;
	case SEMI_CKT:
		m_strCircuit = tr( "Semi-closed" ); break;
	case CLOSED_CKT:
		m_strCircuit = tr( "Closed" ); break;
	case IDEAL_CKT:
		m_strCircuit = tr( "Ideal" ); break;
	default:
		Q_ASSERT( false );
	}

	// Set the state variables

	m_fWeight   = samp.m_fWeight;
	m_bRtnEnb   = samp.m_bRtnEnb;
	m_bUptEnb   = samp.m_bUptEnb;
	m_bVapEnb   = samp.m_bVapEnb;
	m_bFlush	= samp.m_bFlush;
	m_fFGF = samp.m_fFGF;
	m_fVA  = samp.m_fVA;
	m_fCO  = samp.m_fCO;
}

// Compute terms invariant with CO, weight and selected anesthetic

#ifdef EULAR
void GasDoc::ComputeTerms( float fCO, float fWeight, int nAgent,
			   COMP_ARRAY &fEffectiveFlowTo, COMP_ARRAY &fdtOverEffectiveVolume )
{
	COMP_ARRAY &pf = m_anesArray[nAgent]->m_fSolubility;
	float fWtFactor = fWeight/STD_WEIGHT;
	float fLambdaBG;

	for ( int i=0; i < MAX_COMPART; ++i ) {
		fdtOverEffectiveVolume[i] = m_fdt/(m_fVolume[i]*pf[i]);
		if (i != CKT) fdtOverEffectiveVolume[i] /= fWtFactor;
	}

	fLambdaBG = m_anesArray[nAgent]->m_fLambdaBG;
	fEffectiveFlowTo[VRG] = fCO * m_fRatio[VRG] * fLambdaBG;
	fEffectiveFlowTo[MUS] = fCO * m_fRatio[MUS] * fLambdaBG;
	fEffectiveFlowTo[FAT] = fCO * m_fRatio[FAT] * fLambdaBG;
}
#else //!EULAR
void GasDoc::ComputeTerms( float /*fFGF*/, float /*fVA*/, float fCO, float fWeight, int nAgent,
			   COMP_ARRAY &fEffectiveFlowTo, COMP_ARRAY (&fExp)[MAX_VERNIER])
{
	COMP_ARRAY &pf = m_anesArray[nAgent]->m_fSolubility;
	float fWtFactor = fWeight/STD_WEIGHT;
	float fLambdaBG = m_anesArray[nAgent]->m_fLambdaBG;
	float efTot = fCO * fLambdaBG;
	fEffectiveFlowTo[VRG] = efTot * m_fRatio[VRG];
	fEffectiveFlowTo[MUS] = efTot * m_fRatio[MUS];
	fEffectiveFlowTo[FAT] = efTot * m_fRatio[FAT];

	for ( int i = 0; i < VEN; ++i ) {
		double d = m_fdt/((double)m_fVolume[i]*pf[i]);

		switch (i)
		{
		case CKT:
			// (times (FGF + VA), computed each time...)
			break;
		case ALV:
			// (times (VA + CO*lambda), computed each time...)
			d /= fWtFactor;
			break;
		default:
			d *= fEffectiveFlowTo[i]/fWtFactor;
		}
		d = 0 - d;
		for ( int j = 0; j < MAX_VERNIER; ++j ) {
			if ( i == CKT || i == ALV )
				fExp[j][i] = float(d);	// (exp computed each time...)
			else
				fExp[j][i] = float(exp(d));
			d /= 2;
		}
	}
}
#endif //!EULAR

void GasDoc::rewind()
{
	if ( m_theState == RUNNING_STATE )
	{
		ChangeState( 0, STOPPED_STATE );		
	}

	ResetState(*m_gasArray.at(0)->m_pSampArray->at(0));
	for ( int ng = 0; ng < m_gasArray.size(); ++ng )
		m_gasArray.at(ng)->LoadFirstSampleState();
	UpdateAllViews();
}

void GasDoc::fastFwd()
{
	if (IsOver())
		return;
	if ( m_theState == RUNNING_STATE )
		ChangeState( 0, STOPPED_STATE );

 
	m_nTempmarks = 0;			// Delete all temporary bookmarks
	StepSimulation( -1 );		// get the latest
}

void GasDoc::zeroTimer()
{
	QString strMessage = tr( "<p>Erase simulation ""%1"" but keep current settings and values?" ).arg( title() );

	if ( m_theState == RUNNING_STATE )
		ChangeState( 0, STOPPED_STATE );

	int ret = QMessageBox::question( childWindows[0], tr( "Gas Man\xC2\xAE" ), strMessage, QMessageBox::Ok, QMessageBox::Cancel | QMessageBox::Default );
	if ( ret != QMessageBox::Ok )
		return;

	// Reset the initial sample(s)
	for ( int ng = 0; ng < m_gasArray.size(); ++ng )
	{
		GasSample* s =new GasSample(m_gasArray.at(ng), m_gasArray.at(ng)->m_fResults);
		s->m_dwBaseTime = 0L;
		m_gasArray.at(ng)->SetInitialSamp( s );
	}

	m_theState = INITIAL_STATE;
	emit stateChanged(m_theState);

	m_dwNextDisplayTime = m_dwTime = 0L;

	Truncate();
	for ( int ng = 0; ng < m_gasArray.size(); ++ng )
		m_gasArray.at(ng)->SetInitialResults();
	UpdateAllViews();
}

void GasDoc::clearAll()
{
	QString strMessage = tr( "<p>Erase simulation ""%1"" and restore settings to defaults?" ).arg( title() );

	if ( m_theState == RUNNING_STATE )
		ChangeState( 0, STOPPED_STATE );

	int ret = QMessageBox::question( childWindows[0], tr( "Gas Man\xC2\xAE "), strMessage, QMessageBox::Ok | QMessageBox::Default, QMessageBox::Cancel );
	if ( ret == QMessageBox::Ok ) {
		//ugly fix for "reset making all secondary gasses same as primary"
		foreach(GasChildWindow* child, gasChildren())
		{
			int nGas;
            if ((child->getViewType() == CONTROL) || (child->tryGetGas(nGas) && nGas == 0 ))
				continue;
			child->close();
		}
		DeleteContents();	
		InitDocument();
		if ( m_gasArray.at(0)->m_nAgent == -1 ) { // Couldn't read in any valid agents
			QMessageBox::critical( childWindows[0], tr( "Gas Man\xC2\xAE "),  tr( "<p>Can't read anesthetic parameters from GASMAN.INI" ), QMessageBox::Ok );
			qApp->exit();
			return;
		}

		emit modified();
		emit allCleared();
		emit agentColorsChanged();
		updateTitle();
		UpdateAllViews();
	}
}

void GasDoc::unitDose()
{
	GasDoseDlg dlg;

	if ( m_theState == RUNNING_STATE ) ChangeState( 0, STOPPED_STATE );
	if ( !CheckChange() ) return;

	dlg.setDose( m_gasArray.at( m_nActiveGas )->m_fUnitDose );
	
	if ( dlg.exec() == QDialog::Rejected )
		return;

	if ( m_gasArray.at( m_nActiveGas )->m_fUnitDose == dlg.dose() )
		return;

	m_gasArray.at( m_nActiveGas )->m_fUnitDose = dlg.dose();

	Truncate();
}

void GasDoc::setCost()
{
	GasAnesthetic &anes = *m_anesArray[m_gasArray.at( m_nActiveGas )->m_nAgent];
	GasCostDlg dlg;

	if ( m_theState == RUNNING_STATE ) ChangeState( 0, STOPPED_STATE );
	
	dlg.setBottleCost( anes.m_fBottleCost );
	dlg.setBottleSize( anes.m_nBottleSize );
	
	if ( dlg.exec() == QDialog::Rejected )
		return;

	if ( dlg.bottleCost() == anes.m_fBottleCost && dlg.bottleSize() == anes.m_nBottleSize ) return;

	anes.m_nBottleSize   = dlg.bottleSize();
	anes.m_fBottleCost   = dlg.bottleCost();

	emit modified();	// (because this gets saved with the simulation)
	UpdateAllViews( 0, m_nActiveGas+1, GasUpdateEvent::UPD_TICK );
}

void GasDoc::adjustDel()
{
	GasDelDlg dlg;

	if ( m_theState == RUNNING_STATE ) ChangeState( 0, STOPPED_STATE );
	if ( !CheckChange() ) return;

	dlg.setMaxDEL( (*m_anesArray[m_gasArray.at( m_nActiveGas )->m_nAgent]).m_fMAX_DEL );
	dlg.setDEL( m_gasArray.at( m_nActiveGas )->m_fDEL );
	dlg.setFGF( m_fFGF );
	dlg.setVA( m_fVA );
	dlg.setCO( m_fCO );
	
	if ( dlg.exec() == QDialog::Rejected )
		return;

	if ( m_gasArray.at( m_nActiveGas )->m_fDEL != dlg.DEL() )
		SetDEL( (QWidget *)-1L, m_nActiveGas, dlg.DEL() );

	if ( m_fFGF != dlg.FGF() ) {
		m_fFGF = dlg.FGF();
		SetFGF( (QWidget *)-1L, m_fFGF );
	}
	if ( m_fVA != dlg.VA() ) {
		m_fVA = dlg.VA();
		SetVA( (QWidget *)-1L, m_fVA );
	}
	if ( m_fCO != dlg.CO() ) {
		m_fCO = dlg.CO();
		SetCO( (QWidget *)-1L, m_fCO );
	}
}

void GasDoc::changePatient()
{
	GasPatientDlg dlg;

	if ( m_theState == RUNNING_STATE)
		ChangeState( 0, STOPPED_STATE );
	dlg.patientParameters()->setInitialState( m_theState == INITIAL_STATE );

	if ( dlg.patientParameters()->initialState() && !CheckChange( true ) ) return;
	
	dlg.patientParameters()->setWeight( m_fWeight );
	dlg.patientParameters()->setVolumeALV( m_fVolume[ALV] );
	dlg.patientParameters()->setVolumeVRG( m_fVolume[VRG] );
	dlg.patientParameters()->setVolumeMUS( m_fVolume[MUS] );
	dlg.patientParameters()->setVolumeFAT( m_fVolume[FAT] );
	dlg.patientParameters()->setVolumeVEN( m_fVolume[VEN] );
	dlg.patientParameters()->setRatioVRG( qRound( m_fRatio[VRG]*100 ) );
	dlg.patientParameters()->setRatioMUS( qRound( m_fRatio[MUS]*100 ) );
	dlg.patientParameters()->setRatioFAT( qRound( m_fRatio[FAT]*100 ) );
	dlg.patientParameters()->setCO( m_fCO );
	dlg.patientParameters()->setVA( m_fVA );
	
	if ( dlg.exec() == QDialog::Rejected ) 
		return;

	if ( dlg.patientParameters()->weight() != m_fWeight)
		SetWeight( (QWidget *)-1L, dlg.patientParameters()->weight() );
	m_fVolume[ALV] = dlg.patientParameters()->volumeALV();
	m_fVolume[VRG] = dlg.patientParameters()->volumeVRG();
	m_fVolume[MUS] = dlg.patientParameters()->volumeMUS();
	m_fVolume[FAT] = dlg.patientParameters()->volumeFAT();
	m_fVolume[VEN] = dlg.patientParameters()->volumeVEN();
	m_fRatio[VRG] = dlg.patientParameters()->ratioVRG() / 100.0;
	m_fRatio[MUS] = dlg.patientParameters()->ratioMUS() / 100.0;
	m_fRatio[FAT] = dlg.patientParameters()->ratioFAT() / 100.0;
	m_fCO = dlg.patientParameters()->CO();
	m_fVA = dlg.patientParameters()->VA();
}

void GasDoc::setBookmark()
{
	GasBookmarkDlg dlg;

	if ( m_theState == RUNNING_STATE ) ChangeState( 0, STOPPED_STATE );

	dlg.setHighTime( m_dwMaxTime );
	dlg.setTime( m_dwTime );
	dlg.setDoc( this );
	dlg.exec();
}
void GasDoc::setBkPoint()
{
	gasBkPoint dlg;
	dlg.setDoc(this);
	dlg.exec();

}

void GasDoc::disableUptake( bool set )
{
	if ( m_theState == RUNNING_STATE )
		ChangeState( 0, STOPPED_STATE );
	if ( !CheckChange( true ) ) return;
	m_bUptEnb = !set;
	emit updateControl();
}

void GasDoc::disableReturn( bool set )
{
	if ( m_theState == RUNNING_STATE )
		ChangeState( 0, STOPPED_STATE );
	
	if ( !CheckChange( true ) ) return;
	m_bRtnEnb = !set;
	emit updateControl();
}

void GasDoc::enableVapor( bool set )
{
	if ( m_theState == RUNNING_STATE )
		ChangeState( 0, STOPPED_STATE );
	if ( !CheckChange( true ) ) return;
	m_bVapEnb = set;
	emit updateControl();
}

void GasDoc::copyData()
{
	long dw = m_dwTime/1000L;
	int hh = (int)(dw/3600L);
	int mm = (int)(dw-hh*3600)/60;
	int ss = (int)(dw-((hh*60)+mm)*60L);	
	QString strRow = QString( "%1:%2:%3" ).arg( hh ).arg( mm, 2, 10, QChar( '0' ) ).arg( ss, 2, 10, QChar( '0' ) );
	
	strRow += '\t';

	int nGas = m_nActiveGas >= 0 && m_nActiveGas < m_gasArray.length() ? m_nActiveGas : 0;
	 
	strRow += QString::number( m_gasArray.at( nGas )->m_fDEL, 'f' );

	for (int i = 0; i < MAX_RESULT; ++i ) {
		strRow += '\t';
		strRow += QString::number( m_gasArray.at( nGas )->m_fResults[i], 'f' );
	}

	float fRatio = GetCostRatio( nGas );
	float f = m_gasArray.at(nGas)->m_fResults[UPT]*fRatio;
	strRow += '\t';
	strRow += QString::number( f, 'f' );
	
	f = m_gasArray.at(nGas)->m_fResults[DEL]*fRatio;
	strRow += '\t';
	strRow += QString::number( f, 'f' );
	
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->clear();
	clipboard->setText( strRow );
}

void GasDoc::ChangeState( QWidget *that, const AState theState )
{
	emit(stateChanged(theState));

	m_theState = theState;

	m_nTempmarks = 0;					// Delete all temporary bookmarks when state changes
	m_dwNextDisplayTime = 0L;					// No last time requested

	UpdateAllViews( that, 0, GasUpdateEvent::UPD_STATE );

	if ( theState == RUNNING_STATE ) {
		m_bSpeedup = (gasApp->keyboardModifiers() & Qt::ShiftModifier);
		m_dwSched = m_dwAvgSched * qMin( GetCurrentSpeed(), 150 );
		RunOnIdle();
	}
	else
		StopOnIdle();

	gasMainWindow->updateAllActions();
}

// Check if we are about to change the remainder of a simulation.  If so,
// verify that that is what the user wants to do.  If you are committed to
// making the change if it is OK to do so, setting bTrunc will clear the
// remainder of the simulation if it is OK.  If pStat is not null, will
// set *pStat if a dialog is raised. Returns TRUE if change is OK.
bool GasDoc::CheckChange( bool bTrunc, bool *pStat )
{
	if (pStat != NULL)
		*pStat = false;				// Guess: no dialog required

	if ( m_dwTime < m_dwHighTime ) {	// if there are events the user's already seen...
		if ( m_theState == RUNNING_STATE ) ChangeState( 0, STOPPED_STATE );

		QWidget *grabber = QWidget::mouseGrabber();
		if ( grabber )
			grabber->releaseMouse();
		
		if (pStat != NULL)
			*pStat = true;
		int ret = QMessageBox::warning( 
			childWindows[0],
			tr( "Gas Man\xC2\xAE "),
			tr("<p>Changing conditions while replaying a simulation "
				"will erase it from this point forward! (This won't "
				"affect any saved versions.)"
				"<p>Press 'OK' to proceed..."),
			QMessageBox::Ok,
			QMessageBox::Cancel | QMessageBox::Default );
		if ( ret != QMessageBox::Ok )
		{
			emit updatePanel();
			emit updateControl();
			return false;
		}
	}
		
    if ( bTrunc )
			Truncate();
    return true;
}

// See if we can inject a bolus of this agent
bool GasDoc::CanInject( const int nGas ) const
{
	if ( GetUnitDose(nGas) <= 0.0F )
		return false;
	return m_strCircuit[0] != tr( "Open", "open circuit" ).at(0) && m_strCircuit[0] != tr( "Ideal" ).at(0);
}

// The following routine is slightly mis-named.  It is called whenever
// any conditions (settings) of the simulation are changed to chop off
// any subsequent calculations based on the current settings.  However,
// its primary responsibility is to insure that any further computation
// will cause a new sample to be taken, and may be called even if there
// ARE no future calculations.  (see sample.h)

void GasDoc::Truncate()
{
	GasSampArray& sampArray = *(m_gasArray.at(0)->m_pSampArray );
	int nCurSamp, nNextSamp, nMaxSamp, i, ng;
	int ngs = m_gasArray.size();
	float *pf1, *pf2;
	quint32 dwNewCalcTime;

	emit modified();					// An attempt to truncate is caused by change
 	
	// Clear out any flushing / injections pending

	for ( ng=0; ng < ngs; ++ng ) {
		m_gasArray.at(ng)->m_nInjections = 0;
	}
	m_bFlush = false;


	if ( m_dwCalcTime == 0 ) {				// if there are no calculations yet...
		Q_ASSERT( m_gasArray.at(0)->m_pCurSamp == 0 );
		Q_ASSERT( !(sampArray.size()) );
		return;
	}
	
	// We are changing conditions at the current time.  Invalidate any
	// subsequent samples and calculations.
	
	if ( m_dwCalcTime >  m_dwTime ) {	// If we've calculated for time beyond m_dwTime

		// Set calc time to the tick preceding or at m_dwTime.

		dwNewCalcTime = ( m_dwTime / m_cMSec_dx ) * m_cMSec_dx;

		// If the new time isn't in the current buffer, then prime the buffer from history

		if ( dwNewCalcTime < m_dwCalcStart)	{
			for ( ng = 0; ng < ngs; ++ng ) {
				RES_ARRAY& res = ResultsAt( dwNewCalcTime, ng );
				pf1 = *(m_gasArray.at(ng)->m_pfCurResults + int( dwNewCalcTime / m_cMSec_dx ) % SAMP_EVERY );
				pf2 = res;
				for ( i = 0; i < MAX_RESULT; ++i )
					*pf1++ = *pf2++;
			}
			m_dwCalcStart = dwNewCalcTime;	// new earliest calculation
		}

		// Point to the sample for the new calc time. Discard if it needs to be re-taken

		nCurSamp = SampAt( dwNewCalcTime );
		if ( dwNewCalcTime == sampArray[nCurSamp]->m_dwBaseTime )
		{//don't keep if coincidental with current time
			nNextSamp = nCurSamp;
			m_nLastSampAt = -1;		// invalidate cache
		}
		else {	//keep the earlier sample
			nNextSamp = nCurSamp + 1;
			m_dwLastSampAtEnd = dwNewCalcTime;
		}

		// Delete any samples beyond the one for the new calc time

		nMaxSamp = sampArray.size();
		if ( nMaxSamp > nNextSamp )	{
			for ( ng = 0; ng < ngs; ++ng ) {
				Gas *g = m_gasArray.at(ng);
				GasSampArray &sampArray = *(g->m_pSampArray);

				for ( i = nNextSamp; i < nMaxSamp; ++i ) {
					//if ( sampArray[i]->m_dwBaseTime <= m_dwTime ) {
					//	g->m_nInjections += sampArray[i]->m_uInjections;
					//	m_bFlush |= sampArray[i]->m_bFlush;
					//}
					delete sampArray[i];
				}
				for ( int index = nNextSamp; index < nMaxSamp; ++index )
					sampArray.removeAt( nNextSamp );
				
				//If we threw any away, we must've thrown out the last
				g->m_pCurSamp = 0;
			}
		}

		// Make sure the secondary buffer contains no future data

		if ( m_dwOldNext > dwNewCalcTime ) {
			m_dwOldNext = dwNewCalcTime + m_cMSec_dx;
			//Just for consistency...
			if ( m_dwOldStart > m_dwOldNext )
				m_dwOldStart = m_dwOldNext;
		}

		m_dwCalcTime = dwNewCalcTime;		// reset last calculation
		m_dwHighTime = m_dwTime;			// new logical 'end'
		gasMainWindow->updateAllActions();
	}
	
	for ( ng = 0; ng < ngs; ++ng )
		if ( m_gasArray.at(ng)->m_pCurSamp != 0 ) {
			Q_ASSERT( m_gasArray.at(ng)->m_pCurSamp->m_dwBaseTime < m_dwCalcTime );			// no nulls
			m_gasArray.at(ng)->m_pCurSamp = 0;												// force taking of new sample
		}
}

// Return the index into m_nSampArray for the sample just before dwTime
int GasDoc::SampAt( quint32 dwTime )
{
	if ( m_nLastSampAt >= 0 && dwTime < m_dwLastSampAtEnd && dwTime >= m_dwLastSampAtTime)
		return m_nLastSampAt;				//optimize

	GasSampArray &sampArray = *(m_gasArray.at(0)->m_pSampArray);
	int nMaxSamp = sampArray.size();
	Q_ASSERT( nMaxSamp!=0 );

	// Special case: At current calc time, we may not yet have
	// taken a sample, so just return the latest...

	if ( dwTime >= m_dwCalcTime )
		return nMaxSamp-1;

	// Get the sample at the beginning of the interval containing dwTime

	short nCurSamp = m_nSampIdx[(short)(dwTime/m_cMSec_dx/SAMP_EVERY)];
	Q_ASSERT(nCurSamp>=0 && nCurSamp < nMaxSamp);
	Q_ASSERT(sampArray[nCurSamp]->m_dwBaseTime <= dwTime);

	// Search forward until we find one after dwTime

	while ( nCurSamp < nMaxSamp && sampArray[nCurSamp]->m_dwBaseTime <= dwTime )
		nCurSamp++;

	// Remember for next time

	m_dwLastSampAtEnd = nCurSamp < nMaxSamp ? sampArray[nCurSamp]->m_dwBaseTime : m_dwCalcTime;
	m_dwLastSampAtTime = sampArray[--nCurSamp]->m_dwBaseTime;
	m_nLastSampAt = nCurSamp;
	return nCurSamp;
}

// Return a reference to the RES_ARRAY for time dwTime.  This may require
// going back into history and recomputing forward from some sample.
RES_ARRAY& GasDoc::ResultsAt( quint32 dwTime, int nGas )
{
	int i;

	// We don't prognosticate

	Q_ASSERT( dwTime / m_cMSec_dx <= m_dwCalcTime / m_cMSec_dx );

	// Is it in the current buffer?

	if ( dwTime >= m_dwCalcStart ) {						// oh goody
		i = (int)( ( dwTime / m_cMSec_dx ) % SAMP_EVERY );
		return m_gasArray.at(nGas)->m_pfCurResults[i];
	}

	// How about the last buffer?
 
	if ( dwTime >= m_dwOldStart && dwTime < m_dwOldNext ) {
		i= (int)( ( dwTime / m_cMSec_dx ) % SAMP_EVERY );
		return m_gasArray.at(nGas)->m_pfOldResults[i];
	}

	// Damn.  Must recalc forward from the samples.

	return ResultsFromHistoryAt( dwTime, nGas );
}

RES_ARRAY& GasDoc::ResultsFromHistoryAt( quint32 dwTime, int nGas )
{
	quint32 dwt, dwNext, dwBaseTime;
	int ngs = m_gasArray.size();
	Q_ASSERT( ngs <= NGS );
	GasSample *samps[NGS];
	RES_ARRAY fResults[NGS];
#ifdef EULAR
	COMP_ARRAY fdtOverEffectiveVolume[NGS], *pfdtOEV[NGS];
#else //!EULAR
	COMP_ARRAY fExp[NGS][MAX_VERNIER], (*pfExp[NGS])[MAX_VERNIER];
#endif //!EULAR
	COMP_ARRAY fEffectiveFlowTo[NGS], *pfEFT[NGS];
	int njections[NGS];
	float fUptake[NGS], fTotUptake;
	float *pf;
	int i, nres, ng;

	int nPrevSamp = SampAt( dwTime );
	dwBaseTime = m_gasArray.at(0)->m_pSampArray->at(nPrevSamp)->m_dwBaseTime;
	nres = (int)( ( dwBaseTime / m_cMSec_dx ) % SAMP_EVERY );
	for ( ng = 0; ng < ngs; ++ng ) {
		Gas *g = m_gasArray.at(ng);
		GasSample &samp = *g->m_pSampArray->at(nPrevSamp);

		samps[ng] = &samp;
		njections[ng] = samp.m_uInjections;

		// See if we have computational factors already

#ifdef EULAR
		if ( g->m_nTermAgent != (int)samp.m_wAnesID || g->m_fTermCO != samp.m_fCO
		    || g->m_fTermWeight != samp.m_fWeight )	{		//Need to recalc other factors... double damn
			
			ComputeTerms( samp.m_fCO, samp.m_fWeight, samp.m_wAnesID, fEffectiveFlowTo[ng], fdtOverEffectiveVolume[ng] );
			pfdtOEV[ng] = fdtOverEffectiveVolume + ng;
			pfEFT[ng] = fEffectiveFlowTo + ng;
		}
		else {		// At least we can point to the ones we've got
			pfdtOEV[ng] = &g->m_fdtOverEffectiveVolume;
			pfEFT[ng] = &g->m_fEffectiveFlowTo;
		}
#else //!EULAR
		if ( g->m_nTermAgent != (int)samp.m_wAnesID || g->m_fTermFGF != samp.m_fFGF
		    || g->m_fTermVA != samp.m_fVA || g->m_fTermCO != samp.m_fCO || g->m_fTermWeight != samp.m_fWeight )	{//Need to recalc other factors... double damn
			ComputeTerms( samp.m_fFGF, samp.m_fVA, samp.m_fCO, samp.m_fWeight, samp.m_wAnesID, fEffectiveFlowTo[ng], fExp[ng] );
			pfExp[ng] = fExp + ng;
			pfEFT[ng] = fEffectiveFlowTo + ng;
		}
		else {	// At least we can point to the ones we've got
			pfExp[ng] = &g->m_fExp;
			pfEFT[ng] = &g->m_fEffectiveFlowTo;
		}
#endif //!EULAR
		// Copy the first result from the sample

		for ( i = 0, pf = samp.m_fResults; i < MAX_RESULT; ++i )
			g->m_pfOldResults[nres][i] = fResults[ng][i] = pf[i];
	}

	// Stop at the next sample or the end of time

	if ( nPrevSamp < (m_gasArray.at(0)->m_pSampArray->size() - 1) )
		dwNext = m_gasArray.at(0)->m_pSampArray->at(nPrevSamp + 1)->m_dwBaseTime;
	else {	// Should have been in the current buffer, but for back-tracking
		Q_ASSERT( dwTime + m_dwSampTime >= m_dwCalcTime );
		dwNext = qMin(dwBaseTime + m_dwSampTime, m_dwMaxTime);
	}

	// Loop up to the next sample time & recalc

	bool bFlush = samps[0]->m_bFlush;
	for ( dwt = dwBaseTime + m_cMSec_dx; dwt < dwNext; dwt += m_cMSec_dx ) {	// Loop over the duration of the sample
		int nv, nVernier = 0;
retry:	for ( nv = 0; nv < 1 << nVernier; ++nv ) {
			fTotUptake = 0.0F;
			for ( ng = 0; ng < ngs; ++ng ) {	// For each gas, calculate uptake and save total
#ifdef EULAR
				fUptake[ng] = CalcUptake( *samps[ng], fResults[ng] );
#else //!EULAR
				fUptake[ng] = CalcUptake( *samps[ng], fResults[ng], (*pfExp[ng])[nVernier], nVernier );
#endif //!EULAR
				fTotUptake += fUptake[ng];
			}
			for ( ng = 0; ng < ngs; ++ng ) {	// For each gas, calculate results
#ifdef EULAR
				if ( !Calc( *samps[ng], njections[ng], bFlush && ( nv == 0 ), fResults[ng], *pfEFT[ng],
				          *pfdtOEV[ng], fTotUptake, fUptake[ng], nVernier ) )
#else //!EULAR
				if ( !Calc( *samps[ng], njections[ng], bFlush && ( nv == 0 ), fResults[ng], *pfEFT[ng],
				          (*pfExp[ng])[nVernier], dwt, fTotUptake, fUptake[ng], nVernier ) )
#endif //!EULAR
				{
					++nVernier;				// we need a finer integration
					for ( ng = 0; ng < ngs; ++ng ) {	// For each gas, reset the results
						for ( i = 0, pf = m_gasArray.at(ng)->m_pfOldResults[nres]; i < MAX_RESULT; ++i )
							fResults[ng][i] = pf[i];
					}
					goto retry;
				}
			}
		}
		// point into circular queue and store results
		nres = (int)( ( dwt / m_cMSec_dx ) % SAMP_EVERY );
		for ( ng = 0; ng < ngs; ++ng ) {	// For each gas, store results
			for ( i = 0, pf = m_gasArray.at(ng)->m_pfOldResults[nres]; i < MAX_RESULT; ++i )
				pf[i] = fResults[ng][i];
			njections[ng] = 0;
		}
		bFlush = false;
	}

	// Save the new 'old' interval  

	if ( dwBaseTime <= m_dwOldStart && m_dwOldStart <= dwNext && dwNext <= m_dwOldNext ) {
		m_dwOldStart = dwBaseTime;
		if ( m_dwOldNext - m_dwOldStart > m_dwSampTime )
			m_dwOldNext = m_dwOldStart + m_dwSampTime; 
	}
	else
	if ( m_dwOldStart <= dwBaseTime && dwBaseTime <= m_dwOldNext && m_dwOldNext <= dwNext ) {
		m_dwOldNext = dwNext;
		if ( dwNext > m_dwSampTime ) {
			if ( dwNext - m_dwSampTime > m_dwOldStart )
				m_dwOldStart = dwNext - m_dwSampTime;
		}
	}
	else {
		m_dwOldStart = dwBaseTime;
		m_dwOldNext = dwNext;
	}

	// This is why we came

	i= (int)( ( dwTime / m_cMSec_dx ) % SAMP_EVERY );
 
//		 gasChildren().first()->panelView()->stop();
	 
//	while(m_gasArray.at(nGas)->m_pfOldResults[i][x]){
//		qDebug()<<"re"<<m_gasArray.at(nGas)->m_pfOldResults[i][x];
//		x++;
//	}
		 
	return m_gasArray.at(nGas)->m_pfOldResults[i];
}

 
/////////////////////////////////////////////////////////////////////////////
// Routine to calculate a set of simulation results given a previous set. It
// will step forward dt/1<<nVernier seconds, so you should call this routine
// (and the CalcUptake routine) 1<<nVernier times to advance the simulation one
// breath. If any deltas exceed x% (see code) of the max delivered setting,
// Calc returns FALSE. You must then call Calc again after resetting the
// results and doubling the vernier.

// NOTE: Results are in percent litres anesthesia (La/L*100) EXCEPT!!!
// DEL(ivered) and UPT(ake), which are in litres (La).
/////////////////////////////////////////////////////////////////////////////

// FALSE if nVernier needs doubling
bool GasDoc::Calc( GasSample &samp, int nInjections, bool bFlush, RES_ARRAY &fResults,
					COMP_ARRAY &fEffectiveFlowTo,
#ifdef EULAR
					COMP_ARRAY &fdtOverEffectiveVolume,
#else //!EULAR
					COMP_ARRAY (&fExp),
					quint32 dwTime,
#endif //!EULAR
					float fTotUptake, float fUptake, int nVernier )
{
	GasAnesthetic &anes = *m_anesArray[samp.m_wAnesID];
	bool bMyRet = true, bFixedCKT = false;
#ifdef EULAR
	COMP_ARRAY fDelta;
	float f, fVapor, fDEL, fVA, fFGF;
#else
	COMP_ARRAY fTarget;
	float f, g, fVapor, fDEL, fVA, fFGF, fBloodFlow;
	float fExpCKT = 99.0F, fExpALV = 99.0F;
#endif
	int nDiv = 1 << nVernier;

#ifdef EULAR
	// Nothing apriori...
#else //!EULAR
	Q_ASSERT( nVernier < MAX_VERNIER );
#endif //!EULAR

	fDEL = samp.m_fDEL;			// set given delivered pressure (La/L*100)
	fVA = samp.m_fVA;			// and alveolar ventilation (L/m)
	fFGF = samp.m_fFGF;			// and fresh gas flow (L/m)

	// Take into account vaporization effect on FGF, if desired.
	
	if ( samp.m_bVapEnb && anes.m_bIsLiquid ) {
		if ( fDEL < 99.0F )
			fFGF /= (100.0F - fDEL)/100.0F;				//(L/m)
		else
			fFGF *= 100.0F;
	}

	// Compute amount delivered due to vaporization

	fVapor = fDEL * fFGF * (m_fdt/nDiv) / 100.0F;	//(La)

	// Add delivered due to injection

	if ( nInjections ) {
		Q_ASSERT( anes.m_bIsLiquid );
		f = (float)anes.m_wVolatility * samp.m_fUnitDose;
		f *= (float)nInjections/1000.0F/nDiv;				//(La)
		fVapor += f;										//increase delivered
		if ( samp.m_bVapEnb )								//If desired,...
			fFGF += f/(m_fdt/nDiv);							//...correct the fresh gas flow
	}

	// Calculate effective delivered tension

	if ( fFGF > 0.0 )
		fDEL = 100.0 * fVapor / (fFGF * (m_fdt/nDiv));
	else
		fDEL = 0.0;


	// Compute effective FGF and VA

	fEffectiveFlowTo[CKT] = fFGF * anes.m_fSolubility[CKT];	//(L/m)
	fEffectiveFlowTo[ALV] = fVA  * anes.m_fSolubility[ALV];	//(L/m)

	switch ( samp.m_uCktType )
	{
	case OPEN_CKT:

		// In open circuit, delivered determines CKT pressure

		fResults[CKT] = fDEL;	//(La/L*100)
		bFixedCKT = true;
		break;

	case IDEAL_CKT:

		// In an ideal circuit, new mixture displaces pure exhaled gas with
		// no mixing, so delivered, FGF, VA and ALV determine CKT pressure.

		if ( fEffectiveFlowTo[CKT] < fEffectiveFlowTo[ALV] ) {
			f = fEffectiveFlowTo[CKT]/fEffectiveFlowTo[ALV];
			fResults[CKT] = f*fDEL + (1.0F-f)*fResults[ALV];	//(La/L*100)
		}
		else
			fResults[CKT] = fDEL;								//(La/L*100)
		bFixedCKT = true;
		break;

	default:

		// Anything else, CKT pressure is not pre-determined. Compute a rate
		// of change, as for all other compartments. Start with ambient if
		// it was flushed.

		if ( bFlush )
			fResults[CKT] = anes.m_fAmbient;

#ifdef EULAR

		// Compute litres per minute change to the breathing circuit (La/m*100)

		fDelta[CKT] = ( fDEL - fResults[CKT] ) * fEffectiveFlowTo[CKT] - ( fResults[CKT] - fResults[ALV] ) * fEffectiveFlowTo[ALV];

		// Correct for exhaled agent due to uptake effect. Again, we ignore
		// exhaled agent in ideal circuit. (NOTE: La/L = 1 in gaseous state.)

		if ( samp.m_bUptEnb && fTotUptake < 0.0 )
		//if desired and emerging, add venous effect on CKT
			fDelta[CKT] -= ( fResults[ALV] - fResults[CKT] ) * fTotUptake;
#else //!EULAR

		// Compute target tension in breathing circuit (La/L*100)

		g = fEffectiveFlowTo[CKT] + fEffectiveFlowTo[ALV];

		if ( g != 0.0F ) {
			f = fEffectiveFlowTo[CKT] * fDEL + fEffectiveFlowTo[ALV] * fResults[ALV];

			// Correct for exhaled agent due to uptake effect.
			// (NOTE: La/L = 1 in gaseous state.)

			if ( samp.m_bUptEnb && fTotUptake < 0.0F )
			//if desired and emerging, add venous effect on CKT
				f -= fTotUptake * fResults[ALV];

			fTarget[CKT] = f/g;
		}
		else
			fTarget[CKT] = fResults[CKT];
#endif //!EULAR

		break;
	}

#ifdef EULAR

	// Compute litres per minute change to alveoli (La/m*100)

	fDelta[ALV] = ( fResults[CKT] - fResults[ALV]) * fEffectiveFlowTo[ALV] - fUptake * 100.0F;

	// Correct for constant lung capacity.  NOTE: L/La is 1.0
	// in gaseous state:

	// La/L*100 * La/m = La/L*100 * La/m * L/La = La/m*100 @STP

	if ( samp.m_bUptEnb )
	//if uptake enabled
		if ( fTotUptake > 0.0F )
		//if inducing
			fDelta[ALV] += fResults[CKT] * fTotUptake;	//(La/m*100)
		else
		//else emerging
			fDelta[ALV] += fResults[ALV] * fTotUptake;	//(La/m*100)

	// Compute litres per minute to VRG, MUS, FAT (La/m*100)

	fDelta[VRG] = (fResults[ALV] - fResults[VRG]) * fEffectiveFlowTo[VRG];
	fDelta[MUS] = (fResults[ALV] - fResults[MUS]) * fEffectiveFlowTo[MUS];
	fDelta[FAT] = (fResults[ALV] - fResults[FAT]) * fEffectiveFlowTo[FAT];
#else //!EULAR
#	ifndef INCH_WORM

	// Compute target tension in alveoli (La/L*100)

	fBloodFlow = anes.m_fLambdaBG * samp.m_fCO;
	g = fEffectiveFlowTo[ALV] + fBloodFlow;

	if ( g != 0.0F ) {
		f = fEffectiveFlowTo[ALV]*fResults[CKT] + fBloodFlow*fResults[VEN];

		// Correct for constant lung capacity.  NOTE: L/La is 1.0
		// in gaseous state:

		if ( samp.m_bUptEnb )
		//if uptake enabled
		{
			if ( fTotUptake > 0.0F )
			//if inducing
				f += fResults[CKT] * fTotUptake;
			else
			//else emerging
				f += fResults[ALV] * fTotUptake;
		}

		fTarget[ALV] = f/g;
	}
	else
		fTarget[ALV] = fResults[ALV];

	// Compute target tensions for VRG, MUS, FAT (La/L*100)

	fTarget[VRG] = fResults[ALV];
	fTarget[MUS] = fResults[ALV];
	fTarget[FAT] = fResults[ALV];
#	endif //!EULAR and !INCH_WORM
#endif //!EULAR

	// TIME FOR RESULTS!

	//Add to total delivered and total uptake

	fResults[DEL] += fVapor;
	fResults[UPT] += fUptake * (m_fdt/nDiv);		//(La)

	// Compute pressure change in CKT, ALV, VRG, MUS, FAT
	// (La/m*100) * (m/L) = (La/L*100) or percent atm.

	for ( int compart = CKT; compart <= FAT; ++compart ) {
		float fOld, fChange;

		if ( compart == CKT && bFixedCKT )
		// Already computed...
			continue;

		fOld = fResults[compart];

#ifdef EULAR
		fChange = fDelta[compart] * (fdtOverEffectiveVolume[compart]/nDiv);
		fResults[compart] = fOld + fChange;
#else //!EULAR
#	ifdef INCH_WORM

		// Use current CKT to compute target tension for ALV
		// (La/L*100)

		if ( compart == CKT + 1 ) {
			fBloodFlow = anes.m_fLambdaBG * samp.m_fCO;
			g = fEffectiveFlowTo[ALV] + fBloodFlow;

			if ( g != 0.0F ) {
				f = fEffectiveFlowTo[ALV]*fResults[CKT] + fBloodFlow*fResults[VEN];

				// Correct for constant lung capacity.  NOTE: L/La is 1.0
				// in gaseous state:

				if ( samp.m_bUptEnb )
				//if uptake enabled
					if ( fTotUptake > 0.0F )
					//if inducing
						f += fResults[CKT] * fTotUptake;
					else
					//else emerging
						f += fResults[ALV] * fTotUptake;

				fTarget[ALV] = f/g;
			}
			else
				fTarget[ALV] = fResults[ALV];
		}

		// Use current ALV to compute target tensions
		// for VRG, MUS, FAT (La/L*100)

		if ( compart == ALV+1 )	{
			fTarget[VRG] = fResults[ALV];
			fTarget[MUS] = fResults[ALV];
			fTarget[FAT] = fResults[ALV];
		}
#	endif //!EULAR and INCH_WORM

		f = fExp[compart];
		switch (compart)
		{
		case CKT:
			f = fExpCKT = float(exp(f * (fEffectiveFlowTo[CKT] + fEffectiveFlowTo[ALV])));
			break;
		case ALV:
			f = fExpALV = float(exp(f * (fEffectiveFlowTo[ALV] + fBloodFlow)));
			break;
		}
		fResults[compart] =   fTarget[compart]*(1-f)
		                    + fResults[compart]*f;
		fChange = fResults[compart] - fOld;

		if ( nVernier + 1 >= MAX_VERNIER )
		// No more vernier room...
			continue;
#endif //!EULAR

		if ( !bMyRet )
		// Already flagged dubious results
			continue;

		if ( fResults[compart] < 0.0F )	{	// Don't allow negative pressure
			bMyRet = false;
			continue;
		}

		if ( fabs(fChange*400) < anes.m_fMAX_DEL )
		// Always allow delta of .25% of function range
			continue;
 
#ifdef EULAR
		if ( fChange < 0.0F ) {	// If going down...
			if ( (-20)*fChange > fOld )
			// Don't deplete more than 5% of current
				bMyRet = false;
			continue;
		}
		
		if ( 10*fChange > fabs(fDEL - fOld ) ) {	// Don't increase more than 10% of max travel
			bMyRet = false;
			continue;
		}
#endif //EULAR
	}

#ifndef EULAR
	if ( nVernier + 1 < MAX_VERNIER ) {// If there's more vernier room...
		// First compute number of breaths until we've
		// gone VERNIER_TICKS breaths past a setting change.
		int i = VERNIER_TICKS - int((dwTime - samp.m_dwBaseTime)/m_cMSec_dx);
		if ( i > 0 ) {	// If vernier hasn't already ticked away...
			if ( fExpCKT < 0.1F || fExpALV < 0.1F )
			// If influence of current value of CKT or ALV is less than 10%...
				bMyRet = false;
		}
	}
#endif //!EULAR

	// Compute venous pressure if user wants it

	if ( samp.m_bRtnEnb )
		fResults[VEN] = fResults[VRG] * m_fRatio[VRG] + fResults[MUS] * m_fRatio[MUS]
		                + fResults[FAT] * m_fRatio[FAT];
	else
		fResults[VEN] = 0.0F;           

	return bMyRet;
}

//Compute uptake rate this breath

#ifdef EULAR
inline float GasDoc::CalcUptake( GasSample &samp, RES_ARRAY &fResults )
{
	return samp.m_fCO * m_anesArray[samp.m_wAnesID]->m_fLambdaBG
		* (fResults[ALV] - fResults[VEN]) / 100.0F;	//(La/m)
}
#else //!EULAR
float GasDoc::CalcUptake( GasSample &samp, RES_ARRAY &fResults,	COMP_ARRAY &fExp, int nVernier )
{
#ifdef LINEAR_UPTAKE
	return samp.m_fCO * m_anesArray[samp.m_wAnesID]->m_fLambdaBG
	* (fResults[ALV] - fResults[VEN]) / 100.0F;	//(La/m)
#else //!LINEAR_UPTAKE
	COMP_ARRAY &pf = m_anesArray[samp.m_wAnesID]->m_fSolubility;
	float fWtFactor = samp.m_fWeight/STD_WEIGHT;
	float fDiff;
	double fUptake = 0.0;

	for ( int i = VRG; i < VEN; ++i ) {
		fDiff = fResults[ALV] - fResults[i];
		fUptake += m_fVolume[i] * pf[i] * fDiff * (1 - fExp[i]);
	}
	fUptake *= fWtFactor * (1<<nVernier) / m_fdt / 100.0F;	//(La/m)
	if ( !samp.m_bRtnEnb ) {
		fDiff = fResults[VRG] * m_fRatio[VRG] + fResults[MUS] * m_fRatio[MUS]
				+ fResults[FAT] * m_fRatio[FAT];
		fUptake += samp.m_fCO * m_anesArray[samp.m_wAnesID]->m_fLambdaBG * fDiff / 100.0F;
	}
	return (float)fUptake;
#endif //!LINEAR_UPTAKE
}
#endif //!EULAR

// This routine is called in spare time to either compute new results
// or dispatch results to the Views if we have gone far enough. It
// returns TRUE if successful, FALSE if neither is possible.
//
// Enter with dwCalcTime = time at last computation and dwNewTime
// set to the next time of interest.

bool GasDoc::ExtendResultSet()
{
	int ngs = m_gasArray.size();
	float fUptake[NGS], fTotUptake;
	int nTicks, nNext, i, ng, nv, nVernier=0;
	long dwTimeDiff;
	div_t div_result;

	Q_ASSERT( ngs <= NGS );

	// Always remove from run queue when stopping!
 	Q_ASSERT( m_theState == RUNNING_STATE ); //comment out

	if ( m_dwNextDisplayTime != 0L && m_dwNextDisplayTime <= m_dwCalcTime) { // We have calculated to a point that the user is interested in
		CallViews();
		return true;
	}
	
	// If settings are historical
	if ( m_dwTime < m_dwHighTime || m_dwCalcTime > m_dwMaxTime )	// or we're at the end of time
		return false;												// ...we cannot do anything

	if ( GetCurrentSpeed() != AFAP ) {		// If we are not going as fast as possible...
		//How far ahead of where we need to be are we?
		dwTimeDiff =  (long)m_dwCalcTime;
		if ( m_dwNextDisplayTime != 0L )
			dwTimeDiff -= (long)m_dwNextDisplayTime;
		else
			dwTimeDiff -= (long)m_dwTime;
		// a full buffer
	    if ( dwTimeDiff >= (long)m_dwSampTime || dwTimeDiff/GetSpeed() >= 5L*60L*1000L)	// or 5 minutes
			return false;																// ...is far enough
	}

	// Compute ticks since t0

	nTicks = (int)(m_dwCalcTime/m_cMSec_dx);

	// Compute samples since t0 and ticks left over

	div_result = div(nTicks, SAMP_EVERY);
	nTicks = div_result.rem;

	nNext  = (nTicks+1) % SAMP_EVERY;	// point to next slot

	//if @ even SAMP_EVERY boundary, save a pointer

	if ( !nTicks )
		m_nSampIdx[div_result.quot] = (short)m_gasArray.at(0)->m_pSampArray->size();

	for ( ng = 0; ng < ngs; ++ng ) {
		Gas *g = m_gasArray.at(ng);

		// Start a new sample block if necessary

		if ( g->m_pCurSamp == 0 || !nTicks )	{	//if we should start a new sample block
			g->m_pCurSamp = new GasSample( g, g->m_pfCurResults[nTicks] );
			Q_ASSERT( g->m_pCurSamp != 0 );
			g->m_pSampArray->append( g->m_pCurSamp );
			if( g->m_pCurSamp->m_dwBaseTime == 0 )
				g->SetInitialSamp( new GasSample(*g->m_pCurSamp) );
			m_nLastSampAt = -1;					//Invalidate cache
		}

		// Check the terms (Always!)

#ifdef EULAR
		if ( g->m_nTermAgent != g->m_nAgent	|| g->m_fTermCO != m_fCO
	    	|| g->m_fTermWeight != m_fWeight )
		{
			ComputeTerms( m_fCO, m_fWeight, g->m_nAgent, g->m_fEffectiveFlowTo, g->m_fdtOverEffectiveVolume );
			g->m_nTermAgent = g->m_nAgent;		// Remember where these came from
			g->m_fTermCO = m_fCO;
			g->m_fTermWeight = m_fWeight;
		}
#else //!EULAR
		if ( g->m_nTermAgent != g->m_nAgent || g->m_fTermFGF != m_fFGF
		    || g->m_fTermVA != m_fVA || g->m_fTermCO != m_fCO || g->m_fTermWeight != m_fWeight )
		{
			ComputeTerms( m_fFGF, m_fVA, m_fCO, m_fWeight, g->m_nAgent, g->m_fEffectiveFlowTo, g->m_fExp );
			g->m_nTermAgent = g->m_nAgent;		// Remember where these came from
			g->m_fTermFGF = m_fFGF;
			g->m_fTermVA = m_fVA;
			g->m_fTermCO = m_fCO;
			g->m_fTermWeight = m_fWeight;
		}
#endif //!EULAR
	}

	// Step forward

retry:
	for ( ng=0; ng < ngs; ++ng ) {	// For each gas, copy the starting point to the next result slot
		Gas *g = m_gasArray.at(ng);

		for ( i = 0; i < MAX_RESULT; ++i )
			g->m_pfCurResults[nNext][i] = g->m_pfCurResults[nTicks][i];
	}
	for ( nv = 0; nv < 1 << nVernier; ++nv ) {
		fTotUptake = 0.0F;
		for ( ng=0; ng < ngs; ++ng ) {		// For each gas, get uptake rate and add to total uptake
			Gas *g = m_gasArray.at(ng);
#ifdef EULAR
			fUptake[ng] = CalcUptake( *g->m_pCurSamp, g->m_pfCurResults[nNext] );
#else //!EULAR
			fUptake[ng] = CalcUptake( *g->m_pCurSamp, g->m_pfCurResults[nNext], g->m_fExp[nVernier], nVernier );
#endif //!EULAR
			fTotUptake += fUptake[ng];
		}
		for ( ng = 0; ng < ngs; ++ng ) {
			Gas *g = m_gasArray.at(ng);

#ifdef EULAR
			if ( !Calc( *g->m_pCurSamp, g->m_nInjections, m_bFlush && ( nv == 0 ),
			          g->m_pfCurResults[nNext], g->m_fEffectiveFlowTo,
			          g->m_fdtOverEffectiveVolume, fTotUptake, fUptake[ng], nVernier ) )
#else //!EULAR
			if ( !Calc( *g->m_pCurSamp, g->m_nInjections, m_bFlush && ( nv == 0 ),
			          g->m_pfCurResults[nNext], g->m_fEffectiveFlowTo, g->m_fExp[nVernier], m_dwCalcTime,
			          fTotUptake, fUptake[ng], nVernier ) )
#endif //!EULAR
			{
 				nVernier++;	// we need a finer integration
 				goto retry;
 			}
 		}
 	}
 	gTotVernier[nVernier] += 1;
 	
	// Count any injection and flushing we just did

	for ( ng=0; ng < ngs; ++ng ) {
		m_gasArray.at(ng)->m_nInjections = 0;
	}
	m_bFlush = false;

	// We did it!

	m_dwCalcTime += m_cMSec_dx;
	if ( m_dwCalcStart + m_dwSampTime <= m_dwCalcTime )
		// if one fell off the end of the buffer...
		m_dwCalcStart = m_dwCalcTime+m_cMSec_dx - m_dwSampTime;

	// If we just calculated a value at which we should have set a break, and it comes
	// before our current target time, set the preemptive break now. (Not even sure this
	// can happen on a modern computer -- caclulation always gets ahead of display)

	if ( m_dwNextDisplayTime!=0L && m_dwCalcTime < m_dwNextDisplayTime && shouldBreakAtTime(m_dwCalcTime) )
	{
		m_bBreak = true;
		m_dwNextDisplayTime = m_dwCalcTime;
		CallViews();
	}

	return true;
}

// When we hit the new time set above, call the views to update them
void GasDoc::CallViews()
{
	// Make sure we've gotten that far

	Q_ASSERT( m_dwNextDisplayTime != 0L && m_dwNextDisplayTime < m_dwCalcTime+m_cMSec_dx );
	quint32 dwPrev = m_dwTime;
	m_dwTime = m_dwNextDisplayTime;
	m_dwNextDisplayTime = 0L;

	// Process any intervening events when crossing breath boundaries

	if ( dwPrev/m_cMSec_dx != m_dwTime/m_cMSec_dx ) {
		 
		GasSample& samp = *m_gasArray.at(0)->m_pSampArray->at(SampAt(m_dwTime));
		//weight change (rediculous!)
		if ( m_fWeight != samp.m_fWeight )
			SetWeight( 0, samp.m_fWeight );
		//circuit change
		switch ( samp.m_uCktType )
		{
		case OPEN_CKT:
			if (m_strCircuit[0] != tr( "Open", "open circuit" ).at(0) )
				SetCircuit( 0, tr( "Open", "open circuit" ) );
			break;
		case SEMI_CKT:
			if (m_strCircuit[0] != tr( "Semi-closed" ).at(0) )
				SetCircuit( 0, tr( "Semi-closed" ) );
			break;
		case CLOSED_CKT:
			if (m_strCircuit[0] != tr( "Closed" ).at(0) )
				SetCircuit(0, tr( "Closed" ) );
			break;
		case IDEAL_CKT:
			if (m_strCircuit[0] != tr( "Ideal" ).at(0) )
				SetCircuit(0, tr( "Ideal" ) );
			break;
		default:
			Q_ASSERT(false);
		}
		//FGF, VA, CO changes
		if (m_fFGF != samp.m_fFGF) SetFGF(0, samp.m_fFGF);
		if (m_fVA  != samp.m_fVA ) SetVA(0, samp.m_fVA );
		if (m_fCO  != samp.m_fCO ) SetCO(0, samp.m_fCO );
		//Return enabled change
		m_bRtnEnb   = samp.m_bRtnEnb;
		//Uptake enabled change
		m_bUptEnb   = samp.m_bUptEnb;
		//Vaporization enabled change
		m_bVapEnb   = samp.m_bVapEnb;

		for ( int ng = 0; ng < m_gasArray.size(); ++ng ) {
			Gas *g = m_gasArray.at(ng);
			GasSample& samp = *g->m_pSampArray->at(SampAt(m_dwTime));

			//DEL change
			if (g->m_fDEL != samp.m_fDEL) SetDEL(0, ng, samp.m_fDEL);
			//agent change
			if (g->m_nAgent != (int)samp.m_wAnesID)
				SetAgent(0, ng, m_anesArray[samp.m_wAnesID]->m_strName);
			//unit dose change
			g->m_fUnitDose = samp.m_fUnitDose;

			// Store the results conveniently
	
			RES_ARRAY & fResults = ResultsAt(m_dwTime, ng);
			for ( int i = 0; i < MAX_RESULT; ++i )
				g->m_fResults[i] = fResults[i];
		}
	}

	if (m_dwTime > m_dwHighTime) {		//if we've broken new ground
		m_dwHighTime = m_dwTime;		// Reset user's view of 'end'
		emit modified();
	}

	// Finally.  Update the world, but don't do any display work
	// more often than once a second.

	if (dwPrev/1000L != m_dwTime/1000L)
		UpdateAllViews( 0, 0, GasUpdateEvent::UPD_TICK );

	// If we're the view of the minute, beep now

	if ( m_nBeep > 0 && GetCurrentSpeed() < 20 && m_dwTime % 60000L == 0L) {
		//for ( QWidget *wnd = QApplication::focusWidget(); wnd; wnd = wnd->parentWidget() ) {
			//if ( childWindows.contains( qobject_cast<GasChildWindow *>( wnd ) ) ) {
			if ( childWindows.contains( gasMainWindow->activeChild() ) ) {
				if ( m_nBeep == 1 )
					QApplication::beep();
				else {
					QString path = gasApp->ReadProfile( "Signals", QString( "System_%1" ).arg( m_nBeep - 1 ), QString() ).toString();
					if ( QFile::exists( path ) ) QSound::play( path );
				}
			}
		//}
	}
	
	// Change the state if at breakpoint or end of sim. Else,
	// see if there's room for more calcs.

	if ( m_dwTime >= m_dwMaxTime )
		ChangeState( 0, END_STATE );
	else if ( m_bBreak )
		ChangeState( 0, STOPPED_STATE );
	else if ( m_theState == RUNNING_STATE )
		RunOnIdle();
}

// Step the simulation ahead to the passed number of milliseconds,
// or, if the passed number is negative, step to the latest
// calculated time. A request to step beyond max hours is the
// equivalent of a call to stop the simulation.  A request to
// step past a bookmark is equivalent to a request to go to the
// bookmark and stop.

void GasDoc::StepSimulation( long lMSec ) 
{
	int	i, j;

#	define NMIN 15
	 
	quint32 dw = m_dwTime + NMIN * 60L * 1000L;

	// Want the maximum?
	if ( lMSec < 0 )
		// Yes, get whatever we have, but at least N minutes
		m_dwNextDisplayTime = qMax( dw, qMax( m_dwHighTime, m_dwCalcTime ) );
	else
		// No, get time requested (up to N min from last)
		m_dwNextDisplayTime = qMin( (quint32)lMSec, dw);

#undef NMIN
	 
	if ( m_dwNextDisplayTime > m_dwMaxTime )			// At end of the line?
		m_dwNextDisplayTime = m_dwMaxTime;				// max out.

	// If we're going less than 20X, stop to make the
	// (annoying) click when crossing minute boundary
	if ( lMSec >=0 && GetCurrentSpeed() < 20 ) {
		dw = m_dwTime/60000L;  // the last end time;
		if ( dw < m_dwNextDisplayTime/60000L )
			m_dwNextDisplayTime = (dw+1L) * 60000L;
	}

	 
	// See if we hit a bookmark

	m_bBreak = false;						// assume no
	if(lMSec > 0 && isCondition()){
			m_bBreak = true;
	}

	for ( i = 0; i < m_nBookmarks; ++i ) {
		dw = m_dwBookmarks[i];
		if ( m_dwTime < dw && m_dwNextDisplayTime >= dw ) {
			m_bBreak = true;
		 	m_dwNextDisplayTime = dw;
			break;
		}
	}
	 
	for ( i = 0; i < m_nTempmarks; ++i ) {
		dw = m_dwTempmarks[i];
		if ( m_dwTime < dw && m_dwNextDisplayTime >= dw ) {
			m_bBreak = true;
			m_dwNextDisplayTime = dw;
			break;
		}
	}
	
	// There is an implied bookmark at the end of the sim
	 
	if ( m_dwTime < m_dwHighTime && m_dwNextDisplayTime >= m_dwHighTime ) {
		m_bBreak = true;
		m_dwNextDisplayTime = m_dwHighTime;
	}

	Q_ASSERT( m_dwNextDisplayTime >= m_dwTime );				 // Getting nowhere?

	// Get current %MAC for compartments with targets

	QList<int> candidates;
	for ( i = 0; i < MAX_COMPART; ++i )
		if ( m_targetLists[i].count() > 0 )
			candidates.append(i);
	QVector<float> currentPercents = pctMacForTargetsAtTime(candidates, m_dwTime);

	// Get the list of %MAC targets by compartment

	m_targetCompartments.clear();
	for ( i = 0; i < MAX_COMPART; ++i )
	{
		float current = currentPercents[i];
		QList<int> bkpList = m_targetLists[i];
		float low = std::numeric_limits<float>::lowest();
		float high = std::numeric_limits<float>::max();
		bool bTargeted = false;

		if ( bkpList.count() > 0 )
		{
			for (j = 0; j < bkpList.count() && bkpList[j] <= current; ++j);
			if (j < bkpList.count()) { high = bkpList[j]; bTargeted = true; }
			for (--j; j >= 0 && bkpList[j] >= current; --j);
			if (j >= 0) { low = bkpList[j]; bTargeted = true; }
		}
		m_target[i][0] = low;
		m_target[i][1] = high;
		if (bTargeted)
			m_targetCompartments.append(i);
	}

	// Loop through any history we have from m_dwTime to m_dwNewTime and check to see
	// if we hit any targets first. If so, set m_bBreak and constrain m_dwNewTime to the
	// time the target is hit
	if (m_targetCompartments.count() > 0)
	{
		quint32 dwHistorySearchLimit = qMin(m_dwNextDisplayTime, m_dwCalcTime + m_cMSec_dx);
		for ( quint32 t = m_cMSec_dx * (m_dwTime/m_cMSec_dx + 1); t < dwHistorySearchLimit; t += m_cMSec_dx )
			if (shouldBreakAtTime(t))
			{
				m_dwNextDisplayTime = t;
				m_bBreak = true;
				break;
			}
	}

	// If we have the requested numbers, send them out
	 
	if ( m_dwNextDisplayTime < m_dwCalcTime + m_cMSec_dx )
		 	CallViews();
	else {
		// Must be running to see the future...
	 	Q_ASSERT( m_theState == RUNNING_STATE );  //comment out
		// Make sure we are in the run queue
		RunOnIdle();
	}
}

QVector<float> GasDoc::pctMacForTargetsAtTime(QList<int>& targets, quint32 dwTime)
{
	QVector<float> pctMac(MAX_COMPART, 0.0f);
	int ngs = m_gasArray.size();

	for (int ng = 0; ng < ngs; ++ng) {
		RES_ARRAY& results = ResultsAt(dwTime, ng);
		for (int i = 0; i < targets.count(); ++i)
		{
			int compartment = targets[i];
			pctMac[compartment] += m_gasArray[ng]->asMacPercent(results[compartment]);
		}
	}

	return pctMac;
}

bool GasDoc::shouldBreakAtTime(quint32 dwTime)
{
	QVector<float> percents = pctMacForTargetsAtTime(m_targetCompartments, dwTime);
	for (int i = 0; i < m_targetCompartments.count(); ++i)
	{
		int compartment = m_targetCompartments[i];
		float percent = percents[compartment];
		float(&targets)[2] = m_target[compartment];
		if (percent <= targets[0] || percent >= targets[1])
			return true;
	}
	return false;
}

bool GasDoc::isCondition()
{	
	float op1, op2;
//	int j = 0;
	QString name;
	if(op.size()>0){
		name = op.at(0);
		op1 = getOp(name);
		for(int i =1; i< op.size(); i++){
			name = op.at(i);
			op2 = getOp(name);
			name = si.at(i-1);
			if(name == "+")
				op1 += op2;
			else if(name =="-")
				op1 -= op2;
			else if(name == "*")
				op1 *= op2;
			else if(name == "/" && op2!= 0)
				op1 /= op2;
			else 
				return false;
		}

		 
		if(op1 ==expv){
			oldval =op1;	
			return true;
		}
		else if( op1 >expv && oldval < expv ){
			oldval = op1;
			return true;
		}else if(op1< expv && oldval > expv){
			oldval = op1;
			return true;
		}else if(fabs(op1-expv)< 0.0001){
			oldval = op1;
			return true;
		}

		
	
	}
	oldval = op1;
		 
	return false;
}
float GasDoc::getOp(QString & op)
{
	//Op<<"FGF"<<"DEL"<<"CKT"<<"ALV"<<"MUS"<<"FAT";
	if(op == "FGF")
		return GetFGF();
	else if(op =="DEL")
		return GetDEL(0);
	else if(op =="CKT")
		return GetCKT(0);
	else if(op =="ALV")
		return GetALV(0);
	else if(op == "MUS")
		return GetMUS(0);
	else if(op =="FAT")
		return GetFAT(0);
	else if(op =="VRG")
		return GetVRG(0);
	Q_ASSERT(false);
	return 0.0F;
}

//The following overloaded versions of GetXXX retrieve values
//for a particular time.

float GasDoc::GetFGF( float fMin, int nGas )
{
	quint32 dwt = GetDwTime( fMin );

	if ( dwt >= m_dwCalcTime ) { // If at the current time
		if ( m_bFlush) return 30.0F;
		GasAnesthetic& anes = *m_anesArray[m_gasArray.at(nGas)->m_nAgent];
		if ( m_bVapEnb && anes.m_bIsLiquid )
			return m_fFGF * 100.0F / ( 100.0F - qMin( m_gasArray.at( nGas )->m_fDEL, 99.0F ) );
		else
			return m_fFGF;
	}
	// Else in history
	GasSample& samp = *(*m_gasArray.at(0)->m_pSampArray)[SampAt(dwt)];
	GasAnesthetic& anes = *m_anesArray[samp.m_wAnesID];
	if ( samp.m_bFlush && samp.m_dwBaseTime + m_cMSec_dx > dwt ) return 30.0F;
	if ( samp.m_bVapEnb && anes.m_bIsLiquid )
		return samp.m_fFGF * 100.0F / ( 100.0F - qMin( samp.m_fDEL, 99.0F ) );
	else
		return samp.m_fFGF;
}

float GasDoc::GetDEL( float fMin, int nGas, bool *pbInjected, float fLastMin )
//ASSUMES anes.m_fSolubility[CKT] is 1!!
{
	quint32 dwt = GetDwTime( fMin );
	float f;

	if ( dwt >= m_dwCalcTime ) { // If at the current time
		if ( pbInjected != 0 )
			*pbInjected = m_gasArray.at( nGas )->m_nInjections;
		if ( m_bFlush ) return 0.0F;
		if ( dwt < m_cMSec_dx ) return m_gasArray.at( nGas )->m_fDEL;
		quint32 dwtPrev = dwt - m_cMSec_dx;
		GasSample& sampPrev = *(*m_gasArray.at( nGas )->m_pSampArray)[SampAt(dwtPrev)];
		if ( sampPrev.m_fFGF == 0.0F ) return m_gasArray.at(nGas)->m_fDEL;
		return (m_gasArray.at(nGas)->m_fResults[DEL] - ResultsAt(dwtPrev, nGas)[DEL])
		        *100.0F/GetFGF(dwtPrev/60000.0F, nGas)/m_fdt;
	}
	// Else in history
	int nSamp = SampAt(dwt);
	if ( pbInjected != 0 )
	{
		*pbInjected = false;
		if (fLastMin >= 0.0F && fLastMin != fMin)
		{
			int n1, n2;
			quint32 dwt1, dwt2;

			if (fLastMin < fMin){
				dwt1 = GetDwTime(fLastMin);
				//dwt1 = (fLastMin==fMin && dwt >= m_cMSec_dx) ? dwt - m_cMSec_dx : GetDwTime(fLastMin);
				dwt2 = dwt;
				n1 = SampAt(dwt1);
				n2 = nSamp;
			} else if (fLastMin > fMin) {
				dwt1 = dwt;
				dwt2 = GetDwTime(fLastMin);
				n1 = nSamp;
				n2 = SampAt(dwt2);
			}
                else {
                    n1 = 1; n2 = 0; // (just to keep compiler happy; not executed)
            }

			for (int n = n1; !*pbInjected && n <= n2; n++)
            {
				GasSample& samp = *(*m_gasArray.at( nGas )->m_pSampArray)[n];
				*pbInjected = samp.m_uInjections != 0 && samp.m_dwBaseTime >= dwt1 && samp.m_dwBaseTime < dwt2;
			}
		}
	}
	GasSample& samp = *(*m_gasArray.at( nGas )->m_pSampArray)[nSamp];
	if ( samp.m_bFlush && samp.m_dwBaseTime+m_cMSec_dx > dwt ) return 0.0F;
	if ( dwt < m_cMSec_dx ) return samp.m_fDEL;
	quint32 dwtPrev = dwt - m_cMSec_dx;
	GasSample& sampPrev = *(*m_gasArray.at(nGas)->m_pSampArray)[SampAt(dwtPrev)];
	if ( sampPrev.m_fFGF == 0.0F ) return samp.m_fDEL;
	f = ResultsAt( dwtPrev, nGas )[DEL];
	return ( ResultsAt( dwt, nGas )[DEL] - f )
	       *100.0F/GetFGF( dwtPrev/60000.0F, nGas )/m_fdt;
}

float GasDoc::GetVA( float fMin )
{
	quint32 dwt = GetDwTime( fMin );

	if ( dwt >= m_dwCalcTime ) { // If at the current time
		if ( dwt < m_cMSec_dx || !m_bUptEnb ) return m_fVA;
		quint32 dwtPrev = dwt - m_cMSec_dx;
		float fTot = 0.0F;
		for ( int i = 0; i < m_gasArray.size(); ++i ) {
			float f = ResultsAt( dwtPrev, i )[UPT];
			fTot += ( m_gasArray.at(i)->m_fResults[UPT] - f );
		}
		return fTot/m_fdt + m_fVA;
	}
	// Else in history
	GasSample& samp = *(*m_gasArray.at(0)->m_pSampArray)[SampAt(dwt)];
	if ( dwt < m_cMSec_dx || !samp.m_bUptEnb ) return samp.m_fVA;
	quint32 dwtPrev = dwt - m_cMSec_dx;
	float fTot = 0.0F;
	for ( int i = 0; i < m_gasArray.size(); ++i ) {
		float f = ResultsAt( dwtPrev, i )[UPT];
		fTot += ( ResultsAt( dwt, i )[UPT] - f);
	}
	return fTot/m_fdt + samp.m_fVA;
}

float GasDoc::GetCO( float fMin )
{
	quint32 dwt = GetDwTime( fMin );
	if ( dwt >= m_dwCalcTime ) return m_fCO;
	return (*m_gasArray.at(0)->m_pSampArray)[SampAt(dwt)]->m_fCO;
}

float GasDoc::GetCKT( float fMin, int nGas )
{
	quint32 dwt = GetDwTime( fMin );
	if ( dwt >= m_dwCalcTime )
		return m_gasArray.at( nGas )->m_fResults[CKT];
	return ResultsAt( dwt, nGas )[CKT];
	
}

float GasDoc::GetALV( float fMin, int nGas )
{
	quint32 dwt = GetDwTime( fMin );
	if ( dwt >= m_dwCalcTime )
		return m_gasArray.at( nGas )->m_fResults[ALV];
	return ResultsAt( dwt, nGas )[ALV];
}

float GasDoc::GetART( float fMin, int nGas )
{
	quint32 dwt = GetDwTime( fMin );
	if ( dwt >= m_dwCalcTime )
		return m_gasArray.at( nGas )->m_fResults[ALV];
	return ResultsAt( dwt, nGas )[ALV];
}

float GasDoc::GetVRG( float fMin, int nGas )
{
	quint32 dwt = GetDwTime( fMin );
	if ( dwt >= m_dwCalcTime )
		return m_gasArray.at( nGas )->m_fResults[VRG];
	return ResultsAt( dwt, nGas )[VRG];
}

float GasDoc::GetMUS( float fMin, int nGas )
{
	quint32 dwt = GetDwTime( fMin );
	if ( dwt >= m_dwCalcTime )
		return m_gasArray.at( nGas )->m_fResults[MUS];
	return ResultsAt( dwt, nGas )[MUS];
}

float GasDoc::GetFAT(float fMin, int nGas)
{
	quint32 dwt = GetDwTime( fMin );
	if ( dwt >= m_dwCalcTime )
		return m_gasArray.at( nGas )->m_fResults[FAT];
	return ResultsAt( dwt, nGas )[FAT];
}                       

float GasDoc::GetVEN( float fMin, int nGas )
{
	quint32 dwt = GetDwTime( fMin );
	if ( dwt >= m_dwCalcTime )
		return m_gasArray.at( nGas )->m_fResults[VEN];
	return ResultsAt( dwt, nGas )[VEN];
}

float GasDoc::GetCostRatio(int nGas)
{
	GasAnesthetic& anes = *m_anesArray[m_gasArray.at(nGas)->m_nAgent];
	float fBotVol = anes.m_nBottleSize * (float)anes.m_wVolatility;
	if ( fBotVol == 0.0F ) return 0.0F;
	return 1000.0F * anes.m_fBottleCost/fBotVol;
}

float GasDoc::GetUptake( float fMin, int nGas )
{
	quint32 dwt = GetDwTime( fMin );
	if (dwt >= m_dwCalcTime)
		return m_gasArray.at( nGas )->m_fResults[UPT];
	else
		return ResultsAt( dwt, nGas )[UPT];
}

float GasDoc::GetUptakeCost( float fMin, int nGas )
{
	float f;
	float fRatio = GetCostRatio( nGas );
	if ( fRatio == 0.0F ) return 0.0F;

	quint32 dwt = GetDwTime( fMin );
	if ( dwt >= m_dwCalcTime )
		f = m_gasArray.at( nGas )->m_fResults[UPT];
	else
		f = ResultsAt( dwt, nGas )[UPT];
	return f*fRatio;
}

float GasDoc::GetDelivered( float fMin, int nGas )
{
	quint32 dwt = GetDwTime( fMin );
	if ( dwt >= m_dwCalcTime )
		return m_gasArray.at( nGas )->m_fResults[DEL];
	else
		return ResultsAt( dwt, nGas )[DEL];
}

float GasDoc::GetDeliveredCost( float fMin, int nGas )
{
	float f;
	float fRatio = GetCostRatio( nGas );
	if ( fRatio == 0.0F ) return 0.0F;

	quint32 dwt = GetDwTime( fMin );
	if ( dwt >= m_dwCalcTime )
		f = m_gasArray.at( nGas )->m_fResults[DEL];
	else
		f = ResultsAt( dwt,nGas )[DEL];
	return f*fRatio;
}

void GasDoc::SetAgent( QWidget *that, int nGas, const QString &agent )
{
	Gas *g = m_gasArray.at( nGas );

	g->SetAgent( agent );

	UpdateAllViews( that, nGas+1, GasUpdateEvent::UPD_AGENT );

	if ( that == 0 ) return;			// if changed internally, that's it

	// User change -- change dependent settings

	Truncate();							//disregard any future events

	SetDEL( (QWidget *)-1L, nGas, m_anesArray[g->m_nAgent]->m_fDefDEL );

	if ( m_dwTime == 0L ) {
		g->SetInitialResults();
		UpdateAllViews( 0, nGas+1, GasUpdateEvent::UPD_TICK );
	}
	emit agentChanged(nGas, agent);
}

void GasDoc::AddAgent(const QString agent)
{
	Q_ASSERT( m_dwTime == 0L );

	Truncate();
	m_nActiveGas = m_gasArray.size();
	m_gasArray.append( new Gas( this ) );

	emit modified();

	SetAgent((QWidget *)-1L, m_nActiveGas, agent);

	AddDefaultAgentColor(agent);
 	emit agentColorsChanged();

	updateTitle();
}

void GasDoc::AddDefaultAgentColor(const QString &agent)
{
	colors << getDefaultAgentColor(agent);
}

QString GasDoc::getDefaultAgentColor(const QString &agent)
{
	int i = m_anesArray.Lookup(agent);
	if( i >= 0 )
	{
		QString color = m_anesArray.at(i)->m_defaultColor;
		if( !color.isEmpty() )
			return color;
	}
	return gasApp->getNextAvailableColor( this );
}

QString GasDoc::getDefaultAgentColor(const int nGas) const
{
	QString color = m_anesArray.at(m_gasArray.at(nGas)->m_nAgent)->m_defaultColor;
	if( !color.isEmpty() )
		return color;
	return gasApp->getNextAvailableColor( this );
}

GasChildren GasDoc::DeleteAgent(int gasNum)
{
	Q_ASSERT( m_dwTime == 0L );

	Truncate();
	m_gasArray.removeAt(gasNum);

	colors.removeAt(gasNum);

	// Shift all the gas numbers above gasNum down, and remember
	// the views of gasNum to delete
	GasChildren deleteList;
	foreach(GasChildWindow* child, gasChildren())
	{
		if (child->getViewType() == CONTROL)
			continue;
		int nGas;
		if ( !child->tryGetGas(nGas) || nGas < gasNum )
			continue;
		if (nGas > gasNum)
			nGas -= 1;
		else {
			nGas = -1;	//(to make others barf)
			deleteList << child;
		}
		child->resetGas(nGas);
	}

	emit modified();
	emit agentColorsChanged();
	return deleteList;
}

void GasDoc::SetCircuit( QWidget *that, const QString &circuit )
{
	float fFGF;
	
	m_strCircuit = circuit;

	UpdateAllViews( that, 0, GasUpdateEvent::UPD_CIRCUIT );

	if ( that == 0 ) return;			// if changed internally, that's it

	// User change -- change dependent settings

	Truncate();							//disregard any future events
	if ( circuit[0] == tr( "Open", "open circuit" ).at(0) ) // Open
		fFGF = qMax( 10.0F, m_fVA );
	else if ( circuit[0] == tr( "Semi-closed" ).at(0) || circuit[0] == tr( "Ideal" ).at(0) ) // Semi-closed or Ideal
		fFGF = m_dwTime == 0L? m_fDfltFGF : m_fDfltFGF * SEMI_FGF/INIT_SEMI_FGF;
	else if ( circuit[0] == tr( "Closed" ).at( 0 ) )	{		// Closed
		double factor = (double)m_fWeight/STD_WEIGHT;
		factor = sqrt(sqrt(factor*factor*factor));
		fFGF = (float)(STD_FGF*factor);
	} else
		fFGF = m_fFGF;
		
	if ( fFGF != m_fFGF )
		SetFGF( (QWidget *)-1L, fFGF );
}

void GasDoc::SetWeight( QWidget *that, const float &fWeight )
{
	double factor = (double)fWeight/m_fWeight;
	m_fWeight = fWeight;

	UpdateAllViews( that, 0, GasUpdateEvent::UPD_WEIGHT );

	if ( that == 0 ) return;			// if changed internally, that's it

	// User change -- change dependent settings

	Truncate();							//disregard any future events

	factor = sqrt(sqrt(factor*factor*factor));
	SetVA( (QWidget *)-1L, (float)(m_fVA*factor));
	SetCO( (QWidget *)-1L, (float)(m_fCO*factor));

	if ( m_strCircuit[0] == tr( "Closed" ).at( 0 ) ) {
		factor = (double)m_fWeight/STD_WEIGHT;
		factor = sqrt(sqrt(factor*factor*factor));
		SetFGF((QWidget *)-1L, (float)(STD_FGF*factor));
	}

	// At the beginning of the sim, we can alometrically change
	// breathing rate.

	if ( m_dwCalcTime != 0L ) return;

	factor = 10.0F * sqrt(sqrt(m_fWeight)) / 28.92508F;
	SetTimeVariables((quint16)(DT * factor + 0.5F));
}

void GasDoc::SetSpeed( QWidget *that, const short &speed )
{
	m_nSpeed = speed;
	
	UpdateAllViews( that, 0, GasUpdateEvent::UPD_SPEED );
}

void GasDoc::SetVA( QWidget *that, const float &fVA )
{
	m_fVA = fVA;

	UpdateAllViews( that, 0, GasUpdateEvent::UPD_VA );

	if ( that == 0 ) return;			// if changed internally, that's it

	// User change -- change dependent settings

	Truncate();							//disregard any future events

	if ( m_fFGF < fVA && m_strCircuit[0] == tr( "Open", "open circuit" ).at(0) )
	// Can't breath more than FGF in open circuit
		SetFGF( (QWidget *)-1L, fVA );
}

void GasDoc::SetCO( QWidget *that, const float &fCO )
{
	m_fCO = fCO;

	UpdateAllViews( that, 0, GasUpdateEvent::UPD_CO );

	if ( that != 0 )					//if changed by user and not internally...
		Truncate();						//disregard any future events
}

void GasDoc::SetDEL( QWidget *that, int nGas, const float &fDEL )
{
	Gas *g = m_gasArray.at(nGas);

	g->SetDEL(fDEL);
	UpdateAllViews( that, nGas+1, GasUpdateEvent::UPD_DEL );
	if ( that != 0 )					//if changed by user and not internally...
		Truncate();						//disregard any future events
}

void GasDoc::SetFGF( QWidget *that, const float &fFGF )
{
	m_fFGF = fFGF;
	
	UpdateAllViews( that, 0, GasUpdateEvent::UPD_FGF );

	if (that == 0) return;			// if changed internally, that's it

	// User change -- change dependent settings

	Truncate();							//disregard any future events

	// Setting FGF to zero on non-closed circuit means
	// 'please set closed circuit'

	if ( fFGF==0.0F && m_strCircuit[0] != tr( "Closed" ).at( 0 ) )
		SetCircuit( (QWidget *)-1L, tr( "Closed" ) );

	// Clamp VA below FGF in Open Circuit

	if (m_fVA > fFGF && m_strCircuit[0] == tr( "Open", "open circuit" ).at(0) )
		SetVA( (QWidget *)-1L, fFGF );
}

void GasDoc::Inject()
{
	m_gasArray.at(m_nActiveGas)->Inject();
}

void GasDoc::Flush()
{
	int ng;

	m_bFlush = true;
	for ( ng = 0; ng < m_gasArray.size(); ++ng )
		m_gasArray.at(ng)->Flush();
	
	UpdateAllViews( 0, 0, GasUpdateEvent::UPD_TICK );
}
/*
This function informs each view attached to the document, except for the 
view specified by pSender, that the document has been modified
*/
void GasDoc::UpdateAllViews( QWidget *pSender, int info, GasUpdateEvent::AnEvent type )
{
	 
	emit updateControl();
 
	foreach ( GasChildWindow *child, childWindows ) {
		
		GasPanelView *panelView = child->panelView();
		GasGraphView *graphView = child->graphView();

		if ( !panelView || !graphView ) return;

		QWidget *receiver = 0;

		if ( pSender == panelView )
			receiver = graphView;
	
		if ( pSender == graphView )
			receiver = panelView;
	
		GasUpdateEvent evnt( type, info, this );
	
		if ( receiver ) {
			qApp->sendEvent( receiver, &evnt );
		}
		else {
			qApp->sendEvent( panelView, &evnt );
			qApp->sendEvent( graphView, &evnt );
		}

	} 
}

/////////////////////////////////////////////////////////////////////////////
// GasDoc serialization

// The cMSec_dx variable has a known range (around 6000 miliseconds, or one
// breath per calculation). We therefore can use an UNreasonable value as a
// flag to signify that the document was saved with a version.

// DEPRECATED: Older scheme. m_SerializationFlags have been read from new
// file header if SER_NEW_HEADER is set 

void GasDoc::SerializeMSec_dx( QDataStream &stream, bool storing  )
{
	const quint16 wVerFlag = 65535;
	quint16 wVersion = 6, w = 0;
	
	if (storing) {

		stream << wVerFlag;		// New save is always version tagged
		stream << wVersion;		// Store the latest version
		Q_ASSERT( childWindows.size() );

		GasPanelView *pPanel = childWindows[0]->panelView();

		GasPanelView *pFirstPanel = NULL;
		foreach ( GasChildWindow *child, childWindows ) {
			if( pFirstPanel == NULL && child->getViewType() == PANEL )
				pFirstPanel = child->panelView();
		}

		stream << m_cMSec_dx;
		stream << (quint16)w;						//unused
		stream << (quint16)m_nActiveGas;			//only used as flag
		stream << (quint16)pPanel->highDELValue();	//unused
		stream << (quint16)pPanel->highFGFValue();
		stream << (quint16)pPanel->highVAValue();
		stream << (quint16)pPanel->highCOValue();
		stream << (quint16)(
			( pFirstPanel && pFirstPanel->m_bShowUptakeCost ? 1 : 0 ) +
			( pFirstPanel && pFirstPanel->m_bShowMl ? 2 : 0 ) +
			( pFirstPanel ? 4 : 0)
			);
		stream << (quint16)m_nSpeed;
		stream << save_nScaleMinutes;
	}
	else {
		stream >> w;
		if (w != wVerFlag) {
			m_cMSec_dx = w;
			return;
		}

		// Treat like version 1 (don't set flags) if we already have a
		// new header
		stream >> wVersion;
		if ( (m_SerializationFlags & SER_NEW_HEADER) != 0 )
			wVersion = 1;

		switch (wVersion)
		{
		case 6:
			m_SerializationFlags |= SER_TARGETS;
		case 5:
			m_SerializationFlags |= SER_LOCAL_AGENT;
		case 4:
			m_SerializationFlags |= SER_DESCRIPTION;
		case 3:
			m_SerializationFlags |= SER_COLORS;
		case 2:
		case 1:
			stream >> w;
			m_cMSec_dx = w;
			stream >> w;	//unused
			stream >> save_nActiveGas;
			stream >> save_HIGH_DEL;
			stream >> save_HIGH_FGF;
			stream >> save_HIGH_VA;
			stream >> save_HIGH_CO;
			stream >> w;
			stream >> save_nSpeed;
			stream >> save_nScaleMinutes;
			
			m_nSpeed = save_nSpeed;
		case 0:
			break;
		default:
			Q_ASSERT(false);
		}
		if (w & 4){
			save_bShowUptakeCost = w & 1;
			save_bShowMl = w & 2;
		} else {
			save_bShowUptakeCost = GasPanelView::m_nDfltShowCost;
			save_bShowMl = GasPanelView::m_nDfltShowMl;
		}
	}
}

void GasDoc::Serialize( QDataStream &stream, bool storing )
{
	m_anesArray.Serialize(stream, storing, m_SerializationFlags);	// tuck away/fetch anesthetic list

	if (!storing)
	{
		m_gasArray.clear();
		Gas::m_gpDoc = this;			// Reset pointer in persistent objects
	}
	
	m_gasArray.Serialize(stream, storing, m_SerializationFlags);	// and get/put computation points
				
	Gas::m_gpDoc = 0;					// (wish this worked right..)
	int nSamps = m_gasArray.at(0)->m_pSampArray->size();
	
	SerializeMSec_dx( stream, storing );
	
	if (storing) {
		stream << GetDescription();
		stream << m_dwCalcStart;
		stream << colors.size();
		foreach(QString color, colors)
		{
			stream << ToQColor(color);
		}
		stream << m_dwCalcTime;
		stream << m_dwHighTime;
		stream << (quint16)m_nBookmarks;
		for ( int i=0; i<m_nBookmarks; ++i )
			stream << m_dwBookmarks[i];
		
		stream << (quint16)MAX_COMPART;
		for ( int i=0; i<MAX_COMPART; ++i )
		{
			stream << (quint16)m_targetLists[i].count();
			for ( int j=0; j<m_targetLists[i].count(); ++j )
				stream << m_targetLists[i][j];
		}

		//stream.writeRawData( (const char *)m_fVolume, sizeof(m_fVolume) );
		for ( int i=0; i<MAX_COMPART; ++i )
			stream << m_fVolume[i];
		//stream.writeRawData( (const char *)m_fRatio, sizeof(m_fRatio) );
		for ( int i=0; i<MAX_COMPART; ++i )
			stream << m_fRatio[i];
		
		if (nSamps) {
			//stream.writeRawData( (const char *)m_nSampIdx, sizeof(m_nSampIdx) );
			for ( int i=0; i<MAX_SAMP; ++i )
				stream << m_nSampIdx[i];

		} else {//Fake a sample so we restore initial conditions
			RES_ARRAY res;
			GasSample samp(m_gasArray.at(0), res);
			samp.Serialize(stream,storing,0);
		}
	}
	else {
		SetTimeVariables(m_cMSec_dx);
		 
		if (m_SerializationFlags & SER_DESCRIPTION)
			stream >> description;
		stream >> m_dwCalcStart;
		int numColors = 0;
		if(m_SerializationFlags & SER_COLORS)
		{
			stream >> numColors;
			colors.clear();
			for(int i = 0; i < numColors; i++)
			{
				QString tmp;
				stream >> tmp;
				if (i < m_gasArray.length())
					colors.append(QObject::tr(tmp.toLatin1().data()));
			}
		}
		for(int i = numColors; i < m_gasArray.length(); i++)
				colors.append(getDefaultAgentColor(i));
		stream >> m_dwCalcTime;
		Q_ASSERT( (nSamps==0) == (m_dwCalcTime==0L) );
		stream >> m_dwHighTime;
		Q_ASSERT(m_dwCalcStart<=m_dwCalcTime && m_dwHighTime<m_dwCalcTime+m_cMSec_dx);

		if ( m_dwCalcTime > 0 )
			for ( int ng = 0; ng < m_gasArray.size(); ++ng ) {
				ResultsFromHistoryAt( m_dwCalcTime, ng );
				Gas *g = m_gasArray.at(ng);
				for ( int j = 0; j < SAMP_EVERY; j++ )
					for ( int i = 0; i < MAX_RESULT; ++i )
						g->m_pfCurResults[j][i] = g->m_pfOldResults[j][i];
			}

		quint16 w;
		stream >> w; m_nBookmarks = (int)w;
		Q_ASSERT(m_nBookmarks<=MAX_BKMK);
		for (int i = 0; i < m_nBookmarks; ++i)
		{
			int b;
			stream >> b;
			if (i < MAX_BKMK)
				m_dwBookmarks[i] = b;
		}

		if (m_SerializationFlags & SER_TARGETS)
		{
			stream >> w;
			Q_ASSERT(w <= MAX_COMPART);
			for (int i = 0; i < w; ++i)
			{
				quint16 x;
				stream >> x;
				for (int j = 0; j < x; ++j)
				{
					int t;
					stream >> t;
					if (i < MAX_COMPART)
						m_targetLists[i].append(t);
				}
			}
		}

		//stream.readRawData((char *)m_fVolume, sizeof(m_fVolume));
		for ( int i=0; i<MAX_COMPART; ++i )
			stream >> m_fVolume[i];
		//stream.readRawData((char *)m_fRatio, sizeof(m_fRatio));
		for ( int i=0; i<MAX_COMPART; ++i )
			stream >> m_fRatio[i];

		
		if (nSamps)	{
			for ( int i=0; i<MAX_SAMP; ++i )
				stream >> m_nSampIdx[i];
			ResetState(*m_gasArray.at(0)->m_pSampArray->at(0));
		}
		else {
			GasSample *samp = new GasSample();
			samp->Serialize(stream,storing,0);
			ResetState(*samp);
			delete samp;
		}
	}
}

//This routine function performs idle-time processing
void GasDoc::idle()
{
#if defined(Q_OS_MACX)
#		define LOOPSIZE 10
#	else
#		define LOOPSIZE 2
#	endif

	for ( int i = 0; i < LOOPSIZE && started; ++i )
		MakeProgress();
#	undef LOOPSIZE
}

void GasDoc::RunOnIdle()
{
	if ( started ) {
        long dw = m_dwProgress.elapsed();
		if (dw > DT/12 || dw < 0)
			MakeProgress();
		return;
	}
	started = true;
	timer->start( 0 );
	m_dwProgress.start();
}


void GasDoc::StopOnIdle()
{

	started = false;
	timer->stop();
}

void GasDoc::MakeProgress()
{
	// Scheduling half-life is 50 cycles (5 min @ 6 sec/breath).
	// Computed for all running sims
	m_dwAvgSched = 0;
	static quint16 wCycle = 0;
	wCycle = (wCycle+1) % 25;

	if ( GetState() == RUNNING_STATE ) {
		if (!wCycle) m_dwSched = (m_dwSched*707)/1000;
		int speed = GetCurrentSpeed();
		m_dwAvgSched += m_dwSched/qMin(speed,80);
	}

	Q_ASSERT( started );

	m_dwSched += 10000;
	if ( !ExtendResultSet() )
		StopOnIdle();

	m_dwProgress.restart();	// remember making progress
}

//This routine adds the child window to the list of children connected with this document
void GasDoc::addChildWindow( GasChildWindow * window )
{
	childWindows.append( window );
	 
}

//This routine removes the child window from the list of children connected with this document
void GasDoc::removeChildWindow( GasChildWindow * window )
{
	childWindows.removeAll( window );
	updateTitle();
	if ( !childWindows.size())
		deleteLater();
}

//This routine closes all child windows
bool GasDoc::close()
{
	if (m_bClosed)
		return true;
	if(maybeSave())
	{
		m_bClosed = true;
		foreach ( GasChildWindow *child, childWindows ) {
			gasMainWindow->removeChild(child);
		}
		return true;
	}
	return false;
}

//This routine gets the title of the document
QString GasDoc::title()
{
    if (!description.trimmed().isEmpty()){
        return description.split("\n", Qt::SkipEmptyParts).first().trimmed();
    }
    //Qt::SplitBehavior();
	return completeBaseName( m_curFile );
}

//This routine updates the title bar for each child window
void GasDoc::updateTitle()
{
	int graphViews = 0;
	int panelViews = 0;

	QString frameNum = "";
	QString baseTitle = title();
	QString priority = "";
	QString agent = "";

	foreach ( GasChildWindow *child, childWindows ) {		//for each child window

		switch(child->getViewType())
		{
		case GRAPH: frameNum = QString("%1").arg(++graphViews); break;
		case PANEL: frameNum = QString("%1").arg(++panelViews); break;
		default: frameNum = ""; break;
		}

		int nGas;
		if ( m_gasArray.size() > 1 && child->tryGetGas(nGas) ) {
			if (nGas)
				priority = tr("Add'l");
			else
				priority = tr("Primary");
			agent = QString("%1").arg(GetAgent(nGas)->m_strName);
		}
		child->setTitle( baseTitle, frameNum, priority, agent);
	}
	QString newTitle = title();
	emit titleChanged(newTitle);
}

void GasDoc::setCurrentFile( const QString &fileName )
{
	m_curFile = QFileInfo(fileName).canonicalFilePath();
	isUntitled = false;
	
	foreach ( GasChildWindow *child, childWindows )
		child->setWindowModified( false );

	if ( !fileName.isEmpty() ) {
		QStringList files = gasApp->ReadProfile( "Settings", "RecentFileList", QStringList() ).toStringList();
		files.removeAll( fileName );
		files.prepend( fileName );
		while ( files.size() > MaxRecentFiles )
			files.removeLast();

		gasApp->WriteProfile( "Settings", "RecentFileList", files );
	}
	GasMainWindow *gasMainWindow = qobject_cast<GasMainWindow *>( parent() );
	if ( gasMainWindow )
		gasMainWindow->updateRecentFileActions();
	updateTitle();

}

//This routine holds whether the document shown in the window has unsaved changes.
bool GasDoc::isModified()
{
	if ( childWindows.size() )
		return childWindows[0]->isWindowModified();
	return false;
}

bool GasDoc::save()
{
		
	if(glm->instance()->validLicenseExists())
	{
		if (isUntitled)
			return saveAs();
    else
			return saveFile( m_curFile );
	}else{
		QMessageBox::information( childWindows[0], tr( "Gas Man\xC2\xAE" ), tr( "Saving not available in student mode" ) );
		return true;
	}
}

bool GasDoc::saveAs()
{
	if(glm->instance()->validLicenseExists())
	{
		QString startDir = gasMainWindow->SaveDir() + QDir::separator() + title();

#ifdef Q_OS_MACX 
		QString fileName = QFileDialog::getSaveFileName( childWindows[0], tr("Save As"), startDir, 
			tr( "Gas Man\xC2\xAE Files (*.gas )" ), NULL, QFileDialog::DontUseNativeDialog );
#else
		QString fileName = QFileDialog::getSaveFileName( childWindows[0], tr("Save As"), startDir, tr( "Gas Man\xC2\xAE Files (*.gas )" ) );
#endif

		if (fileName.isEmpty())
			return false;
		if(fileName.split(".").last().toLower()!="gas")
			fileName+=".gas";
		if ( !saveFile(fileName) )
			return false;
		
		QFileInfo fi(fileName);
		gasMainWindow->setSaveDir(fi.path());
		return true;
	}else{
		QMessageBox::information( childWindows[0], tr( "Gas Man\xC2\xAE "), tr( "Saving not available in student mode" ) );
		return true;
	}
}

//This routine saves the data into the file
bool GasDoc::saveFile( const QString &fileName, bool setCurrent )
{
	 
	QFile file( fileName );
	if ( !file.open( QFile::WriteOnly ) ) {
        	QMessageBox::warning( childWindows[0], tr( "Gas Man\xC2\xAE" ), tr( "<p>Cannot write file %1:\n%2." ).arg( fileName ).arg( file.errorString() ) );
		return false;
	}

	QApplication::setOverrideCursor(Qt::WaitCursor);		//Sets the application override cursor to "Wait Cursor"
	QDataStream out( &file );								//Serialize the data into the file

	// Ouput latest serialization header
	m_SerializationFlags = SER_INITIAL_STATE | SER_COLORS | SER_DESCRIPTION | SER_LOCAL_AGENT | SER_NEW_HEADER | SER_TARGETS;
	out << (qint32)9933;
	out << m_SerializationFlags;

	// Serialize
	out.setVersion(QDataStream::Qt_4_0);
	Serialize( out, true );

	QApplication::restoreOverrideCursor();					//Restores the original widgets' cursors

	if ( setCurrent ) setCurrentFile( fileName );
	file.close();
	return true;
}

//This routine loads the data from the file
bool GasDoc::loadFile( const QString &fileName )
{
	QFile file( fileName );
	if ( !file.open( QFile::ReadOnly ) ) {
		QMessageBox::warning( getDialogParent(), tr( "Gas Man\xC2\xAE "), tr( "<p>Cannot read file ""%1"":\n%2." ).arg( fileName ).arg( file.errorString() ) );
        	return false;
	}

	if( !m_curFile.isNull() )
		DeleteContents();

	m_nSpeed = m_nDfltSpeed;

	QApplication::setOverrideCursor( Qt::WaitCursor );		//Sets the application override cursor to "Wait Cursor"
	QDataStream in( &file );								//Reads the data serialized from the file
	 
	qint32 ver;
	in >> ver;
	switch (ver)
	{
	case 9933:
		in >> m_SerializationFlags;
		break;
	case 9932:
		m_SerializationFlags = SER_NONE;
		break;
	default:
		QApplication::restoreOverrideCursor();
		QMessageBox::warning( getDialogParent(), tr( "Gas Man\xC2\xAE "), tr( "<p>File ""%1"" is either corrupt or from an unknown version of Gas Man&reg;\n." ).arg( fileName ) );
		return false;
	}
		
	in.setVersion(QDataStream::Qt_4_0);
	Serialize( in, false );
	QApplication::restoreOverrideCursor();					//Restores the original widgets' cursors

	setCurrentFile( fileName );
	m_bOpened = true;

	return true;
}

//This routine checks if the changes are stored and/or if they should be stored
bool GasDoc::maybeSave()
{
    if ( !isModified() )
		return true;
	int ret;
//    int ret = QMessageBox::warning( childWindows[0], tr("Gas Man\xC2\xAE"), tr("<p>The document has been modified. "
//		"Do you want to save your changes?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
	//give the messagebox accelerator key, but no icon. test if the discard and cancel work correctly
	QMessageBox * box = new QMessageBox();
	box->setWindowTitle(tr("Gas Man\xC2\xAE - ") + title());
	 
	box->setText(tr("<p>The document has been modified. "
	  		"Do you want to save your changes?"));
	QPushButton *dis = new QPushButton(tr("&Discard"));
	QPushButton *sav = new QPushButton(tr("&Save"));
	QPushButton *can = new QPushButton(tr("&Cancel"));
#ifdef Q_OS_MAC 
	dis->setShortcut(Qt::Key_D);
	sav->setShortcut(Qt::Key_S);
	can->setShortcut(Qt::Key_C);
#endif
 
	box->addButton( sav,QMessageBox::AcceptRole);
	
	
	box->addButton( dis,QMessageBox::DestructiveRole);
	
	box->addButton( can,QMessageBox::RejectRole);
	box->setDefaultButton(sav);
	ret = box->exec();
	 
	if ( ret == /*QMessageBox::Save */  0)
        return save();
	if ( ret ==/* QMessageBox::Cancel*/ 2 )
        return false;


	foreach(GasChildWindow* child, childWindows)
		child->setWindowModified( false );

	return true;
}

//This routine sends a message with the document as an attachment
void GasDoc::send()
{
#if defined(Q_OS_WIN)	
	QLibrary lib( "Mapi32.dll" );
	LPMAPISENDMAIL mapisendmail = (LPMAPISENDMAIL)lib.resolve("MAPISendMail"); //Get the address of the MAPISendMail function

	Q_ASSERT( mapisendmail != 0 );

	QTemporaryFile *file = new QTemporaryFile();	//Construct attachment
	if ( file->open() ) {
		file->setAutoRemove ( false );
		bool bModified = isModified();
		bool bResult = saveFile( file->fileName(), false );		//Store data to attachment
		if ( bModified ){
			emit modified();
		}
		if ( !bResult ) {
			delete file;
			return;
		}
	}
	//We fill the structure with information about the file containing a
	//message attachment stored as a temporary file
	MapiFileDesc fileDesc;
	memset(&fileDesc, 0, sizeof(fileDesc));
	fileDesc.nPosition = (unsigned long)-1;

    QByteArray pathName = QDir::toNativeSeparators(file->fileName()).toLocal8Bit();
    QByteArray fileName = QDir::toNativeSeparators( m_curFile ).toLocal8Bit();

	fileDesc.lpszPathName = pathName.data();
    fileDesc.lpszFileName = fileName.data();

	delete file;
	
	//We fill the structure with information about the message
	MapiMessage msg;
	memset(&msg, 0, sizeof(msg));
	msg.nFileCount = 1;
	msg.lpFiles = &fileDesc;
	
	//Sends a message
	int ret = mapisendmail( 0, 0L, &msg, MAPI_LOGON_UI | MAPI_DIALOG, 0L );

	if ( ret != SUCCESS_SUCCESS &&	ret != MAPI_USER_ABORT && ret != MAPI_E_LOGIN_FAILURE )
		QMessageBox::critical( childWindows[0], tr( "Gas Man\xC2\xAE "), tr( "Send Mail failed to send message." ) );

#elif defined(Q_OS_MACX)
    // TODO: Not implemented for Mac. See following note for explanation.
    // The "Send" command has been disabled for the Mac.
    // Apple says to use an AppleScript to make email attachments.
    // See <http://developer.apple.com/qa/qa2001/qa1018.html>
#else
    // TODO: The following method does not work with all email clients,
    // as the 'attach' key is not standardized. Otherwise this would be
    // an ideal method to use for all cases.
    bool status;

    QUrlQuery mailto("mailto:"); // Empty mail to url
    mailto.addQueryItem("subject", title());
    
    // Construct attachment
    QTemporaryFile attachment(QDir::tempPath() + title() + ".XXXXXX.gas");
    status = false;
    if (attachment.open()) {
        attachment.setAutoRemove(false);
        status = saveFile(attachment.fileName(), false);
        mailto.addQueryItem("attach", attachment.fileName());
    }

    // Open mail client
    if (status) {
        status = QDesktopServices::openUrl(QUrl(mailto.toString()));
    }
    
    if (!status) {
        QMessageBox::critical(childWindows[0], tr("Gas Man\xC2\xAE"),
                              tr("Send Mail failed to send message."));
    }
#endif
}

EventList GasDoc::SampComp( GasSample &s1, GasSample &s2 )
{
	EventList events = 0;

	if (s1.m_fDEL != s2.m_fDEL) events |= UPD_DEL;
	if (s1.m_fCO != s2.m_fCO) events |= UPD_CO;
	if (s1.m_fFGF != s2.m_fFGF) events |= UPD_FGF;
	if (s1.m_fVA != s2.m_fVA) events |= UPD_VA;
	if (s1.m_fWeight != s2.m_fWeight) events |= UPD_WEIGHT;
	if (s1.m_wAnesID != s2.m_wAnesID) events |= UPD_AGENT;
	if (s2.m_uInjections || s2.m_bFlush) events |= UPD_INJECT;
	if (   s1.m_bRtnEnb  != s2.m_bRtnEnb
		|| s1.m_bUptEnb  != s2.m_bUptEnb
		|| s1.m_bVapEnb  != s2.m_bVapEnb
		|| s1.m_uCktType != s2.m_uCktType
		)
		events |= UPD_CIRCUIT;
	return events;
}

QDomElement GasDoc::createDomElement(QDomDocument& doc, const QString& name, const GasAttributeMap& attributes, const QList<QDomElement>& children)
{
	QDomElement ret = doc.createElement(name);

	foreach(QString str, attributes.keys())
	{
		QString valString;
		QVariant valVar = attributes.value(str);

		// round to 4 sig digits
		if(valVar.type() == QVariant::Double)
			valString = QString::number(valVar.toDouble(), 'f', 4);
		else
			valString = valVar.toString();			
		ret.setAttribute(str, QString("%1").arg(valString));
	}

	foreach(QDomElement child, children)
		ret.appendChild(child);

	return ret;
}

QDomDocument GasDoc::toXml(const QByteArray& ba, const QString& externalImageName, const QString& iconName, bool graph, bool printout, bool wantResults)
{
	QDomDocument doc;
	doc.appendChild( doc.createProcessingInstruction( "xml", "version=\'1.0\' encoding=\'utf-8\'" ) );
	doc.appendChild( doc.createProcessingInstruction(
		"xml-stylesheet",
		"type=\'text/xsl\' href=\'" + fileName(gasApp->getXslFile()) + "\'" )
		);

	QMap<QString, QVariant> attributes;
	attributes["datetime"] = QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss" );
	attributes["iconPath"] = iconName;
	attributes["description"] = description;
	if (externalImageName.isEmpty())
	{
		attributes["imageData"] = ba;
		graph = false;
	}
	else
		attributes["externalImage"] = externalImageName;

	attributes["printGraph"] = graph;
	attributes["printPrintout"] = printout;

	QList<QDomElement> children;
	children << paramsToXml(doc);
	children << settingsToXml(doc, wantResults);

	QDomElement gasman = createDomElement(doc, "gasman", attributes, children);	
	doc.appendChild( gasman );

	return doc;
}

QDomElement GasDoc::paramsToXml(QDomDocument& doc)
{
	GasAttributeMap attributes;
	QList<QDomElement> children;
	children << patientToXml(doc);
	children << agentsToXml(doc);
	return createDomElement(doc, "params", attributes, children);
}

QDomElement GasDoc::patientToXml(QDomDocument& doc)
{
	QList<QDomElement> children;
	children << weightToXml(doc);
	children << volumesToXml(doc);
	children << flowsToXml(doc);
	return createDomElement(doc, "patient", GasAttributeMap(), children);													
}

QDomElement GasDoc::weightToXml(QDomDocument& doc)
{
	GasAttributeMap attributes;
	attributes["value"] = m_fWeight;
	attributes["unit"] = tr("kilograms");
	return createDomElement(doc, "weight", attributes);
}

QDomElement GasDoc::volumesToXml(QDomDocument& doc)
{
	double fWtRatio = m_fWeight / STD_WEIGHT;
	GasAttributeMap attributes;
	attributes["alv"] = /*qRound( */fWtRatio * m_fVolume[ALV]/* * 1000 ) / 1000.0*/;
	attributes["vrg"] = /*qRound( */fWtRatio * m_fVolume[VRG]/* * 1000 ) / 1000.0*/;
	attributes["mus"] = /*qRound( */fWtRatio * m_fVolume[MUS]/* * 1000 ) / 1000.0*/;
	attributes["fat"] = /*qRound( */fWtRatio * m_fVolume[FAT]/* * 1000 ) / 1000.0*/;
	attributes["ven"] = /*qRound( */fWtRatio * m_fVolume[VEN]/* * 1000 ) / 1000.0*/;
	return createDomElement(doc, "volumes", attributes);
}

QDomElement GasDoc::flowsToXml(QDomDocument& doc)
{
	GasAttributeMap attributes;
	attributes["vrg"] = m_fRatio[VRG];
	attributes["mus"] = m_fRatio[MUS];
	attributes["fat"] = m_fRatio[FAT];
	return createDomElement(doc, "flows", attributes);
}

QDomElement GasDoc::agentsToXml(QDomDocument& doc)
{
	GasAttributeMap attributes;
	QList<QDomElement> children;
	for(int i = 0; i < m_gasArray.size(); i++)
	{
		GasAnesthetic& anes = *m_anesArray[m_gasArray[i]->m_nAgent];
		children << agentToXml(doc, anes);
	}
	return createDomElement(doc, "agents", attributes, children);
}

QDomElement GasDoc::agentToXml(QDomDocument& doc, const GasAnesthetic& anes)
{
	GasAttributeMap attributes;
	attributes["name"] = anes.m_strName;
	attributes["lambdaBlood"] = anes.m_fLambdaBG;
	attributes["lambdaVrg"] = anes.m_fSolubility[VRG];
	attributes["lambdaFat"] = anes.m_fSolubility[FAT];
	attributes["lambdaMus"] = anes.m_fSolubility[MUS];
	attributes["volatility"] = anes.m_wVolatility;
	attributes["bottleSize"] = anes.m_nBottleSize;
	attributes["bottleCost"] = anes.m_fBottleCost;
	return createDomElement(doc, "agent", attributes);
}

QDomElement GasDoc::settingsToXml(QDomDocument& doc, bool wantResults)
{
	QList<QDomElement> children;

	int nSamps = getSampleCount();
	if ( nSamps == 0 ) {
		// Special case: Just want initial results. Create a sample for t0
		Gas* pGas = m_gasArray.first();
		GasSample sample(pGas, *pGas->m_pfCurResults);
		children << settingToXml(doc, sample);
		if ( wantResults ) {
			QList<QDomElement> grandChild;
			grandChild << resultToXml(doc, 0);
			children << createDomElement(doc, "results", GasAttributeMap(), grandChild);
		}
	} else {
		int nSamp = 0;
		while ( nSamp < nSamps )
		{
			GasSample& sample= *m_gasArray.first()->m_pSampArray->at(nSamp);
			if ( sample.m_dwBaseTime/1000L > GetHighTime() )
				break;
			children << settingToXml(doc, sample);
			QDomElement results = resultsToXml(doc, nSamp, nSamps);
			if ( wantResults )
				children << results;
		}
	}
	return createDomElement(doc, "settings", GasAttributeMap(), children);
}

QDomElement GasDoc::settingToXml(QDomDocument& doc, GasSample& sample)
{
	// get human readable time
	quint32 time = sample.m_dwBaseTime;
	int hh = (int)(time/3600000);
	int mm = (int)((time-hh*3600000)/60000);
	int ss = (int)((time-((hh*60)+mm)*60000)/1000);	

	GasAttributeMap attributes;
	attributes["time"] = QString("%1:%2:%3").arg( hh, 2, 10, QLatin1Char( '0' ) ).arg( mm, 2, 10, QLatin1Char( '0' ) ).arg( ss, 2, 10, QLatin1Char( '0' ) );

	float tmp = time/60000;
	attributes["va"] = GetVA(tmp);
	attributes["co"] = GetCO(tmp);
	attributes["fgf"] = GetFGF(tmp, 0);

	// get circuit type
	QString szFlag[] = {tr("(UR)"),  tr("(R)"),  tr("(U)"), "", tr("(UR)*"), tr("(R)*"), tr("(U)*"), "*",
						tr("(VUR)"),  tr("(VR)"),  tr("(VU)"),  tr("(V)"), tr("(VUR)*"), tr("(VR)*"), tr("(VU)*"), tr("(V)*") };
	
	int i =
		  sample.m_bVapEnb*8
		+ sample.m_bFlush*4
		+ sample.m_bRtnEnb*2
		+ sample.m_bUptEnb;

	QString circuit;
	switch (sample.m_uCktType)
	{
	case OPEN_CKT:
		circuit = tr( "Open%1" ).arg( szFlag[i] );
		break;
	case SEMI_CKT:
		circuit = tr( "Semi%1" ).arg( szFlag[i] );
		break;
	case CLOSED_CKT:
		circuit = tr( "Closed%1" ).arg( szFlag[i] );
		break;
	case IDEAL_CKT:
		circuit = tr( "Ideal%1" ).arg( szFlag[i] );
		break;
	}
	attributes["circuit"] = circuit;

	QList<QDomElement> children;
	children << agentSettingsToXml(doc, time);

	return createDomElement(doc, "setting", attributes, children);
}

QDomElement GasDoc::agentSettingsToXml(QDomDocument& doc, quint32 time)
{
	QList<QDomElement> children;

	for(int ng = 0; ng < m_gasArray.size(); ng++)
		children << agentSettingToXml(doc, ng, time);

	return createDomElement(doc, "agentsettings", GasAttributeMap(), children);
}

QDomElement GasDoc::agentSettingToXml(QDomDocument& doc, int gasNum, quint32 time)
{
	Gas* pGas = m_gasArray.at(gasNum);
	int nSamps = getSampleCount();
	GasSample *sample;

	if ( nSamps == 0 ) {
		Q_ASSERT( time == 0 );
		sample = new GasSample(pGas, *pGas->m_pfCurResults);
	} else {
		sample = pGas->m_pSampArray->at(SampAt(time));
	}

	GasAttributeMap attributes;
	attributes["agentname"] = pGas->m_strAgent;
	attributes["del"] = sample->m_fDEL;

	// if there was an injection and it was for this gas
	if ( sample->m_uInjections )
		attributes["inject"] = QString( "%1" ).arg( sample->m_uInjections*sample->m_fUnitDose, 5, 'f', 2 );
	else
		attributes["inject"] = 0;

	if ( nSamps == 0 )
		delete sample;

	return createDomElement(doc, "agentsetting", attributes);
}

QDomElement GasDoc::resultsToXml(QDomDocument& doc, int& nSamp, int nSamps)
{
	QList<QDomElement> children;

	bool more = nSamp < nSamps;
	while ( more )
	{
		quint32 time = m_gasArray.first()->m_pSampArray->at(nSamp)->m_dwBaseTime;
		if ( time/1000L > GetHighTime() )
			break;
		children << resultToXml(doc, time);
		if ( ++nSamp >= nSamps )
			break;
		for ( int ng = 0; more && ng < m_gasArray.size(); ng++ )
		{
			GasSample* prevSamp = (*m_gasArray[ng]->m_pSampArray)[nSamp-1];
			GasSample* samp = (*m_gasArray[ng]->m_pSampArray)[nSamp];
			more = SampComp(*prevSamp, *samp) == 0;
		}
	}

	return createDomElement(doc, "results", GasAttributeMap(), children);
}

QDomElement GasDoc::resultToXml(QDomDocument& doc, quint32 time)
{
	GasAttributeMap attributes;
	QList<QDomElement> children;

	// calculate human readable time
	int hh = (int)(time/3600000);
	int mm = (int)((time-hh*3600000)/60000);
	int ss = (int)((time-((hh*60)+mm)*60000)/1000);	

	attributes["time"] = QString("%1:%2:%3").arg( hh, 2, 10, QLatin1Char( '0' ) ).arg( mm, 2, 10, QLatin1Char( '0' ) ).arg( ss, 2, 10, QLatin1Char( '0' ) );
	for(int i = 0; i < m_gasArray.size(); i++)
	{
		children << agentResultToXml(doc, i, time/60000.0F);
	}
	return createDomElement(doc, "result", attributes, children);
}

QDomElement GasDoc::agentResultToXml(QDomDocument& doc, int gasNum, float fMin)
{
	GasAttributeMap attributes;

	attributes["name"] = m_gasArray[gasNum]->m_strAgent;
	attributes["fgf"] = GetFGF(fMin, gasNum);
	attributes["va"] = GetVA(fMin);
	attributes["uptake"] = GetUptake(fMin, gasNum);
	attributes["delivered"] = GetDelivered(fMin, gasNum);

	QList<QDomElement> children;
	children << compartmentsToXml(doc, gasNum, fMin);

	return createDomElement(doc, "agentresult", attributes, children);
}

QDomElement GasDoc::compartmentsToXml(QDomDocument& doc, int gasNum, float fMin)
{
	GasAttributeMap attributes;

	QList<QDomElement> children;
	children << compartmentToXml(doc, tr("CKT"), GetCKT(fMin, gasNum));
	children << compartmentToXml(doc, tr("ALV"), GetALV(fMin, gasNum));
	children << compartmentToXml(doc, tr("ART"), GetART(fMin, gasNum));
	children << compartmentToXml(doc, tr("VRG"), GetVRG(fMin, gasNum));
	children << compartmentToXml(doc, tr("MUS"), GetMUS(fMin, gasNum));
	children << compartmentToXml(doc, tr("FAT"), GetFAT(fMin, gasNum));
	children << compartmentToXml(doc, tr("VEN"), GetVEN(fMin, gasNum));

	return createDomElement(doc, "compartments", attributes, children);
}

QDomElement GasDoc::compartmentToXml(QDomDocument& doc, QString compartmentName, float val)
{	
	GasAttributeMap attributes;
	attributes["name"] = compartmentName;
	attributes["value"] = val;

	return createDomElement(doc, "compartment", attributes);
}

//////////////////////////////////////////////////////////////////////////////////
void GasDoc::setColorToDefault(const int nGas)
{
	colors.replace(nGas, getDefaultAgentColor(nGas));
 	emit agentColorsChanged();
}

void GasDoc::defaultAgentColors()
{
	for(int i=0; i<m_gasArray.length(); i++)
		colors.replace(i, getDefaultAgentColor(i));
 	emit agentColorsChanged();
}

void GasDoc::changeColor(int nGas, const QString& color)
{	
	colors.replace(nGas, color);
 	emit agentColorsChanged();
}

void GasDoc::colorSwap(int first, int second)
{
    colors.swapItemsAt(first, second);
	emit agentColorsChanged();
}

