
#include "gascostdlg.h"

#include <float.h>

/////////////////////////////////////////////////////////////////////////////
// GasCostDlg dialog

GasCostDlg::GasCostDlg( QWidget *parent )
	: QDialog( parent )
{
	setupUi( this );
	//Remove a context help button from the dialogs
	setWindowFlags( windowFlags() ^ Qt::WindowContextHelpButtonHint );
	sbSize->setRange( 0, 2000 );
	dsbCost->setRange( 0, MAX_BOTTLE_COST );
	//Create connections
	connect( buttonBox, SIGNAL(accepted()), this, SLOT(accept()) );
	connect( buttonBox, SIGNAL(rejected()), this, SLOT(reject()) );
}

void GasCostDlg::setBottleCost( double bCost )
{
	dsbCost->setValue( bCost );
}

double GasCostDlg::bottleCost() const
{
	return dsbCost->value();
}

void GasCostDlg::setBottleSize( int bSize )
{
	sbSize->setValue( bSize );
}

int GasCostDlg::bottleSize() const
{
	return sbSize->value();
}
