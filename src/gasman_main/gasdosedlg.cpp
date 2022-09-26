
#include "gasdosedlg.h"

/////////////////////////////////////////////////////////////////////////////
// GasDoseDlg dialog

GasDoseDlg::GasDoseDlg( QWidget *parent )
	: QDialog( parent )
{
	setupUi( this );
	//Remove a context help button from the dialogs
	setWindowFlags( windowFlags() ^ Qt::WindowContextHelpButtonHint );
	dsbUnitDose->setRange( 1.e-002f, 20.0f );
	//Create connections
	connect( buttonBox, SIGNAL(accepted()), this, SLOT(accept()) );
	connect( buttonBox, SIGNAL(rejected()), this, SLOT(reject()) );
}

void GasDoseDlg::setDose( double value )
{
	dsbUnitDose->setValue( value );
}

double GasDoseDlg::dose() const
{
	return dsbUnitDose->value();
}
