
#ifndef GASAGENTDLG_H
#define GASAGENTDLG_H

#include "ui_gasagentdlg.h"
#include "gasanes.h"

class GasAgentDlg : public QDialog, private Ui::GasAgent
{
	Q_OBJECT
public:
	GasAgentDlg( QWidget *parent = 0 );

	void setAgent( const QString &agnt );
	QString agent() const;

	void setAgentChangeable( const bool changeable );

	void setCircuit( const QString &circ );
	QString circuit() const;

	inline void setArray( GasAnesArray *a ) { m_pAnesArray = a; }
	inline GasAnesArray *array() const { return m_pAnesArray; }

public slots:
	int exec();		//Initialize and show the dialog as a modal dialog

// Attributes
private:
	GasAnesArray *m_pAnesArray;
	QString m_strAgent;
	bool m_agentChangeable;
};

#endif
