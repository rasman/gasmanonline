
#include "gaspatientdlg.h"

#if QT_VERSION >=0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

/////////////////////////////////////////////////////////////////////////////
// GasPatientDlg dialog

GasPatientDlg::GasPatientDlg( QWidget *parent )
	: QDialog( parent )
{
	//Remove a context help button from the dialogs
	setWindowFlags( windowFlags() ^ Qt::WindowContextHelpButtonHint );
	setWindowTitle( tr( "Change patient" ) );
	
	gasPatientWidget = new GasPatientWidget (this);

	QDialogButtonBox *buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this );
	//Create connections
	connect( buttonBox, SIGNAL(accepted()), this, SLOT(accept()) );
	connect( buttonBox, SIGNAL(rejected()), this, SLOT(reject()) );

	QVBoxLayout *mainLayout = new QVBoxLayout( this );
	mainLayout->addWidget( gasPatientWidget );
	mainLayout->addWidget( buttonBox );

	resize( minimumSizeHint() );
}

//Initialize and show the dialog as a modal dialog
int GasPatientDlg::exec()
{
	gasPatientWidget->init();
	return QDialog::exec();
}

void GasPatientDlg::accept()
{
	if ( !gasPatientWidget->checkRatio() )
		return;

	QDialog::accept();
}
