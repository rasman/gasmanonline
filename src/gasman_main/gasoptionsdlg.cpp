#include "gasoptionsdlg.h"

#if QT_VERSION >=0x050000
#include <QMessageBox>
#else
#include <QtGui>
#endif
#include <math.h>
#include "gasapplication.h"
#include "gaslicensemanager.h"
#include "gasagentdelegate.h"


/////////////////////////////////////////////////////////////////////////////
// GasPatientWidget widget

GasPatientWidget::GasPatientWidget( QWidget *parent )
: QWidget( parent )
{
	setupUi( this );

	m_bInitial = true;
	m_fScale = 1.0;
	m_fVolumeFactor = 1.0;
	m_fFlowFactor = 1.0;
	m_fMax = MAX_WEIGHT;
	m_fMin = MIN_WEIGHT;

	dsbWeight->setRange( m_fMin, m_fMax );
	dsbWeight->setValue( STD_WEIGHT );

	sbRatioFAT->setRange( 0, 100 );
	sbRatioVRG->setRange( 0, 100 );
	sbRatioMUS->setRange( 0, 100 );

	dsbMetabolismVA->setDecimals( 3 );
	dsbMetabolismCO->setDecimals( 3 );

	rbKilograms->setChecked( true );
	cur_id = 0;

	bgWeight = new QButtonGroup( this );
	bgWeight->addButton( rbKilograms, 0 );
	bgWeight->addButton( rbPounds, 1 );
	
	//create connections
	connect( bgWeight, SIGNAL( buttonClicked( int ) ), this, SLOT( weightUnitChanged( int ) ) );
	connect( dsbWeight, SIGNAL( editingFinished() ), this, SLOT( weightChanged() ) );
	connect( dsbMetabolismVA, SIGNAL( editingFinished() ), this, SLOT( metabolismVaChanged() ) );
	connect( dsbMetabolismCO, SIGNAL( editingFinished() ), this, SLOT( metabolismCoChanged() ) );
	connect( dsbALV, SIGNAL( editingFinished() ), this, SLOT( alvChanged() ) );
	connect( dsbVRG, SIGNAL( editingFinished() ), this, SLOT( vrgChanged() ) );
	connect( dsbMUS, SIGNAL( editingFinished() ), this, SLOT( musChanged() ) );
	connect( dsbFAT, SIGNAL( editingFinished() ), this, SLOT( fatChanged() ) );
	connect( dsbVEN, SIGNAL( editingFinished() ), this, SLOT( venChanged() ) );
}

void GasPatientWidget::init()
{
	//Initialization

	if ( !m_bInitial )
	{
		dsbWeight->setEnabled( false );
		dsbALV->setEnabled( false );
		dsbVRG->setEnabled( false );
		dsbMUS->setEnabled( false );
		dsbFAT->setEnabled( false );
		dsbVEN->setEnabled( false );
		sbRatioVRG->setEnabled( false );
		sbRatioMUS->setEnabled( false );
		sbRatioFAT->setEnabled( false );
		dsbMetabolismVA->setEnabled( false );
		dsbMetabolismCO->setEnabled( false );
	}

	//The outside world deals with the volumes normalized to weight, and
	//the flows un-normalized. We deal with everything normalized.
	if( weight() != STD_WEIGHT )
	{
		double fCO = CO();	// save the flows
		double fVA = VA();
		weightChanged();	// unnormalize everything (same as change wt from std)
		setCO(fCO);
		setVA(fVA);			// put back the flows
	}
}

void GasPatientWidget::setWeight( double w )
{
	if( cur_id != 0 )
		w = w * 2.20462;
	dsbWeight->setValue( w );
}

// return weight IN KILOGRAMS!!!
double GasPatientWidget::weight()  const
{
	double w = dsbWeight->value();
	if( cur_id != 0 )
		w = w / 2.20462;
	return w;
}

void GasPatientWidget::setVolumeALV( double alv )
{
	m_ALV = alv;
	dsbALV->setValue( alv );
}

double GasPatientWidget::volumeALV()  const
{
	return m_ALV / m_fVolumeFactor;
}

void GasPatientWidget::setVolumeVRG( double vrg )
{
	m_VRG = vrg;
	dsbVRG->setValue( vrg );
}

double GasPatientWidget::volumeVRG()  const
{
	return m_VRG / m_fVolumeFactor;
}

void GasPatientWidget::setVolumeMUS( double mus )
{
	m_MUS = mus;
	dsbMUS->setValue( mus );
}

double GasPatientWidget::volumeMUS()  const
{
	return m_MUS / m_fVolumeFactor;
}

void GasPatientWidget::setVolumeFAT( double fat )
{
	m_FAT = fat;
	dsbFAT->setValue( fat );
}

double GasPatientWidget::volumeFAT()  const
{
	return m_FAT / m_fVolumeFactor;
}

void GasPatientWidget::setVolumeVEN( double ven )
{
	m_VEN = ven;
	dsbVEN->setValue( ven );
}

double GasPatientWidget::volumeVEN()  const
{
	return m_VEN / m_fVolumeFactor;
}

void GasPatientWidget::setRatioVRG( int vrg )
{
	sbRatioVRG->setValue( vrg );
}

int GasPatientWidget::ratioVRG() const
{
	return sbRatioVRG->value();
}

void GasPatientWidget::setRatioMUS( int mus )
{
	sbRatioMUS->setValue( mus );
}

int GasPatientWidget::ratioMUS() const
{
	return sbRatioMUS->value();
}

void GasPatientWidget::setRatioFAT( int fat )
{
	sbRatioFAT->setValue( fat );
}

int GasPatientWidget::ratioFAT() const
{
	return sbRatioFAT->value();
}

void GasPatientWidget::setCO( double co )
{
	m_CO = co;
	dsbMetabolismCO->setValue( co );
}

double GasPatientWidget::CO() const
{
	return m_CO;
}

double GasPatientWidget::NormalCO() const
{
	return m_CO / m_fFlowFactor;
}

void GasPatientWidget::setVA( double va )
{
	m_VA = va;
	dsbMetabolismVA->setValue( va );
}

double GasPatientWidget::VA() const
{
	return m_VA;
}

double GasPatientWidget::NormalVA() const
{
	return m_VA / m_fFlowFactor;
}

/*
Compute
Scale (an acceptable order of magnitude for ALV, VRG, MUS, FAT, VEN volumes)
Ratio (ratio of patient mass to standard male) Volumes are normalized to this number.
Factor (Ratio to the 3/4 power) Flows are normalized to this number.
Bottom line: 
You can set volumes at some number whose order of magnitude isn't rediculous
based on weight, and

Volumes and flows are stored normalized to a standard (e.g. 70kg) male.
*/
void GasPatientWidget::ComputeNormalizationFactors()
{	
	
	double R = weight() / STD_WEIGHT;

	switch ( (int)(floor( log10( R ) + 0.3 ) ) )
	{
	case -4: m_fScale = 0.0001; break;
	case -3: m_fScale = 0.001; break;
	case -2: m_fScale = 0.01; break;
	case -1: m_fScale = 0.1; break;
	default: Q_ASSERT( false );
	case 0: m_fScale = 1.0; break;
	case 1: m_fScale = 10.0; break;
	case 2: m_fScale = 100.0; break;
	case 3: m_fScale = 1000.0; break;
	}
	dsbALV->setRange( 0.1 * m_fScale, 100 * m_fScale );
	dsbVRG->setRange( 0.1 * m_fScale, 100 * m_fScale );
	dsbMUS->setRange( 0.1 * m_fScale, 1000 * m_fScale );
	dsbFAT->setRange( 0.1 * m_fScale, 100 * m_fScale );
	dsbVEN->setRange( 0.1 * m_fScale, 10 * m_fScale );

	m_fVolumeFactor = R;
	m_fFlowFactor = sqrt( sqrt( R * R * R ) );
}

void GasPatientWidget::weightUnitChanged( int id )
{
	QRadioButton *cur_btn = qobject_cast<QRadioButton *>( bgWeight->button( id ) );
	if ( !cur_btn || cur_id == id ) return;

	if ( cur_btn == rbKilograms ) {
		m_fMax = MAX_WEIGHT;
		m_fMin = MIN_WEIGHT;
	}
	else {
		m_fMax = MAX_WEIGHT * 2.20462;
		m_fMin = MIN_WEIGHT * 2.20462;
	}

	double fWeight = weight();
	cur_id = id;
	dsbWeight->setRange( m_fMin, m_fMax );
	setWeight(fWeight);
}

void GasPatientWidget::weightChanged()
{
	// Get the normalized values
	double fALV = volumeALV();
	double fVRG = volumeVRG();
	double fMUS = volumeMUS();
	double fFAT = volumeFAT();
	double fVEN = volumeVEN();

	double fCO = NormalCO();
	double fVA = NormalVA();

	ComputeNormalizationFactors();

	// Display un-normalized values
	setVolumeALV( fALV * m_fVolumeFactor );
	setVolumeVRG( fVRG * m_fVolumeFactor );
	setVolumeMUS( fMUS * m_fVolumeFactor );
	setVolumeFAT( fFAT * m_fVolumeFactor );
	setVolumeVEN( fVEN * m_fVolumeFactor );

	setCO( fCO * m_fFlowFactor );
	setVA( fVA * m_fFlowFactor );
}

bool GasPatientWidget::checkRatio()
{
	if ( ratioFAT() + ratioVRG() + ratioMUS() != 100 ) {
		QMessageBox::critical( this, tr( "Gas Man\xC2\xAE "), tr( "<p>The sum of the flow percentages must be 100." ), QMessageBox::Ok );
		return false;
	}
	return true;
}

void GasPatientWidget::setInternalValue(double& iv, const QDoubleSpinBox* const pSB) const
{
	const double rv = roundingValue(pSB);
	if (round(pSB->value(), rv) != round(iv, rv))
		iv = pSB->value();
}

void GasPatientWidget::metabolismVaChanged() { setInternalValue(m_VA, dsbMetabolismVA); }
void GasPatientWidget::metabolismCoChanged() { setInternalValue(m_CO, dsbMetabolismCO); }
void GasPatientWidget::alvChanged() { setInternalValue(m_ALV, dsbALV); }
void GasPatientWidget::vrgChanged() { setInternalValue(m_VRG, dsbVRG); }
void GasPatientWidget::musChanged() { setInternalValue(m_MUS, dsbMUS); }
void GasPatientWidget::fatChanged() { setInternalValue(m_FAT, dsbFAT); }
void GasPatientWidget::venChanged() { setInternalValue(m_VEN, dsbVEN); }

/////////////////////////////////////////////////////////////////////////////
// GasChoiceSound dialog

GasChoiceSound::GasChoiceSound( QWidget *parent )
	: QDialog( parent ) 
{
	setupUi( this );
	//Remove a context help button from dialogs
	setWindowFlags( windowFlags() ^ Qt::WindowContextHelpButtonHint );
	
	//create connections
	connect( tbBrowse_1, SIGNAL( clicked() ), this, SLOT( browse() ) );
	connect( tbBrowse_2, SIGNAL( clicked() ), this, SLOT( browse() ) );
	connect( tbBrowse_3, SIGNAL( clicked() ), this, SLOT( browse() ) );
	connect( tbBrowse_4, SIGNAL( clicked() ), this, SLOT( browse() ) );

	connect( leSystem_1, SIGNAL( editingFinished() ), this, SLOT( edited() ) );
	connect( leSystem_2, SIGNAL( editingFinished() ), this, SLOT( edited() ) );
	connect( leSystem_3, SIGNAL( editingFinished() ), this, SLOT( edited() ) );
	connect( leSystem_4, SIGNAL( editingFinished() ), this, SLOT( edited() ) );

	connect( buttonBox, SIGNAL(accepted()), this, SLOT(accept()) );
	connect( buttonBox, SIGNAL(rejected()), this, SLOT(reject()) );
}

void GasChoiceSound::browse()
{
	//browse a *.wav (under Windows) or *.wav, *.aif (under MacOS) file
	QString filter;
#ifdef Q_OS_MACX
	filter = tr("Sound Files (*.wav *.aif)");
#else
	filter = tr("Sound Files (*.wav)");
#endif
	QString fileName = QFileDialog::getOpenFileName( this, tr( "Open file" ), QString(),  filter);
	if ( fileName.isEmpty() || !QFile::exists( fileName ) )
		return;

	QLineEdit *le = 0;
	if ( sender() == tbBrowse_1 )
		le = leSystem_1;
	else if ( sender() == tbBrowse_2 )
		le = leSystem_2;
	else if ( sender() == tbBrowse_3 )
		le = leSystem_3;
	else if ( sender() == tbBrowse_4 )
		le = leSystem_4;
	
	if ( !le ) return;

	le->setText( fileName );
}

void GasChoiceSound::edited()
{
    QLineEdit *le = qobject_cast<QLineEdit*>(sender());
    if (!le) return;

    if (le->text().isEmpty())
  		le->setText( tr("<no sound>") );
}

void GasChoiceSound::accept()
{
	//write the sound scheme to gasman.ini
	gasApp->WriteProfile( "Signals", "System_1", leSystem_1->text().isEmpty() ? tr( "<no sound>" ) : leSystem_1->text() );
	gasApp->WriteProfile( "Signals", "System_2", leSystem_2->text().isEmpty() ? tr( "<no sound>" ) : leSystem_2->text() );
	gasApp->WriteProfile( "Signals", "System_3", leSystem_3->text().isEmpty() ? tr( "<no sound>" ) : leSystem_3->text() );
	gasApp->WriteProfile( "Signals", "System_4", leSystem_4->text().isEmpty() ? tr( "<no sound>" ) : leSystem_4->text() );

	QDialog::accept();
}

int GasChoiceSound::exec()
{
	//read from gasman.ini the current sound scheme
	leSystem_1->setText( gasApp->ReadProfile( "Signals", "System_1", tr( "<no sound>" ) ).toString() );
	leSystem_2->setText( gasApp->ReadProfile( "Signals", "System_2", tr( "<no sound>" ) ).toString() );
	leSystem_3->setText( gasApp->ReadProfile( "Signals", "System_3", tr( "<no sound>" ) ).toString() );
	leSystem_4->setText( gasApp->ReadProfile( "Signals", "System_4", tr( "<no sound>" ) ).toString() );
	
	return QDialog::exec();		//Shows the dialog as a modal dialog
}

/////////////////////////////////////////////////////////////////////////////
// GasPDefaultsWidget widget

GasPDefaultsWidget::GasPDefaultsWidget( QWidget *parent )
	: QWidget( parent )
{
	setupUi( this );

	bgSignal = new QButtonGroup( this );
	bgSignal->addButton( rbAudibleOff, 0 );
	bgSignal->addButton( rbAudibleBeep, 1 );
	bgSignal->addButton( rbAudibleSystem1, 2 );
	bgSignal->addButton( rbAudibleSystem2, 3 );
	bgSignal->addButton( rbAudibleSystem3, 4 );
	bgSignal->addButton( rbAudibleSystem4, 5 );

	bgLineLabel = new QButtonGroup( this );
	bgLineLabel->addButton( rbGraphLabelsOff, 0 );
	bgLineLabel->addButton( rbGraphLabelsOn, 1 );
	bgLineLabel->addButton( rbGraphLabelsAuto, 2 );

	bgLineThickness = new QButtonGroup( this );
	bgLineThickness->addButton( rbGraphThicknessOne, 0 );
	bgLineThickness->addButton( rbGraphThicknessTwo, 1 );
	bgLineThickness->addButton( rbGraphThicknessThree, 2 );
	
	//create connections
	connect( hsGraphPercent, SIGNAL( valueChanged( int ) ), this, SLOT( graphPercentChanged( int ) ) );
	connect( pbCustom, SIGNAL( clicked() ), this, SLOT( custom() ) );

	setUseGraphpaper( false );
	setJumpPercent( 25 );
	hsGraphPercent->setRange( 25, 100 );
}

void GasPDefaultsWidget::custom()
{
	//set up the sound scheme
	GasChoiceSound dlg( this );
	dlg.exec();
}

void GasPDefaultsWidget::setSignal( int sig )
{
	QRadioButton *btn = qobject_cast<QRadioButton *>( bgSignal->button( sig ) );
	if ( !btn ) return;
	btn->setChecked( true );
}

int GasPDefaultsWidget::signal() const
{
	return bgSignal->checkedId();
}

void GasPDefaultsWidget::setLineLabel( int label )
{
	QRadioButton *btn = qobject_cast<QRadioButton *>( bgLineLabel->button( label ) );
	if ( !btn ) return;
	btn->setChecked( true );
}

int GasPDefaultsWidget::lineLabel() const
{
	return bgLineLabel->checkedId();
}

void GasPDefaultsWidget::setLineThickness( int thickness )
{
	QRadioButton *btn = qobject_cast<QRadioButton *>( bgLineThickness->button( thickness ) );
	if ( !btn ) return;
	btn->setChecked( true );
}

int GasPDefaultsWidget::lineThickness() const
{
	return bgLineThickness->checkedId();
}

void GasPDefaultsWidget::setJumpPercent( int percent )
{
	hsGraphPercent->setValue( percent );
}

int GasPDefaultsWidget::jumpPercent() const
{
	return hsGraphPercent->value();
}

void GasPDefaultsWidget::setUseGraphpaper( bool use )
{
	cbUseGraphPaper->setChecked( use );
}

bool GasPDefaultsWidget::isUseGraphpaper() const
{
	return cbUseGraphPaper->isChecked();
}

void GasPDefaultsWidget::graphPercentChanged( int value )
{
	lPercentValue->setText( QString::number( value ) );
}

/////////////////////////////////////////////////////////////////////////////
// GasVDefaultsWidget widget

GasVDefaultsWidget::GasVDefaultsWidget( QWidget *parent )
	: QWidget( parent )
{
	setupUi( this );

	sbFGF->setRange( 0, int( MAX_FGF ) );
	sbVA->setRange( 0, int( MAX_VA ) );
	sbCO->setRange( 0, int( MAX_CO ) );
	
	cbTime->addItems( listTime );

	cbShowControl->hide();
}

void GasVDefaultsWidget::setEnabledScroll( bool set )
{
	cbEnableScrolling->setChecked( set );
}

bool GasVDefaultsWidget::isEnabledScroll() const
{
	return cbEnableScrolling->isChecked();
}

void GasVDefaultsWidget::setVisibleCost( bool show )
{
	cbShowCost->setChecked( show );
}

bool GasVDefaultsWidget::isVisibleCost() const
{
	return cbShowCost->isChecked();
}

void GasVDefaultsWidget::setVisibleMl( bool show )
{
	cbShowMl->setChecked( show );
}

bool GasVDefaultsWidget::isVisibleMl() const
{
	return cbShowMl->isChecked();
}

void GasVDefaultsWidget::setVisibleControl( bool show )
{
	cbShowControl->setChecked( show );
}

bool GasVDefaultsWidget::isVisibleControl() const
{
	return cbShowControl->isChecked();
}

void GasVDefaultsWidget::setHighCO( quint16 hCO )
{
	sbCO->setValue( hCO );
}

quint16 GasVDefaultsWidget::highCO() const
{
	return sbCO->value();
}

void GasVDefaultsWidget::setHighFGF( quint16 hFGF )
{
	sbFGF->setValue( hFGF );
}

quint16 GasVDefaultsWidget::highFGF() const
{
	return sbFGF->value();
}

void GasVDefaultsWidget::setHighVA( quint16 hVA )
{
	sbVA->setValue( hVA );
}

quint16 GasVDefaultsWidget::highVA() const
{
	return sbVA->value();
}

void GasVDefaultsWidget::setScale( const QString &scal )
{
	int index = cbTime->findText( scal, Qt::MatchFixedString );
	if( index == -1 ) return;
	cbTime->setCurrentIndex( index );
}

QString GasVDefaultsWidget::scale() const
{
	return cbTime->currentText();
}

/////////////////////////////////////////////////////////////////////////////
// GasCDefaultsWidget widget

GasCDefaultsWidget::GasCDefaultsWidget(QWidget *parent)
  : QWidget(parent)
{
	setupUi(this);
	tableWidget->setItemDelegateForColumn(0, new GasAgentDelegate(GasAgentDelegate::anestheticSizeColumnType, this));
	tableWidget->setItemDelegateForColumn(1, new GasAgentDelegate(GasAgentDelegate::anestheticCostColumnType, this));
	tableWidget->setItemDelegateForColumn(2, new GasAgentDelegate(GasAgentDelegate::anestheticColorColumnType, this));

	QHeaderView* hdr = tableWidget->horizontalHeader();
#if QT_VERSION >=0x050000
	hdr->setSectionResizeMode(0, QHeaderView::Fixed);
	hdr->resizeSection(0, 70);
	hdr->setSectionResizeMode(1, QHeaderView::Fixed);
	hdr->resizeSection(1, 80);
	hdr->setSectionResizeMode(2, QHeaderView::Fixed);
	hdr->resizeSection(2, 80);
#else
	hdr->setResizeMode(0, QHeaderView::Fixed);
	hdr->resizeSection(0, 70);
	hdr->setResizeMode(1, QHeaderView::Fixed);
	hdr->resizeSection(1, 80);
	hdr->setResizeMode(2, QHeaderView::Fixed);
	hdr->resizeSection(2, 80);
#endif
}

GasAttributeMap GasCDefaultsWidget::getSizes() const
{
	GasAttributeMap agentsAndSizes;
	for(int i = 0; i < tableWidget->rowCount(); i++)
		agentsAndSizes[tableWidget->verticalHeaderItem(i)->text()] = tableWidget->item(i, 0)->text();
	return agentsAndSizes;
}

GasAttributeMap GasCDefaultsWidget::getCosts() const
{
	GasAttributeMap agentsAndCosts;
	for(int i = 0; i < tableWidget->rowCount(); i++)
		agentsAndCosts[tableWidget->verticalHeaderItem(i)->text()] = tableWidget->item(i, 1)->text();
	return agentsAndCosts;
}

GasAttributeMap GasCDefaultsWidget::getColors() const
{
	GasAttributeMap agentsAndColors;
	for(int i = 0; i < tableWidget->rowCount(); i++)
		agentsAndColors[tableWidget->verticalHeaderItem(i)->text()] = tableWidget->item(i, 2)->text();
	return agentsAndColors;
}

void GasCDefaultsWidget::addAgentSizeCostColor(const QString& agent, const int& size, const float& cost, const QString& color, const bool editable)
{
	int rowCount = tableWidget->rowCount();
	tableWidget->setRowCount(rowCount + 1);

	QTableWidgetItem* item = new QTableWidgetItem(agent);
	tableWidget->setVerticalHeaderItem(rowCount, item);

	item = new QTableWidgetItem( QString::number( size, 'f', 0 ) );
	tableWidget->setItem(rowCount, 0, item);
	item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	if (!editable) item->setFlags(Qt::NoItemFlags);

	item = new QTableWidgetItem( QString::number( cost, 'f', 2 ) );
	tableWidget->setItem(rowCount, 1, item);
	item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	if (!editable) item->setFlags(Qt::NoItemFlags);

	item = new QTableWidgetItem(color);
	tableWidget->setItem(rowCount, 2, item);
}

/////////////////////////////////////////////////////////////////////////////
// GasSDefaultsWidget widget

GasSDefaultsWidget::GasSDefaultsWidget( QWidget *parent )
	: QWidget( parent )
{
	setupUi( this );
	dsbFGF->setRange( 0.0, MAX_FGF );
	dsbCKT->setRange( 0.0, 100.0 );

	cbSpeed->addItems( listSpeed );
	cbCircuit->addItems( listCircuit );
}

void GasSDefaultsWidget::setVolumeCKT( double ckt )
{
	dsbCKT->setValue( ckt );
}

double GasSDefaultsWidget::volumeCKT() const
{
	return dsbCKT->value();
}

void GasSDefaultsWidget::setFGF( double fgf )
{
	dsbFGF->setValue( fgf );
}

double GasSDefaultsWidget::FGF() const
{
	return dsbFGF->value();
}

void GasSDefaultsWidget::setSpeed( const QString &spd )
{
	int index = cbSpeed->findText( spd, Qt::MatchFixedString );
	if( index == -1 ) return;
	cbSpeed->setCurrentIndex( index );
}

QString GasSDefaultsWidget::speed() const
{
	return cbSpeed->currentText();
}

QString GasSDefaultsWidget::agent() const
{
	return cbAgent->currentText();
}

void GasSDefaultsWidget::setCircuit( const QString &circ )
{
	int index = cbCircuit->findText( circ, Qt::MatchFixedString );
	if( index == -1 ) return;
	cbCircuit->setCurrentIndex( index );
}

QString GasSDefaultsWidget::circuit() const
{
	return cbCircuit->currentText();
}

void GasSDefaultsWidget::init()
{
	//Initialization
	cbAgent->clear();
	QStringList agents = gasApp->getAgents();
	cbAgent->addItems( agents );

	int index = cbAgent->findText( m_agent, Qt::MatchFixedString );
	if( index == -1 ) return;
	cbAgent->setCurrentIndex( index );
}

/////////////////////////////////////////////////////////////////////////////
// GasOptionsDialog dialog

GasOptionsDialog::GasOptionsDialog( QWidget *parent )
	: QDialog( parent )
{
	//remove a context help button from dialogs
	setWindowFlags( windowFlags() ^ Qt::WindowContextHelpButtonHint );
	setWindowTitle( tr( "Default options" ) );
	
	//create a tabs
	gasPatientWidget = new GasPatientWidget;
	gasPDefaultsWidget = new GasPDefaultsWidget;
	gasVDefaultsWidget = new GasVDefaultsWidget;
	gasSDefaultsWidget = new GasSDefaultsWidget;
	gasCDefaultsWidget = new GasCDefaultsWidget;


	twOptions = new QTabWidget;
	twOptions->setUsesScrollButtons( false );
	twOptions->addTab( gasPDefaultsWidget, tr( "Program" ) );
	twOptions->addTab( gasVDefaultsWidget, tr( "View" ) );
	twOptions->addTab( gasSDefaultsWidget, tr( "Simulation" ) );
	twOptions->addTab( gasCDefaultsWidget, tr( "Agent" ) );
	twOptions->addTab( gasPatientWidget, tr( "Patient" ) );

	QDialogButtonBox *buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this );
	connect( buttonBox, SIGNAL( accepted() ), this, SLOT( accept() ) );
	connect( buttonBox, SIGNAL( rejected() ), this, SLOT( reject() ) );

	QVBoxLayout *mainLayout = new QVBoxLayout( this );
	mainLayout->addWidget( twOptions );
	mainLayout->addWidget( buttonBox );
	
	twOptions->setCurrentIndex( 0 );

	resize( minimumSizeHint() );
}

//initialize and shows the dialog as a modal dialog
int GasOptionsDialog::exec()
{	
	gasPatientWidget->init();
	gasSDefaultsWidget->init();
	return QDialog::exec();
}

void GasOptionsDialog::accept()
{
	if ( twOptions->currentWidget() == gasPatientWidget && !gasPatientWidget->checkRatio() )
		return;

	QDialog::accept();
}
