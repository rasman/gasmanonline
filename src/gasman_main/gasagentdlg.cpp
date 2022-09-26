
#include "gasagentdlg.h"

/////////////////////////////////////////////////////////////////////////////
// GasAgentDlg dialog

GasAgentDlg::GasAgentDlg( QWidget *parent )
	: QDialog( parent )
{
	setupUi( this );
	//Remove a context help button from the dialogs
	setWindowFlags( windowFlags() ^ Qt::WindowContextHelpButtonHint );

	m_strAgent = "";

	cbCircuit->addItems( listCircuit );
	
	//Create connections
	connect( buttonBox, SIGNAL(accepted()), this, SLOT(accept()) );
	connect( buttonBox, SIGNAL(rejected()), this, SLOT(reject()) );
}

void GasAgentDlg::setAgent( const QString &agnt )
{	
	m_strAgent = agnt;
	int index = cbAgent->findText( agnt, Qt::MatchFixedString );
	if( index != -1 )
		cbAgent->setCurrentIndex( index );
}

void GasAgentDlg::setAgentChangeable( const bool changeable )
{
	m_agentChangeable = changeable;
}

QString GasAgentDlg::agent() const
{
	return cbAgent->currentText();
}

void GasAgentDlg::setCircuit( const QString &circ )
{
	int index = cbCircuit->findText( circ, Qt::MatchFixedString );
	if( index == -1 ) return;
	cbCircuit->setCurrentIndex( index );
}

QString GasAgentDlg::circuit() const
{
	return cbCircuit->currentText();
}

int GasAgentDlg::exec()
{
	Q_ASSERT( m_pAnesArray != 0 );
	
	GasAnesArray &anesArray = *m_pAnesArray;
	int nAnes = anesArray.size();

	cbAgent->clear();

	for ( int i = 0; i < nAnes; ++i )
		cbAgent->addItem( anesArray[i]->m_strName );
	
	setAgent( m_strAgent );
	cbAgent->setEnabled( m_agentChangeable );

	return QDialog::exec();
}
