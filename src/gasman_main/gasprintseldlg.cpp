
#include "gasprintseldlg.h"

/////////////////////////////////////////////////////////////////////////////
// GasPrintSelDlg dialog


GasPrintSelDlg::GasPrintSelDlg( QWidget *parent )
	: QDialog( parent )
{
	setupUi( this );

	connect( buttonBox, SIGNAL(accepted()), this, SLOT(accept()) );
	connect( buttonBox, SIGNAL(rejected()), this, SLOT(reject()) );
}

GasPrintSelDlg::~GasPrintSelDlg()
{
}

void GasPrintSelDlg::setGraphs( bool set )
{
	cbGraphs->setChecked( set );
}

bool GasPrintSelDlg::isGraphs() const
{
	return cbGraphs->isChecked();
}

void GasPrintSelDlg::setPrintout( bool set )
{
	cbPrintout->setChecked( set );
}

bool GasPrintSelDlg::isPrintout() const
{
	return cbPrintout->isChecked();
}
