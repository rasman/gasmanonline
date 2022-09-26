#include "ui_gaspanelview.h"

#include "gaspanelview.h"

#if QT_VERSION >=0x050000
#include <QtWidgets/QToolTip>
#include <QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#else
#include <QtGui>
#endif
#include "gaschildwindow.h"
#include "gasdoc.h"
#include "gasagentdlg.h"
#include "gasapplication.h"
#include "gasgraphview.h"
#include "semaphoreholder.h"
#include <QtDebug>

static bool bStoppedForChange;						// NZ if spinner/gauge pressed while running
const int repeatInterval = 100;

int	GasPanelView::m_nDfltHIGH_FGF;					// Defaults from INI
int	GasPanelView::m_nDfltHIGH_VA;
int	GasPanelView::m_nDfltHIGH_CO;
int	GasPanelView::m_nDfltShowCost;
int GasPanelView::m_nDfltShowMl;
int	GasPanelView::m_nDfltShowControl;

/////////////////////////////////////////////////////////////////////////////
// Class GasDoubleValidator changes the behavior of QLineEdit with QDoubleValidator.
// We need to reimplement "validate()" to have a stricter behavior:
//        value must be in range( setRange) and must have 2 digits after the decimal point.
// see http://www.trolltech.com/developer/knowledgebase/faq.2006-05-15.0450651751
//
class GasDoubleValidator : public QDoubleValidator
{
public:
	GasDoubleValidator( QObject* parent = 0 )
		: QDoubleValidator( parent )
	{}
	QValidator::State validate ( QString & input, int &pos ) const
	{
		if ( input.isEmpty() || input == "." ) {
			return Intermediate;
		}
		if ( QDoubleValidator::validate( input, pos ) != Acceptable ) {
			return Invalid;
		}
		return Acceptable;
	}
};


/////////////////////////////////////////////////////////////////////////////
// GasPanelView construction/destruction

GasPanelView::GasPanelView( GasDoc *doc, GasChildWindow * childWindow, QWidget *parent )
	: GasViewCommon( childWindow, parent ), gasdoc( doc ), koefWidthMchn( 0.75 )
{
	MAX_DEL = 500.0F;			//Set when we find out...
	m_nLastAgent = m_nGas = -1;	//No previous agent
	m_fMAC = 0.0F;				//MAC unknown
	m_fLastCKT = -1.0F;			//No previous ...
	m_fLastALV = -1.0F;
	m_fLastART = -1.0F;
	m_fLastVRG = -1.0F;
	m_fLastMUS = -1.0F;
	m_fLastFAT = -1.0F;
	m_fLastVEN = -1.0F;
	
	m_fLastUptake = -1.0F;
	m_fLastDelivered = -1.0F;
	m_fLastTime = -1.0F;

	hiddenNumerics = false;	//We show everything at first

	m_bShowUptakeCost = m_nDfltShowCost;
	m_bShowMl = m_nDfltShowMl;

	// Create form from the UI
	ui = new Ui_GasPanelView();
	ui->setupUi( this );
	
	
	ui->leDEL->setAlignment(Qt::AlignHCenter);
	ui->leHighDEL->setAlignment(Qt::AlignHCenter);
	ui->leHighFGF->setAlignment(Qt::AlignHCenter);
	ui->leFGF->setAlignment(Qt::AlignHCenter);
	ui->leHighVA->setAlignment(Qt::AlignHCenter);
	ui->leVA->setAlignment(Qt::AlignHCenter);
	ui->leHighCO->setAlignment(Qt::AlignHCenter);
	ui->leCO->setAlignment(Qt::AlignHCenter);
	 
	// Initialization of the form items
	ui->leHighFGF->setText( QString::number( m_nDfltHIGH_FGF ) );
	ui->leHighCO->setText( QString::number( m_nDfltHIGH_CO ) );
	ui->leHighVA->setText( QString::number( m_nDfltHIGH_VA ) );

	// do not allow line edits to accept drops
	ui->leHighDEL->setAcceptDrops(false);
	ui->leHighFGF->setAcceptDrops(false);
	ui->leHighCO->setAcceptDrops(false);
	ui->leHighVA->setAcceptDrops(false);

	ui->leDEL->setAcceptDrops(false);
	ui->leFGF->setAcceptDrops(false);
	ui->leCO->setAcceptDrops(false);
	ui->leVA->setAcceptDrops(false);

	ui->tbDELup->setAutoRepeatInterval( repeatInterval );
	ui->tbDELdown->setAutoRepeatInterval( repeatInterval );
	ui->tbFGFup->setAutoRepeatInterval( repeatInterval );
	ui->tbFGFdown->setAutoRepeatInterval( repeatInterval );
	ui->tbVAup->setAutoRepeatInterval( repeatInterval );
	ui->tbVAdown->setAutoRepeatInterval( repeatInterval );
	ui->tbCOup->setAutoRepeatInterval( repeatInterval );
	ui->tbCOdown->setAutoRepeatInterval( repeatInterval );

	QString vaToolTip = tr( "<p><center>Differs from setting due to Uptake Effect, caused by the uptake of \
							anesthetic from the lungs, which also causes the Concentration Effect and Second \
							Gas Effect. Can be disabled in Special menu. Click display to see or change original \
							setting.</center></p>" );

	QString fgfToolTip = tr( "<p><center>Differs from setting due to Vaporization Effect, caused by the addition of \
							 vapor to the flow. Only occurs if Vapor is enabled in Special menu. Click display \
							 to see or change original setting.</center></p>" );
	
	ui->stFGF->setToolTip( fgfToolTip );
	ui->stVA->setToolTip( vaToolTip );

	connect( ui->leFGF, SIGNAL( editingFinished() ), this, SLOT( changed() ) );
	connect( ui->leDEL, SIGNAL( editingFinished() ), this, SLOT( changed() ) );
	connect( ui->leVA, SIGNAL( editingFinished() ), this, SLOT( changed() ) );
	connect( ui->leCO, SIGNAL( editingFinished() ), this, SLOT( changed() ) );

	connect( ui->leFGF, SIGNAL( textChanged ( const QString & ) ), this, SLOT( verify( const QString & ) ) );
	connect( ui->leDEL, SIGNAL( textChanged ( const QString & ) ), this, SLOT( verify( const QString & ) ) );
	connect( ui->leVA, SIGNAL( textChanged ( const QString & ) ), this, SLOT( verify( const QString & ) ) );
	connect( ui->leCO, SIGNAL( textChanged ( const QString & ) ), this, SLOT( verify( const QString & ) ) );
	
	connect( ui->tbDELup, SIGNAL( clicked() ), this, SLOT( upDownValue() ) );
	connect( ui->tbDELdown, SIGNAL( clicked() ), this, SLOT( upDownValue() ) );
	connect( ui->tbFGFup, SIGNAL( clicked() ), this, SLOT( upDownValue() ) );
	connect( ui->tbFGFdown, SIGNAL( clicked() ), this, SLOT( upDownValue() ) );
	connect( ui->tbVAup, SIGNAL( clicked() ), this, SLOT( upDownValue() ) );
	connect( ui->tbVAdown, SIGNAL( clicked() ), this, SLOT( upDownValue() ) );
	connect( ui->tbCOup, SIGNAL( clicked() ), this, SLOT( upDownValue() ) );
	connect( ui->tbCOdown, SIGNAL( clicked() ), this, SLOT( upDownValue() ) );

	connect( ui->leHighFGF, SIGNAL( editingFinished() ), this, SLOT( maxChanged() ) );
	connect( ui->leHighDEL, SIGNAL( editingFinished() ), this, SLOT( maxChanged() ) );
	connect( ui->leHighVA, SIGNAL( editingFinished() ), this, SLOT( maxChanged() ) );
	connect( ui->leHighCO, SIGNAL( editingFinished() ), this, SLOT( maxChanged() ) );

	connect( ui->leHighFGF, SIGNAL( textChanged ( const QString & ) ), this, SLOT( verify( const QString & ) ) );
	connect( ui->leHighDEL, SIGNAL( textChanged ( const QString & ) ), this, SLOT( verify( const QString & ) ) );
	connect( ui->leHighVA, SIGNAL( textChanged ( const QString & ) ), this, SLOT( verify( const QString & ) ) );
	connect( ui->leHighCO, SIGNAL( textChanged ( const QString & ) ), this, SLOT( verify( const QString & ) ) );

	connect( ui->tbFlush, SIGNAL( clicked() ), this, SLOT( flush() ) );
	connect( ui->m_Srynge, SIGNAL( clicked() ), this, SLOT( Srynge() ) );

	connect( ui->ggDEL, SIGNAL( FireGasgaugeStart() ), this, SLOT( gasgaugeChangeStart() ) );
	connect( ui->ggFGF, SIGNAL( FireGasgaugeStart() ), this, SLOT( gasgaugeChangeStart() ) );
	connect( ui->ggVA, SIGNAL( FireGasgaugeStart() ), this, SLOT( gasgaugeChangeStart() ) );
	connect( ui->ggCO, SIGNAL( FireGasgaugeStart() ), this, SLOT( gasgaugeChangeStart() ) );

	connect( ui->ggDEL, SIGNAL( FireGasgaugeChange( short ) ), this, SLOT( gasgaugeChangeGauge( short ) ) );
	connect( ui->ggFGF, SIGNAL( FireGasgaugeChange( short ) ), this, SLOT( gasgaugeChangeGauge( short ) ) );
	connect( ui->ggVA, SIGNAL( FireGasgaugeChange( short ) ), this, SLOT( gasgaugeChangeGauge( short ) ) );
	connect( ui->ggCO, SIGNAL( FireGasgaugeChange( short ) ), this, SLOT( gasgaugeChangeGauge( short ) ) );

	connect( ui->ggDEL, SIGNAL( FireGasgaugeStop() ), this, SLOT( gasgaugeChangeStop() ) );
	connect( ui->ggFGF, SIGNAL( FireGasgaugeStop() ), this, SLOT( gasgaugeChangeStop() ) );
	connect( ui->ggVA, SIGNAL( FireGasgaugeStop() ), this, SLOT( gasgaugeChangeStop() ) );
	connect( ui->ggCO, SIGNAL( FireGasgaugeStop() ), this, SLOT( gasgaugeChangeStop() ) );

	connect( gasdoc, SIGNAL(updatePanel()), this, SLOT(updateData()));
	connect( ui->m_Gasmchn, SIGNAL( Resized() ), this, SLOT( gasmchnGeometryUpdated() ) );

	GasDoubleValidator *dvValidator = new GasDoubleValidator( this );
	dvValidator->setBottom( 0 );
	dvValidator->setDecimals( 2 );
	ui->leDEL->setValidator( dvValidator );
	ui->leFGF->setValidator( dvValidator );
	ui->leCO->setValidator( dvValidator );
	ui->leVA->setValidator( dvValidator );

	QIntValidator *ivFGF = new QIntValidator( 1, int( MAX_FGF ), this );
	ui->leHighFGF->setValidator( ivFGF );

	QIntValidator *ivVA = new QIntValidator( 1, int( MAX_VA ), this );
	ui->leHighVA->setValidator( ivVA );

	QIntValidator *ivCO = new QIntValidator( 1, int( MAX_CO ), this );
	ui->leHighCO->setValidator( ivCO );

	ivDEL = new QIntValidator( this );
	
	ui->leHighDEL->setValidator( new QIntValidator(this));

	ui->leFGF->installEventFilter( this );
	ui->leVA->installEventFilter( this );

	// Set width for gauge-controls by setting the fixed size for "High"-lineEdits
	QFontMetrics fm( font() );
	int ps = fm.boundingRect( "0000" ).width()+ 7;
	ui->leHighDEL->setFixedWidth( ps );
	ui->leHighFGF->setFixedWidth( ps );
	ui->leHighVA->setFixedWidth( ps );
	ui->leHighCO->setFixedWidth( ps );

	ps = fm.boundingRect( "00000" ).width()+ 7;
	ui->frDEL->setFixedWidth( ps + 2);
	ui->frFGF->setFixedWidth( ps + 2);
	ui->frVA->setFixedWidth( ps + 2);
	ui->frCO->setFixedWidth( ps + 2);
	ui->leDEL->setFixedWidth( ps );
	ui->leFGF->setFixedWidth( ps );
	ui->leVA->setFixedWidth( ps );
	ui->leCO->setFixedWidth( ps );
	
	// Stop to use central GridLayout (!!!) It is important.  
	layout()->setEnabled ( false ); 
	// resizing of items to minimalSizeHints
	ui->m_Gasmchn->resize( ui->m_Gasmchn->minimumSizeHint() );
	ui->frDEL->resize( ui->frDEL->minimumSizeHint() );
	ui->lbDEL->resize( ui->lbDEL->minimumSizeHint() );
	ui->frFlush->resize( ui->frFlush->minimumSizeHint() );
	ui->lbFGF->resize( ui->lbFGF->minimumSizeHint() );
	ui->lbVA->resize( ui->lbVA->minimumSizeHint() );
	ui->lbCO->resize( ui->lbCO->minimumSizeHint() );
	ui->frame_t->resize( ui->frame_t->minimumSizeHint() );

	stepTimer = new QTimer( this );
	
	setMinimumSize( minimumSizeHint() );
	ui->stFGF->setVisible( false );

	LeChangeSemaphore.release(1);	 
}

GasPanelView::~GasPanelView()
{
	StopTicker();
	delete ui;
}

void GasPanelView::updateData()
{
	FetchData();
}

// Calculation of minimumSizeHint for GasPanelView
QSize GasPanelView::minimumSizeHint () const
{
	QFontMetrics fm( font() );

	if (ui->m_Gasmchn == NULL)
		return QWidget::minimumSizeHint();

	// "frame_t" must be less then 46% percent of GasMachine. Otherwise, the right side of "frame_t" would be off of the form.
	if ( ( ui->m_Gasmchn->minimumWidth() * 0.46 ) < ui->frame_t->width() ) 
		ui->m_Gasmchn->setMinimumWidth( ui->frame_t->width() * 100 / 46 + 1 ); 	// MinimumSize of GasMachine
	
	// Recalculation of the GasMachine minHeight
	int minGgHeight = ui->frDEL->minimumSizeHint().height() + layout()->spacing() + fm.boundingRect( ui->lbDEL->text() ).height()*2 + 4;
	if ( ui->m_Gasmchn->minimumHeight() < minGgHeight )
		ui->m_Gasmchn->setMinimumHeight( minGgHeight );

	int w1 = ui->frDEL->minimumSizeHint().width() + ui->ggDEL->minimumWidth() + 
		ui->frFlush->minimumSizeHint().width() + layout()->spacing() * 2;  // Minumum width for Gauge-block
	int w2 = ui->m_Gasmchn->minimumWidth()  + layout()->spacing(); // Minimum width for GasMachine
	int w = w1 + w2 + layout()->margin() * 2; // Minimum width for form
	int h = ( ui->m_Gasmchn->minimumSizeHint().height() + layout()->margin() ) * 2 + layout()->spacing(); // Minimum height
	((GasPanelView *)this)->koefWidthMchn =  w2 / (double)( w1 + w2); // Percent of GasMachine in the width of the form

	return QSize( w, h );
}

QSize GasPanelView::sizeHint () const
{
	return minimumSizeHint();
}

void GasPanelView::InitialUpdate(int nGas) 
{
	m_nGas = nGas;
	m_strAgent = gasdoc->GetAgent( m_nGas )->m_strName;
	ui->lbDEL->setText(tr( "<b>" ) + m_strAgent + tr( "</b><br>DEL %atm" ));

	FetchData( true );					// Fetch all data from the doc
	setLeEnabled( ui->leVA, true );
	setLeEnabled( ui->leFGF, true );
	ui->leFGF->setCursorPosition ( 0 );
	ui->m_Gasmchn->SetGasLabelCKT(tr("CKT")+'('+m_strCircuit[0]+')');

	// Set up the appropriate labels
	showCost( m_bShowUptakeCost );

	// Set up the anesthetic choices

	UpdateData( false );											// Pump it all out AGAIN
	gasdoc->updateTitle();
}

void GasPanelView::ReadProfile()
{
	// Read in the profile strings

	m_nDfltHIGH_FGF = gasApp->ReadProfile( "Defaults", "HighFGF", 10 ).toUInt();
	if ( m_nDfltHIGH_FGF < 1 || m_nDfltHIGH_FGF > MAX_FGF )
		m_nDfltHIGH_FGF = 10;

	m_nDfltHIGH_VA = gasApp->ReadProfile( "Defaults", "HighVA", 10 ).toUInt();
	if ( m_nDfltHIGH_VA < 1 || m_nDfltHIGH_VA > MAX_VA )
		m_nDfltHIGH_VA = 10;

	m_nDfltHIGH_CO = gasApp->ReadProfile( "Defaults", "HighCO", 10 ).toUInt();
	if ( m_nDfltHIGH_CO < 1 || m_nDfltHIGH_CO > MAX_CO )
		m_nDfltHIGH_CO = 10;

	m_nDfltShowCost = gasApp->ReadProfile( "Defaults", "ShowCost", 0 ).toBool();

	m_nDfltShowMl = gasApp->ReadProfile( "Defaults", "ShowMl", 0 ).toBool();
	 
	m_nDfltShowControl = gasApp->ReadProfile( "Defaults", "ShowControl", 1 ).toBool();
}

void GasPanelView::StopForChange()
{
	bStoppedForChange = false;
	if ( gasdoc->GetState() == RUNNING_STATE ) {
		stop();
		bStoppedForChange = true;
	}
}

// Stop our ticker.
void GasPanelView::StopTicker()
{
	stepTimer->stop();
	disconnect( stepTimer, SIGNAL( timeout() ), this, SLOT( nextStep() ) );
}

// Start our ticker.
void GasPanelView::StartTicker()
{
	int nSpeed = gasdoc->GetCurrentSpeed();
	 
	switch ( nSpeed ) {
		case 1: 
		case 2:
		case 5:
		case 10: 
			m_nTickSize = 1000 / nSpeed; break;
		case AFAP:
			m_nTickSize = 250; break;
		default:
			m_nTickSize = 100; break;
	}

	connect( stepTimer, SIGNAL( timeout() ), this, SLOT( nextStep() ) );
	stepTimer->start( m_nTickSize );
}

void GasPanelView::ChangeCircuit( const QString &circuit )
{	
	StopForChange();
	
	if (gasdoc->CheckChange())
	{
		m_strCircuit = circuit;
		ui->m_Srynge->setEnabled( gasdoc->CanInject( m_nGas ) );
		gasdoc->SetCircuit( this, m_strCircuit );
	}
		
	if ( bStoppedForChange ) {
		run();
	}
}

void GasPanelView::ChangeWeight( double weight_old )
{
	UpdateData( true );
	
	StopForChange();

	if ( gasdoc->CheckChange() )
		gasdoc->SetWeight( this, weight_old );
		
	if ( bStoppedForChange ) {
		run();
	}
}

void GasPanelView::Srynge()
{
	if ( !gasdoc->CanInject( m_nGas ) ) return;			//Because we don't gray it out
	if ( gasdoc->CheckChange( true ) )
		gasdoc->Inject();
}

void GasPanelView::flush()
{
	if ( gasdoc->CheckChange( true ) )
		gasdoc->Flush();
}

void GasPanelView::hideNumerics( bool set )
{
	ui->m_Gasmchn->hideNumerics( set );
	hiddenNumerics = set;
}

void GasPanelView::showCost( bool set )
{	
	m_bShowUptakeCost = set;
	QString strTemp, strNew;
	int idx;

	strNew = set ? tr( "($)" ) : m_bShowMl ? tr( "(mL)" ) : tr( "(L)" );

	strTemp = ui->lUptake->text();
	idx = strTemp.indexOf( tr( "(" ) );
	strTemp.replace( idx, strTemp.length() - idx, strNew );
	ui->lUptake->setText( strTemp );
		
	strTemp = ui->lDelivered->text();
	idx = strTemp.indexOf( tr( "(" ) );
	strTemp.replace( idx, strTemp.length() - idx, strNew );
	ui->lDelivered->setText( strTemp );

	FetchUptake();
	FetchDelivered();
	UpdateData( false );
}

void GasPanelView::showMl( bool set )
{
	m_bShowMl = set;
	showCost(m_bShowUptakeCost);
}

void GasPanelView::changeAgent()
{
	StopForChange();
	if ( !(gasdoc->CheckChange()) ) return;

	GasAgentDlg dlg( this );

	int nGas = gasdoc->m_nActiveGas;
	dlg.setAgent(gasdoc->m_gasArray.at( nGas )->m_strAgent);	
	dlg.setAgentChangeable( gasdoc->GetTime() == 0.0F );
	dlg.setCircuit( m_strCircuit );
	dlg.setArray( &( gasdoc->m_anesArray ) );

	if ( dlg.exec() == QDialog::Rejected )
		return;

	if ( m_strAgent != dlg.agent() && !CheckAgentChange( dlg.agent(), nGas ) ) return;

	if ( m_strCircuit != dlg.circuit() ) {
		m_strCircuit = dlg.circuit();
		ui->m_Srynge->setEnabled( gasdoc->CanInject( m_nGas ) );
		gasdoc->SetCircuit( this, m_strCircuit );
	}
}

bool GasPanelView::CheckAgentChange( QString const &strAgent, const int nGas )
{
	if( gasdoc->GetTime() != 0.0F )
		return false;

	int nNew = gasdoc->m_anesArray.Lookup( strAgent );
	if( nNew < 0 )
		return false;

	gasdoc->SetAgent((QWidget *)-1L, nGas, strAgent);
	gasdoc->setColorToDefault( nGas );
	gasdoc->updateTitle();
	return true;
}

// This routine is called when the anesthetic agent is changed
// to reset values dependent upon agent.
void GasPanelView::SetAnesParams()
{
	GasAnesArray &anes = gasdoc->m_anesArray;
	int nAgent = anes.Lookup( m_strAgent );

	// Get the parameters we read from gas.ini

	Q_ASSERT( nAgent >= 0 );

	// Leave what the user set if the agent hasn't changed

	if ( nAgent == m_nLastAgent ) return;

	// New agent -- Get the parameters

	MAX_DEL   = anes[nAgent]->m_fMAX_DEL;
	ivDEL->setRange( 1, int( MAX_DEL ) );
	FMT_DEL   = anes[nAgent]->m_strFmt;
	ui->leHighDEL->setText( QString::number( anes[nAgent]->m_wHighDEL ) );

	int temp  = anes[nAgent]->m_wHighFGF;
	if(temp!= 0)
		ui->leHighFGF->setText(QString::number(temp));
	temp = anes[nAgent]->m_wHighVA;
	if(temp!=0)
		ui->leHighVA->setText(QString::number(temp));
	temp =anes[nAgent]->m_wHighCO;
	if(temp !=0)
		ui->leHighCO->setText(QString::number(temp));
	m_fMAC    = anes[nAgent]->m_fMAC;

	SetDELHIGH();
	
	if ( ui->leDEL->text().toDouble() > MAX_DEL )
		ui->leDEL->setText( FormatToFMT_DEL( MAX_DEL ) );

	if ( !anes[nAgent]->m_bIsLiquid )
		showMl(false);

	m_nLastAgent = nAgent;
}

// Set the values that depend on the max delivered setting

void GasPanelView::SetDELHIGH()
{

	// Like above, make sure we repaint when this is over...
	 
	int high_del = ui->leHighDEL->text().toInt();

	m_cnCKT =(short)( m_fLastCKT * 100.0 / (float)high_del );
	if ( m_cnCKT > 100)
		m_cnCKT = 100;

	m_cnALV = (short)( m_fLastALV * 100.0 / (float)high_del );
	if ( m_cnALV > 100 )
		m_cnALV = 100;

	m_cnART = (short)( m_fLastART * 100.0 / (float)high_del );
	if ( m_cnART > 100 )
		m_cnART = 100;

	m_cnVRG = (short)( m_fLastVRG * 100.0 / (float)high_del );
	if ( m_cnVRG > 100 )
		m_cnVRG = 100;

	m_cnMUS = (short)( m_fLastMUS * 100.0 / (float)high_del );
	if ( m_cnMUS > 100 )
		m_cnMUS = 100;

	m_cnFAT = (short)( m_fLastFAT * 100.0 / (float)high_del );
	if ( m_cnFAT > 100 )
		m_cnFAT = 100;

	m_cnVEN = (short)( m_fLastVEN * 100.0 / (float)high_del );
	if ( m_cnVEN > 100 )
		m_cnVEN = 100;

	m_cnMAC = (short)( m_fMAC * 100.0 / (float)high_del );
	if ( m_cnMAC > 100 )
		m_cnMAC = 0;
}

// Fetch all the document info we need to display

void GasPanelView::FetchData( const bool bInitialize )		
{
	int nSpeed;
	
	m_strAgent = gasdoc->GetAgent( m_nGas )->m_strName;
	SetAnesParams();
	m_strCircuit = gasdoc->GetCircuit();
	if ( ( nSpeed = gasdoc->GetSpeed() ) !=AFAP )
		m_strSpeed = QString( "%1x" ).arg( nSpeed );
	else
		m_strSpeed = tr( "AFAP" );
	
	ui->leDEL->setText( FormatToFMT_DEL( gasdoc->GetDEL( m_nGas ) ) );
	if ( !ui->leFGF->isReadOnly() ) ui->leFGF->setText( QString::number( gasdoc->GetFGF(), 'f', 2 ) );
	if ( !ui->leVA->isReadOnly() ) ui->leVA->setText( QString::number( gasdoc->GetVA(), 'f', 2 ) );
	ui->leCO->setText( QString::number( gasdoc->GetCO(), 'f', 2 ) );
	
	// If this a saved doc, restore some parameters
	if ( bInitialize && gasdoc->m_bOpened ) {
		m_bShowUptakeCost = gasdoc->save_bShowUptakeCost;
		m_bShowMl = gasdoc->save_bShowMl;
		ui->leHighFGF->setText( QString::number( gasdoc->save_HIGH_FGF ) );
		ui->leHighVA->setText( QString::number( gasdoc->save_HIGH_VA ) );
		ui->leHighCO->setText( QString::number( gasdoc->save_HIGH_CO ) );
	}
	FetchCKT();
	FetchALV();
	FetchART();
	FetchVRG();
	FetchMUS();
	FetchFAT();
	FetchVEN();
	FetchTime();
	FetchUptake();
	FetchDelivered();
	FetchVAOut();
	FetchFGFOut();
}

QString GasPanelView::FormatToFMT_DEL(const float x)
{
	int fieldWidth = FMT_DEL.at(1).digitValue();
	char format = FMT_DEL.at(4).toLatin1();
	int precision = FMT_DEL.at(3).digitValue();

	return QString( "%1" ).arg( x, fieldWidth, format, precision ).trimmed();
}

bool GasPanelView::FetchCKT()
{
	float x = gasdoc->GetCKT( m_nGas );
	if ( x == m_fLastCKT ) return false;
	
	m_fLastCKT = x;
	m_cnCKT = (short)( x * 100.0 / ui->leHighDEL->text().toFloat() );
	if ( m_cnCKT > 100 )
		m_cnCKT = 100;
	
	if ( m_cnCKT < 0 )
		m_cnCKT = 0;

	ui->m_Gasmchn->SetGasValueCKT( FormatToFMT_DEL(x) );
	return true;
}

bool GasPanelView::FetchALV()
{
	float x = gasdoc->GetALV( m_nGas );
	if ( x == m_fLastALV ) return false;

	m_fLastALV = x;
	m_cnALV = (short)( x * 100.0 / ui->leHighDEL->text().toFloat() );
	if ( m_cnALV > 100 )
		m_cnALV = 100;

	if ( m_cnALV < 0 )
		m_cnALV = 0;

	ui->m_Gasmchn->SetGasValueALV( FormatToFMT_DEL(x) );

	return true;
}

bool GasPanelView::FetchART()
{
	float x = gasdoc->GetART( m_nGas );
	if ( x == m_fLastART ) return false;

	m_fLastART = x;
	m_cnART = (short)( x * 100.0 / ui->leHighDEL->text().toFloat() );
	if ( m_cnART > 100 )
		m_cnART = 100;

	if ( m_cnART < 0 )
		m_cnART = 0;

	ui->m_Gasmchn->SetGasValueART( FormatToFMT_DEL(x) );

	return true;
}

bool GasPanelView::FetchVRG()
{
	float x = gasdoc->GetVRG( m_nGas );
	if ( x == m_fLastVRG ) return false;

	m_fLastVRG = x;
	m_cnVRG = (short)( x * 100.0 / ui->leHighDEL->text().toFloat() );
	if ( m_cnVRG > 100 )
		m_cnVRG = 100;

	if ( m_cnVRG < 0 )
		m_cnVRG = 0;

	ui->m_Gasmchn->SetGasValueVRG( FormatToFMT_DEL(x) );

	return true;
}

bool GasPanelView::FetchMUS()
{
	float x = gasdoc->GetMUS( m_nGas );
	if ( x == m_fLastMUS ) return false;

	m_fLastMUS = x;
	m_cnMUS = (short)( x * 100.0 / ui->leHighDEL->text().toFloat() );
	if ( m_cnMUS > 100 )
		m_cnMUS = 100;

	if ( m_cnMUS < 0 )
		m_cnMUS = 0;

	ui->m_Gasmchn->SetGasValueMUS( FormatToFMT_DEL(x) );

	return true;
}

bool GasPanelView::FetchFAT()
{
	float x = gasdoc->GetFAT( m_nGas );
	if ( x == m_fLastFAT ) return false;

	m_fLastFAT = x;
	m_cnFAT = (short)( x * 100.0 / ui->leHighDEL->text().toFloat() );
	if ( m_cnFAT > 100 )
		m_cnFAT = 100;

	if ( m_cnFAT < 0 )
		m_cnFAT = 0;

	ui->m_Gasmchn->SetGasValueFAT( FormatToFMT_DEL(x) );

	return true;
}

bool GasPanelView::FetchVEN()
{
	float x = gasdoc->GetVEN( m_nGas );
	if ( x == m_fLastVEN ) return false;

	m_fLastVEN = x;
	m_cnVEN = (short)( x * 100.0 / ui->leHighDEL->text().toFloat() );
	if ( m_cnVEN > 100 )
		m_cnVEN = 100;

	if ( m_cnVEN < 0 )
		m_cnVEN = 0;

	ui->m_Gasmchn->SetGasValueVEN( FormatToFMT_DEL(x) );

	return true;
}

bool GasPanelView::FetchUptake()
{
	float x;
	if ( m_bShowUptakeCost )
		x = gasdoc->GetUptakeCost( m_nGas );
	else
	{
		x = gasdoc->GetUptake( m_nGas );
		if ( m_bShowMl )
		{
			GasAnesthetic &anes = *gasdoc->m_anesArray[gasdoc->m_gasArray.at( m_nGas )->m_nAgent];
			x = anes.ToMlLiquid(x);
		}
	}

	if ( x == m_fLastUptake ) return false;
	
	m_fLastUptake = x;

	ui->leUptake->setText( QString( "%1" ).arg( x, 0, 'f', x >= 1000? 0 : 2 ) );

	return true;
}

bool GasPanelView::FetchDelivered()
{
	float x;
	if ( m_bShowUptakeCost )
		x = gasdoc->GetDeliveredCost( m_nGas );
	else
	{
		x = gasdoc->GetDelivered( m_nGas );
		if ( m_bShowMl )
		{
			GasAnesthetic &anes = *gasdoc->m_anesArray[gasdoc->m_gasArray.at( m_nGas )->m_nAgent];
			x = anes.ToMlLiquid(x);
		}
	}
	
	if ( x == m_fLastDelivered ) return false;
	
	m_fLastDelivered = x;

	ui->leDelivered->setText( QString( "%1" ).arg( x, 0, 'f', x >= 1000? 0 : 2 ) );

	return true;
}

bool GasPanelView::FetchTime()
{
	float x = gasdoc->GetTime();
	if ( x == m_fLastTime ) return false;
	m_fLastTime = x;
	int d = (int)(x/86400.0F);
	x -= d*86400.0F;
	int hh = (int)(x/3600.0F);
	int mm = (int)((x-hh*3600.0F)/60.0F);
	int ss = (int)(x-((hh*60)+mm)*60.0F);
	QString display = QTime( hh, mm, ss ).toString( "hh:mm:ss" );
	if (d > 0) 	{
		display.prepend(QString( "%1." ).arg(d));
		if( ui->lTime->text() != tr( "(d.hh.mm.ss)" ) )
			ui->lTime->setText( tr( "(d.hh.mm.ss)" ) );
	} 
	else if( ui->lTime->text() != tr( "(hh.mm.ss)" ) )
		ui->lTime->setText( tr( "(hh.mm.ss)" ) );
	ui->leTime->setText( display );
	return true;
}

bool GasPanelView::FetchVAOut()
{
	if ( !ui->leVA->isReadOnly() ) return false; 
	
	float x = gasdoc->GetVA(m_fLastTime/60.0F);
	QString txt = QString::number( x, 'f', 2 );
	if ( txt != ui->leVA->text() )
		ui->leVA->setText( txt );
	return true;
}

bool GasPanelView::FetchFGFOut()
{
	if ( !ui->leFGF->isReadOnly() ) return false; 
	
	float x = gasdoc->GetFGF(m_fLastTime/60.0F, m_nGas);
	QString txt = QString::number( x, 'f', 2 );
	if ( txt != ui->leFGF->text() )
		ui->leFGF->setText( txt );
	return true;
}

// Processing of pressing of all Up/Down arrow-buttons
void GasPanelView::upDownValue()
{
	QToolButton *tb = qobject_cast<QToolButton *>( sender() );
	if ( !tb ) return;

	SemaphoreHolder holder(&LeChangeSemaphore);
	if ( !holder.IsAcquired() )
		return;

	if ( tb->isDown() )
	{
		int interval = tb->autoRepeatInterval();
		if ( interval > 10 )
			tb->setAutoRepeatInterval( interval - 5 );
	}
	else
		tb->setAutoRepeatInterval( repeatInterval );

	double val;
	QLineEdit *le, *hiLe = NULL;
	bool up = true;

	if ( tb == ui->tbDELup ) {
		float delta = 5 * pow ( 10.0, -FMT_DEL.at(3).digitValue() );
		val = gasdoc->GetDEL( m_nGas ) + delta;
		le = ui->leDEL;
		hiLe = ui->leHighDEL;
	}
	else if ( tb == ui->tbDELdown ) {
		up = false;
		float delta = 5 * pow ( 10.0, -FMT_DEL.at(3).digitValue() );
		val = gasdoc->GetDEL( m_nGas ) - delta;
		le = ui->leDEL;
	}
	else if ( tb == ui->tbFGFup ) {
		val = gasdoc->GetFGF() + 0.05;
		le = ui->leFGF;
		hiLe = ui->leHighFGF;
	}
	else if ( tb == ui->tbFGFdown ) {
		up = false;
		val = gasdoc->GetFGF() - 0.05;
		le = ui->leFGF;
	}
	else if ( tb == ui->tbVAup ) {
		val = gasdoc->GetVA() + 0.05;
		le = ui->leVA;
		hiLe = ui->leHighVA;
	}
	else if ( tb == ui->tbVAdown ) {
		up = false;
		val = gasdoc->GetVA() - 0.05;
		le = ui->leVA;
	}
	else if ( tb == ui->tbCOup ) {
		val = gasdoc->GetCO() + 0.05;
		le = ui->leCO;
		hiLe = ui->leHighCO;
	}
	else if ( tb == ui->tbCOdown ) {
		up = false;
		val = gasdoc->GetCO() - 0.05;
		le = ui->leCO;
	}
	else le = NULL; // keep compiler happy

	double newVal;
	if (up)
		newVal = qMin( val, (double)hiLe->text().toInt() );
	else
		newVal = qMax( val, 0.0 );

	QString newText;
	if (le == ui->leDEL)
		newText = FormatToFMT_DEL(newVal);
	else
		newText = QString::number( newVal, 'f', 2 );

	le->setText( newText );
	changed( le, tb);
}

// Processing of "value changed" in lineEdits of the gauge controlls
void GasPanelView::changed( QWidget *widget, QToolButton *tb )
{
	QLineEdit *le = qobject_cast<QLineEdit *>( widget ? widget : sender() );
	if ( !le ) return;

	if ( NULL == tb )
	{
		float val = -1.0;
		if ( le == ui->leFGF )
			val = gasdoc->GetFGF();
		else if ( le == ui->leDEL)
			val = gasdoc->GetDEL( m_nGas );
		else if ( le == ui->leVA)
			val = gasdoc->GetVA();
		else if ( le == ui->leCO)
			val = gasdoc->GetCO();

		QString newText;
		if (le == ui->leDEL)
			newText = FormatToFMT_DEL(val);
		else
			newText = QString::number( val, 'f', 2 );

		if ( newText == le->text() )
			return;
	}
	
	StopForChange();

	bool dialogStatus;
	if ( gasdoc->CheckChange( true, &dialogStatus ) )
	{
		GasCallbackEvent<GasPanelView> *pEvent = new GasCallbackEvent<GasPanelView>( &GasPanelView::LeChanged, le );
		pEvent->PushArg( (void *)(widget) );
		gasApp->postEvent( this, pEvent );
	}
	else if ( bStoppedForChange )
		run();

	if ( dialogStatus && tb != NULL )
	{
		tb->setDown(false);
		tb->setAutoRepeatInterval( repeatInterval );
	}
}

void GasPanelView::LeChanged( QWidget *widget, GasCallbackEvent<GasPanelView> *pEvent )
{
	QLineEdit *le = qobject_cast<QLineEdit *>( widget );
	bool bFocus = ( pEvent->PopArg() != NULL );

	double val = le->text().toDouble();
	if ( le == ui->leFGF ) {
		ui->ggFGF->SetValue( (int)(val * GSCALE ) );
		gasdoc->SetFGF( (QWidget *)-1, val );
	}
	else if ( le == ui->leDEL ) {
		ui->ggDEL->SetValue( (int)(val * GSCALE ) );
		gasdoc->SetDEL( (QWidget *)-1, m_nGas, val );
	}
	else if ( le == ui->leVA ) {
		ui->ggVA->SetValue( (int)(val * GSCALE ) );
		gasdoc->SetVA( (QWidget *)-1, val );
	}
	else if ( le == ui->leCO ) {
		ui->ggCO->SetValue( (int)(val * GSCALE ) );
		gasdoc->SetCO( (QWidget *)-1, val );
	}

	if ( bFocus )
	{
		le->home( false );
		le->setFocus();
	}
}

// Processing of "value changed" in "High"-lineEdits of gauge-controls
// THIS IS A HACK. SHOULD NOT BE REACHING INTO ANOTHER VIEW!
void GasPanelView::maxChanged()
{
	QLineEdit *le = qobject_cast<QLineEdit *>( sender() );
	if ( !le )
		return;
	int value = le->text().toInt();
	
	UpdateData( true );
	
 	GasAnesArray &anes = gasdoc->m_anesArray;
 	int nAgent = anes.Lookup( m_strAgent );

	GasChildren  children =  gasdoc->gasChildren();
	QString agent = gasdoc->GetAgent(m_nGas)->m_strName;
	if ( le == ui->leHighDEL ) {
		int pos=0;
		QString te = le->text();
		if(ivDEL->validate(te,pos)!=QValidator::Acceptable){
			QMessageBox::warning(this, tr( "Gas Man\xC2\xAE"),tr("The input is not valid, please check and input it again."),QMessageBox::Ok,QMessageBox::Ok);	
			le->setText(QString("%1").arg(anes[nAgent]->m_wHighDEL));
			return;
		}
		foreach(GasChildWindow * child , children)
				child->setWindowModified( true );
	 	anes[nAgent]->m_wHighDEL = value;
		SetDELHIGH();
		DisplayExtraData();
		 
		foreach(GasChildWindow * child , children){
			if(agent == gasdoc->GetAgent(child->panelView()->m_nGas)->m_strName){		
				child->graphView()->SetDELmax( value );	
				child->graphView()->SetIARMmax(value);
			}
		}
	} else if ( le == ui->leHighFGF ) {
		anes[nAgent]->m_wHighFGF = value;
		SetGauge( ui->ggFGF, value );
		ui->ggFGF->SetValue( (int)( qMin( gasdoc->GetFGF(), (float)value ) * GSCALE ) );
		foreach(GasChildWindow * child , children){
			if(agent == gasdoc->GetAgent(child->panelView()->m_nGas)->m_strName)		
				child->graphView()->SetFGFmax( value );
		}
	} else if ( le == ui->leHighVA ) {
 		anes[nAgent]->m_wHighVA = value;
		SetGauge( ui->ggVA, value );
		ui->ggVA->SetValue( (int)( qMin( gasdoc->GetVA(), (float)value ) * GSCALE ) );
		foreach(GasChildWindow * child , children){
			if(agent == gasdoc->GetAgent(child->panelView()->m_nGas)->m_strName)
				child->graphView()->SetVAmax( value );
		}
	}
	else if ( le == ui->leHighCO ) {
		anes[nAgent]->m_wHighCO = value;
		SetGauge( ui->ggCO, value );
		 
		ui->ggCO->SetValue( (int)( qMin( gasdoc->GetCO(), (float)value ) * GSCALE ) );
		
		foreach(GasChildWindow * child , children){
			if(agent == gasdoc->GetAgent(child->panelView()->m_nGas)->m_strName){		
				child->graphView()->SetCOmax( value );
				child->graphView()->SetIARMmax( ui->leHighDEL->text().toInt() );
			}
		}
	}
}

// Verify values of lineEdits and "High" lineEdits
void GasPanelView::verify( const QString & )
{
	QObject *s = sender();
	bool incorrect = false;
	QPalette p;

	if ( s == ui->leDEL || s == ui->leHighDEL ) {
		incorrect = ui->leDEL->text().toDouble() > ui->leHighDEL->text().toInt();
		p = ui->leDEL->palette();
		p.setColor( QPalette::Text, incorrect ? Qt::red : Qt::black );
		ui->leDEL->setPalette( p );
	}
	else if ( s == ui->leFGF || s == ui->leHighFGF ) {
		incorrect = ui->leFGF->text().toDouble() > ui->leHighFGF->text().toInt();
		p = ui->leFGF->palette();
		p.setColor( QPalette::Text, incorrect ? Qt::red : Qt::black );
		ui->leFGF->setPalette( p );
	}
	else if ( s == ui->leVA || s == ui->leHighVA ) {
		incorrect = ui->leVA->text().toDouble() > ui->leHighVA->text().toInt();
		p = ui->leVA->palette();
		p.setColor( QPalette::Text, incorrect ? Qt::red : Qt::black );
		ui->leVA->setPalette( p );
	}
	else if ( s == ui->leCO || s == ui->leHighCO ) {
		incorrect = ui->leCO->text().toDouble() > ui->leHighCO->text().toInt();
		p = ui->leCO->palette();
		p.setColor( QPalette::Text, incorrect ? Qt::red : Qt::black );
		ui->leCO->setPalette( p );
	}

	QLineEdit *le = qobject_cast<QLineEdit *>( s );
	if ( le && !le->hasFocus() )
		le->home( false );
}

void GasPanelView::gasgaugeChangeStart()
{
	GasGauge *gauge = qobject_cast<GasGauge *>( sender() );
	if ( !gauge ) return;

	StopForChange();
	bool dialogStatus;
	if ( !gasdoc->CheckChange( true, &dialogStatus ) ) {	//User doesn't want to change saved simulation
		if ( bStoppedForChange )
			run();
		return;
	}

	if ( gauge == ui->ggVA )
		setLeEnabled( ui->leVA, true );

	if ( gauge == ui->ggFGF )
		setLeEnabled( ui->leFGF, true );

	gauge->SetStartStatus(!dialogStatus);
}

void GasPanelView::gasgaugeChangeGauge( short value )
{
	GasGauge *gauge = qobject_cast<GasGauge *>( sender() );
	if ( !gauge ) return;

	float f = (float)value/GSCALE;
	
	if ( gauge == ui->ggDEL )
		ui->leDEL->setText( FormatToFMT_DEL( f ) );
	else if ( gauge == ui->ggFGF )
		ui->leFGF->setText( QString::number( f, 'f', 2 ) );
	else if ( gauge == ui->ggVA )
		ui->leVA->setText( QString::number( f, 'f', 2 ) );
	else if ( gauge == ui->ggCO )
		ui->leCO->setText( QString::number( f, 'f', 2 ) );
}

void GasPanelView::gasgaugeChangeStop()
{
	GasGauge *gauge = qobject_cast<GasGauge *>( sender() );
	if ( !gauge ) return;

	float f = (float)gauge->GetValue()/GSCALE;

	if ( gauge == ui->ggDEL )
		gasdoc->SetDEL( this, m_nGas, f );
	else if ( gauge == ui->ggFGF )
		gasdoc->SetFGF( this, f );
	else if ( gauge == ui->ggVA )
		gasdoc->SetVA( this, f );
	else if ( gauge == ui->ggCO )
		gasdoc->SetCO( this, f );

	if ( bStoppedForChange )
		run();
}

void GasPanelView::ChangeAgent( const QString &agent )
{	
	bool bViewChange = false;

	StopForChange();

	// If this agent is already part of the sim, don't warn about
	// changing the future of the sim. (bViewChange => don't CheckChange())

	int nNew = gasdoc->m_anesArray.Lookup( agent );
	Q_ASSERT( nNew >= 0 );
	for ( int i = 0; i < gasdoc->m_gasArray.size(); ++i )
		if ( gasdoc->m_gasArray.at(i)->m_nAgent == nNew ) {
			bViewChange = true;
			break;
		}

	if ( bStoppedForChange ) {
		run();
		//bStoppedForChange = false;
	}
}

void GasPanelView::ChangeSpeed( const QString &speed )
{
	StopForChange();
	
	QString strSpeed = speed;
	strSpeed.chop( 1 );
	
	short nSpeed = AFAP;
	
	if ( speed == tr( "AFAP" ) || ( ( nSpeed = strSpeed.toInt() ) != 0 ) ) 	{
		m_strSpeed = speed;
		gasdoc->SetSpeed( this, nSpeed );
	}
	 
	if ( bStoppedForChange ) {
		run();
		//bStoppedForChange = false;
	}
}
void GasPanelView::SetSpeed(const QString & speed)
{
	QString strSpeed = speed;
	strSpeed.chop( 1 );
	
	short nSpeed = AFAP;
	
	if ( speed == tr( "AFAP" ) || ( ( nSpeed = strSpeed.toInt() ) != 0 ) ) 	{
		m_strSpeed = speed;
		gasdoc->SetSpeed( this, nSpeed );
	}
	
}
void GasPanelView::stop()
{
	StopTicker();
	gasdoc->ChangeState( this, STOPPED_STATE );
}

void GasPanelView::start()
{
	gasdoc->ChangeState( this, RUNNING_STATE );
	m_dwTickCount = 0;
	m_dwTickTimer = (long)( gasdoc->GetTime() * 1000 );
	StartTicker();
	tickCounter.start();
}

void GasPanelView::run()
{	
	if(!gasdoc->duplicateAgents())  
	{
		switch ( gasdoc->GetState() )
		{
		case RUNNING_STATE:
			stop();
			break;
		case STOPPED_STATE:
		case INITIAL_STATE:
			start();
			break;
		case END_STATE:
			break;
		default:
			Q_ASSERT( false );
		}
	}else{
		QMessageBox::information( this, tr( "Gas Man\xC2\xAE" ), tr( "<p>Two of the same agents are being administered. You must remove one before starting the simulation." ) );		
	}
}

void GasPanelView::UpdateData( bool bSaveAndValidate )
{
	if ( !bSaveAndValidate )
		DisplayExtraData();
}

void GasPanelView::DisplayExtraData()
{
	int fgf = ui->leHighFGF->text().toInt();
	int va = ui->leHighVA->text().toInt();
	int co = ui->leHighCO->text().toInt();
	int del = ui->leHighDEL->text().toInt();
	
	SetGauge( ui->ggFGF, fgf );
	SetGauge( ui->ggVA, va );
	SetGauge( ui->ggCO, co );
	SetGauge( ui->ggDEL, del );

	ui->ggFGF->SetValue( (int)( qMin( gasdoc->GetFGF(), (float)fgf ) * GSCALE ) );
	ui->ggVA->SetValue( (int)( qMin( gasdoc->GetVA(), (float)va ) * GSCALE ) );
	ui->ggCO->SetValue( (int)( qMin( gasdoc->GetCO(), (float)co ) * GSCALE ) );
	ui->ggDEL->SetValue( (int)( qMin( gasdoc->GetDEL( m_nGas ), (float)del ) * GSCALE ) );
	
	ui->m_Gasmchn->SetMAC( m_cnMAC );
	ui->m_Gasmchn->SetCKT( m_cnCKT );
	ui->m_Gasmchn->SetALV( m_cnALV );
	ui->m_Gasmchn->SetART( m_cnART );
	ui->m_Gasmchn->SetVRG( m_cnVRG );
	ui->m_Gasmchn->SetMUS( m_cnMUS );
	ui->m_Gasmchn->SetFAT( m_cnFAT );
	ui->m_Gasmchn->SetVEN( m_cnVEN );

	ui->m_Srynge->setEnabled( gasdoc->CanInject( m_nGas ) );
}

// Set up the machine's gauges

void GasPanelView::SetGauge( GasGauge *pctl, int cMax )
{
	int nLog = (int)(pow( 10.0, floor(log10((double)cMax)))+0.5);
	int mantissa = cMax/nLog;
	int nTick, nLTick;

	if ( nLog < 10 ) {
		nTick = 1;
		nLTick = 5;
	}
	else if ( mantissa < 2 ) {
		nTick = nLog/10;
		nLTick = nTick * 5;
	}
	else if ( mantissa < 4 ) {
		nTick = nLog/5;
		nLTick = nLog;
	}
	else if ( mantissa < 5 ) {
		nTick = nLog/2;
		nLTick = nLog;
	}
	else {
		nTick = nLog/2;
		nLTick = nTick * 5;
	}

	pctl->SetMax( cMax * GSCALE );
	pctl->SetTickEvery( nTick * GSCALE );
	pctl->SetLTickEvery( nLTick * GSCALE );
}

// Filter for different events from gauge-contol lineEdits 
bool GasPanelView::eventFilter( QObject *obj, QEvent *evnt )
{
	if  ( ( obj == ui->leFGF ) || ( obj == ui->leVA ) ) {
		QLineEdit * le = static_cast<QLineEdit *>(obj);
		if ( evnt->type() == QEvent::MouseButtonRelease || evnt->type() == QEvent::FocusIn ) {
			if ( le->isReadOnly() ) {
				le->setText( QString::number( ( obj == ui->leFGF ) ? gasdoc->GetFGF() : gasdoc->GetVA(), 'f', 2 ) );
				setLeEnabled( le, true );
				le->setFocus();
			}
		}
	}
	return QWidget::eventFilter( obj, evnt );
}

void GasPanelView::customEvent( QEvent *evnt )
{
	if ( GasCallbackEvent<GasPanelView>::TryCallback( this, evnt ) )
		return;

	if ( evnt->type() != (QEvent::Type)GasUpdateQEvent )
		return;

	GasUpdateEvent *gasevent = static_cast<GasUpdateEvent * >( evnt );
	if ( !gasevent ) return;
	
	GasDoc *pDoc = gasevent->document();
	if ( !pDoc ) return;

	int nGas = gasevent->gas();
	if ( nGas && (( nGas - 1 ) != m_nGas ) ) return;

	switch ( gasevent->type() )
	{
	case GasUpdateEvent::UPD_AGENT: {
			m_strAgent = pDoc->GetAgent( m_nGas )->m_strName;
			ui->lbDEL->setText(tr( "<b>" ) + m_strAgent + tr( "</b><br>DEL %atm" ));
			SetAnesParams();
			UpdateData( false );
		}
		break;
	case GasUpdateEvent::UPD_CIRCUIT: {
			QString m_strCircuit = pDoc->GetCircuit();
			ui->m_Gasmchn->SetGasLabelCKT(tr("CKT")+'('+m_strCircuit[0]+')');
			ui->m_Srynge->setEnabled( gasdoc->CanInject( m_nGas ) );
		}
		break;
	case GasUpdateEvent::UPD_SPEED: {
			int nSpeed = pDoc->GetSpeed();
			QString m_strSpeed;
			if ( nSpeed != AFAP )
				m_strSpeed = QString( "%1x" ).arg( nSpeed );
			else
				m_strSpeed = tr( "AFAP" );
		}
		break;
	case GasUpdateEvent::UPD_DEL: {
			ui->leDEL->setText( FormatToFMT_DEL( pDoc->GetDEL( m_nGas ) ) );
			ui->ggDEL->SetValue( short( qMin( pDoc->GetDEL( m_nGas ), ui->leHighDEL->text().toFloat() ) * GSCALE ) );
		}
		break;
	case GasUpdateEvent::UPD_FGF: {
			ui->leFGF->setText( QString::number( pDoc->GetFGF(), 'f', 2 ) );
			ui->ggFGF->SetValue( short( qMin( pDoc->GetFGF(), ui->leHighFGF->text().toFloat() ) * GSCALE ) );
			setLeEnabled( ui->leFGF, true );
		}
		break;
	case GasUpdateEvent::UPD_VA: {
			ui->leVA->setText( QString::number( pDoc->GetVA(), 'f', 2 ) );
			ui->ggVA->SetValue( short( qMin( pDoc->GetVA(), ui->leHighVA->text().toFloat() ) * GSCALE ) );
			setLeEnabled( ui->leVA, true );
		}
		break;
	case GasUpdateEvent::UPD_CO: {
			ui->leCO->setText( QString::number( pDoc->GetCO(), 'f', 2 ) );
			ui->ggCO->SetValue( short( qMin( pDoc->GetCO(), ui->leHighCO->text().toFloat() ) * GSCALE ) );
		}
		break;
	case GasUpdateEvent::UPD_WEIGHT:
		break;

	case GasUpdateEvent::UPD_TICK:
		if ( FetchCKT() )
			ui->m_Gasmchn->SetCKT( m_cnCKT );
		if ( FetchALV() )
			ui->m_Gasmchn->SetALV(m_cnALV );
		if ( FetchART() )
			ui->m_Gasmchn->SetART( m_cnART );
		if ( FetchVRG() )
			ui->m_Gasmchn->SetVRG( m_cnVRG );
		if ( FetchMUS() )
			ui->m_Gasmchn->SetMUS( m_cnMUS );
		if ( FetchFAT() )
			ui->m_Gasmchn->SetFAT( m_cnFAT );
		if ( FetchVEN() )
			ui->m_Gasmchn->SetVEN( m_cnVEN );
		FetchTime();
		FetchUptake();
		FetchDelivered();
		
		if ( !ui->leVA->hasFocus () )
			setLeEnabled( ui->leVA, false );
		FetchVAOut();

		if ( !ui->leFGF->hasFocus () )
			setLeEnabled( ui->leFGF, false );
		FetchFGFOut();

		break;
	case GasUpdateEvent::UPD_STATE: {
			AState state = pDoc->GetState();
			switch ( state )
			{
			case STOPPED_STATE:
			case END_STATE:
				StopTicker();	
				FetchData();
				UpdateData( false );
				break;
			case RUNNING_STATE:
			case INITIAL_STATE:
				break;
			default:
				Q_ASSERT( false );
			}
		}
		break;
	case GasUpdateEvent::UPD_NGAS:
		m_nGas = 0;
		break;
	case GasUpdateEvent::UPD_NONE:
		setLeEnabled( ui->leVA, true );
		setLeEnabled( ui->leFGF, true );
		ui->leFGF->setCursorPosition ( 0 );
		FetchData();
		m_strAgent = pDoc->GetAgent( m_nGas )->m_strName;
 		ui->lbDEL->setText(tr( "<b>" ) + m_strAgent + tr( "</b><br>DEL %atm" ));
		UpdateData( false );
		break;
	default:
		Q_ASSERT( false );
	}
}

// The only reason for this is to make the line between the vaporizer and
// the circuit part of the background

void GasPanelView::paintEvent( QPaintEvent * )
{
	int nWidth, nHeight;
	QPoint pt[3];
	QRect rc = ui->ggDEL->geometry();

	QRect rcMchn = ui->m_Gasmchn->GetCKTRect();
	QPoint posMchn = mapFromGlobal( ui->m_Gasmchn->mapToGlobal( rcMchn.topLeft() ) );
	
	if ( !rc.height() )
		return;
	nHeight = rc.height()/3;
	QPoint pRight = mapFromGlobal( mapToGlobal( rc.topRight() ) );
	QPoint pLeft = mapFromGlobal( mapToGlobal( rc.topLeft() ) );
	rc.setLeft ( (3 * pRight.x() +  pLeft.x() ) /4 );
	nWidth = posMchn.x() - rc.left() + rcMchn.width() / 2;
	rc.setRight( rc.left() + nWidth );
	pt[1].setX( rc.right() );
	pt[1].setY( posMchn.y() - 1 );//rc.top()  ); 
	rc.setTop( posMchn.y() +  - nHeight/2 );
	rc.setBottom( rc.top() + nHeight );

	QPainter pdc( this );
	pdc.setRenderHint( QPainter::Antialiasing );
	pdc.drawArc( rc, 0, 180*16);

	pt[0].setX( pt[1].x() + nWidth/12 );
	pt[2].setX( pt[1].x() - nWidth/12 );
	pt[2].setY( pt[1].y() - nHeight/8 );
	pt[0].setY( pt[2].y() );
	pdc.setBrush( Qt::black );
	pdc.drawPolygon( (const QPoint * )&pt, 3 );
}

void GasPanelView::nextStep()
{
	//if ( bStoppedForChange || ( gasdoc->GetState() == STOPPED_STATE )  ) return;

	int nSpeed = gasdoc->GetCurrentSpeed();
	if ( nSpeed == AFAP )
			gasdoc->StepSimulation( -1 );
	else {			
		quint32 dw = m_dwTickCount;
		m_dwTickCount = tickCounter.elapsed();
		if (dw > m_dwTickCount)
			m_dwTickTimer += (long)m_nTickSize * nSpeed;
		else
			m_dwTickTimer += (long)(m_dwTickCount-dw) * nSpeed;
		gasdoc->StepSimulation(m_dwTickTimer);
	}
}

void GasPanelView::mousePressEvent ( QMouseEvent * event )
{
	if ( event->button() == Qt::LeftButton ) {
		QWidget *w = NULL;

		if ( ui->stVA->underMouse() ) {
			w = ui->stVA;
		}
		else if ( ui->stFGF->underMouse() ) {
			w = ui->stFGF;
		}

		if ( w != NULL ) {
			QToolTip::showText( mapToGlobal( event->pos() ), w->toolTip() );
		}
	}
}

////////////////////////////////////////////////////////////////////////
// After resize the size/pos needs to be updated for all form items.
#define MIDDLE( Xo,Wm, Wo) Xo + ( Wm - Wo ) / 2
#define LABELXOFFS(X)  ui->fr##X->pos().x() + (ui->gg##X->geometry().topRight().x() - ui->fr##X->geometry().topLeft().x() - ui->lb##X->width() ) / 2

void GasPanelView::resizeEvent( QResizeEvent *e )
{
	GasViewCommon::resizeEvent( e );

	// move and resize Gasmchn
	int pWidth = width() - 	layout()->margin()*2;
	int pHeight = height() - layout()->margin()*2;
	int mHeight = int( pHeight / 2 );
	
	int x = int( layout()->margin() + pWidth * ( 1 - koefWidthMchn ) + 1 );
	int y = MIDDLE( layout()->margin(), pHeight / 2, mHeight ); 

	ui->m_Gasmchn->move( x, y );
	ui->m_Gasmchn->resize( int( pWidth * koefWidthMchn ), mHeight );
}

void GasPanelView::gasmchnGeometryUpdated()
{
	int x,y,yg,dY;
	int gaugeHeight, controlHeight;
	int gaugeWidth, controlWidth;
	int labelYOffset,labelYOffset2,controlXOffset;
	QRect rec,rec1;
	QPoint pos,pos1;
	QFontMetrics fm( font() );
	int pHeight = height() - layout()->margin()*2;
	
	// Move liquid to injection point
	pos = ui->m_Gasmchn->GetInjectPoint() - QPoint( 0, ui->m_Srynge->size().height() );
	ui->m_Srynge->move( ui->m_Gasmchn->mapFromGlobal( pos ) );

	// Moving and resiging DEL gauge and control
	rec = ui->m_Gasmchn->GetCKTRect();
	pos = mapFromGlobal( ui->m_Gasmchn->mapToGlobal( rec.topLeft() ) );
	gaugeHeight = rec.height();

	dY = ui->leHighDEL->geometry().y() + ui->leHighDEL->geometry().height()/2 - 1;  // Y-Midpoint of leHighDEL  mapped to frame_2
	controlHeight = ui->frDEL->minimumSizeHint().height() < ( gaugeHeight + dY + 2) ? gaugeHeight + dY + 2 : ui->frDEL->minimumSizeHint().height();
	controlWidth = ui->frDEL->width();
	ui->frDEL->resize( controlWidth, controlHeight );
	ui->frDEL->move( layout()->margin() , pos.y() - dY );

	int freeSpace = ( pos.x() - ui->frDEL->geometry().topRight().x() - ui->frFlush->width() - layout()->spacing()*3 );
	freeSpace -= ui->ggDEL->minimumSize().width();
	if ( freeSpace <= 0 ) {
		gaugeWidth = ui->ggDEL->minimumSize().width();
		freeSpace = 0;
	} else {
		 freeSpace /= 3;
		 if ( ( freeSpace + ui->ggDEL->minimumSize().width() ) >= ui->ggDEL->maximumSize().width() * 2 )
			gaugeWidth = ui->ggDEL->maximumSize().width();
		 else
			gaugeWidth = ui->ggDEL->minimumSize().width() +  freeSpace / 2;
	}

	ui->ggDEL->resize( gaugeWidth, gaugeHeight );
	x = ui->frDEL->pos().x() + ui->frDEL->width() + layout()->spacing() + freeSpace / 2;
	ui->ggDEL->move( x, pos.y() );

	controlXOffset = ui->ggDEL->pos().x() - ui->frDEL->pos().x();
	// Moving DEL label
	y = (mapFromGlobal( mapToGlobal( ui->m_Gasmchn->geometry().bottomLeft() ) ) ).y();
	labelYOffset = y - ui->frDEL->pos().y();
	ui->lbDEL->move( qMax(1, LABELXOFFS(DEL)) , y - fm.boundingRect( ui->lbDEL->text() ).height()*2 - 4 );
	
	// Moving Flush
	x = MIDDLE( ui->ggDEL->geometry().topRight().x() + layout()->spacing() , pos.x() - ui->ggDEL->geometry().topRight().x() - layout()->spacing()*2, ui->frFlush->width() );
	y = MIDDLE( pos.y(), rec.height(), ui->frFlush->height() );
	if ( x < ui->ggDEL->geometry().topRight().x() + layout()->spacing() )
		x = ui->ggDEL->geometry().topRight().x() + layout()->spacing();
	ui->frFlush->move( x, y );

	/////////////////////////////////////////////////////////////
	// Moving and resiging FGF gauge and control
	labelYOffset -= ( fm.boundingRect( ui->lbFGF->text() ).height() + 4 );
	ui->ggFGF->resize( gaugeWidth, gaugeHeight );
	x = MIDDLE( ui->frFlush->pos().x(), ui->frFlush->width(), ui->ggFGF->width() );
	yg = MIDDLE( layout()->margin() + layout()->spacing() + pHeight / 2 , pHeight / 2 ,  labelYOffset );
	ui->ggFGF->move( x , yg + dY );

	ui->frFGF->resize( controlWidth, controlHeight );
	ui->frFGF->move( ui->ggFGF->pos().x() - controlXOffset, yg );
	
	labelYOffset2 = ui->frFGF->pos().y() + labelYOffset - fm.boundingRect( ui->lbFGF->text() ).height()- layout()->spacing();
	ui->lbFGF->move(  LABELXOFFS(FGF) , labelYOffset2  );

	////////////////////////////////////
	// Moving and resiging VA gauge and control
	ui->ggVA->resize( gaugeWidth, gaugeHeight );
	x = mapFromGlobal( ui->m_Gasmchn->GetInjectPoint() ).x();
	ui->ggVA->move( x , yg + dY );

	ui->frVA->resize( controlWidth, controlHeight );
	ui->frVA->move( ui->ggVA->pos().x() - controlXOffset, yg );
	
	ui->lbVA->move(  LABELXOFFS(VA) , labelYOffset2 );

	////////////////////////////////////
	// Moving and resiging CO gauge and control
	rec = ui->m_Gasmchn->GetARTRect();
	ui->ggCO->resize( gaugeWidth, gaugeHeight );
	pos = mapFromGlobal( ui->m_Gasmchn->mapToGlobal( rec.bottomLeft() ) );
	x = MIDDLE( pos.x(), rec.width(), ui->ggCO->width() );
	ui->ggCO->move( x , yg + dY );

	ui->frCO->resize( controlWidth, controlHeight );
	ui->frCO->move( ui->ggCO->pos().x() - controlXOffset, yg );
	
	ui->lbCO->move(  LABELXOFFS(CO) , labelYOffset2 );
	
	///////////////////////////////
	// Moving and resizing "frame_t"
	ui->frame_t->resize( ui->frame_t->minimumSizeHint().width(), pHeight / 2);
	rec = ui->m_Gasmchn->GetMUSRect();
	pos = mapFromGlobal( ui->m_Gasmchn->mapToGlobal( rec.topLeft() ) );
	x = ( pos.x() + ui->frame_t->width() <  width() - layout()->margin() ) ? pos.x() :  width() - layout()->margin() - ui->frame_t->width();
	ui->frame_t->move( x, layout()->margin() + layout()->spacing() + pHeight / 2 );

	update();	
}
#undef LABELXOFFS
#undef MIDDLE
////////////////////////////////////////////////////////////////////////

int GasPanelView::highDELValue() const {
//	if(gasdoc->m_fDELSetting ==0)
	GasAnesArray &anes = gasdoc->m_anesArray;
 	int nAgent = anes.Lookup( m_strAgent );
	if( anes[nAgent]->m_wHighDEL !=0)
		return anes[nAgent]->m_wHighDEL;
	else
		return ui->leHighDEL->text().toInt();
//	else 
// 		return gasdoc->m_fDELSetting ;
}  
int GasPanelView::highFGFValue() const 
{ 
	GasAnesArray &anes = gasdoc->m_anesArray;
 	int nAgent = anes.Lookup( m_strAgent );
	if(anes[nAgent]->m_wHighFGF==0)
		return ui->leHighFGF->text().toInt();
	else
		return anes[nAgent]->m_wHighFGF;
//	return ui->leHighFGF->text().toInt(); 
}
int GasPanelView::highVAValue() const 
{ 
	GasAnesArray &anes = gasdoc->m_anesArray;
 	int nAgent = anes.Lookup( m_strAgent );
	if(anes[nAgent]->m_wHighVA!=0)	
		return anes[nAgent]->m_wHighVA;
	else
		return ui->leHighVA->text().toInt(); 
}
int GasPanelView::highCOValue() const
{ 
	GasAnesArray &anes = gasdoc->m_anesArray;
 	int nAgent = anes.Lookup( m_strAgent );
	if(anes[nAgent]->m_wHighCO)
		return anes[nAgent]->m_wHighCO;
	else
	return ui->leHighCO->text().toInt(); 
}



void GasPanelView::setHighDELValue( int i )
{
	ui->leHighDEL->setText( QString::number( i ) );
}

void GasPanelView::setLeEnabled( QLineEdit *le, bool f )
{
	le->setReadOnly( !f );
	float x = 0, x1 = 0;	
	if ( le == ui->leFGF ) {
		if ( gasdoc->GetState() != INITIAL_STATE ) {
			x = gasdoc->GetFGF(m_fLastTime/60.0F, m_nGas);
			x1 = gasdoc->GetFGF();
		}
		if ( !f && ( x != x1 ) )
			ui->stFGF->setVisible( true );
		else
			ui->stFGF->setVisible( false );
	} else {
		if ( gasdoc->GetState() != INITIAL_STATE ) {
			x = gasdoc->GetVA(m_fLastTime/60.0F );
			x1 = gasdoc->GetVA();
		}
		if ( !f && ( x != x1 ) ){
			ui->stVA->setVisible( true );
		}
		else
			ui->stVA->setVisible( false );
	}
}
