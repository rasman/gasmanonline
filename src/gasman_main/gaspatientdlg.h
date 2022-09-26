
#ifndef GASPATIENTDLG_H
#define GASPATIENTDLG_H

#include "gasoptionsdlg.h"


class GasPatientDlg : public QDialog
{
	Q_OBJECT	
public:
	GasPatientDlg( QWidget *parent = 0 );

	inline GasPatientWidget *patientParameters() const { return gasPatientWidget; }

public slots:
	virtual void accept();
	int exec();				//Initialize and show the dialog as a modal dialog

private:
	GasPatientWidget *gasPatientWidget;
};

#endif
