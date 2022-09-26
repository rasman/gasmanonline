
#include "gasdeldlg.h"

#include "gasglobal.h"

/////////////////////////////////////////////////////////////////////////////
// GasDelDlg dialog

GasDelDlg::GasDelDlg( QWidget *parent )
	: QDialog( parent )
{
	setupUi( this );
	//Remove a context help button from the dialogs
	setWindowFlags( windowFlags() ^ Qt::WindowContextHelpButtonHint );

	dsbDEL->setMaximum( 0.0 );
	dsbVA->setRange( 0.0, MAX_VA );
	dsbFGF->setRange( 0.0, MAX_FGF );
	dsbCO->setRange( 0.0, MAX_CO );
	
	//Create connections
	connect( buttonBox, SIGNAL(accepted()), this, SLOT(accept()) );
	connect( buttonBox, SIGNAL(rejected()), this, SLOT(reject()) );
}

void GasDelDlg::setDEL( double del )
{
	dsbDEL->setValue( del );
}

double GasDelDlg::DEL() const
{
	return dsbDEL->value();
}

void GasDelDlg::setMaxDEL( double max )
{
	dsbDEL->setMaximum( max );
}

double GasDelDlg::maxDEL() const
{
	return dsbDEL->maximum();
}

void GasDelDlg::setCO( double co )
{
	dsbCO->setValue( co );
}

double GasDelDlg::CO() const
{
	return dsbCO->value();
}

void GasDelDlg::setVA( double va )
{
	dsbVA->setValue( va );
}

double GasDelDlg::VA() const
{
	return dsbVA->value();
}

void GasDelDlg::setFGF( double fgf )
{
	dsbFGF->setValue( fgf );
}

double GasDelDlg::FGF() const
{
	return dsbFGF->value();
}
