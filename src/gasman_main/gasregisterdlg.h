#ifndef GASREGISTERDLG_H
#define GASREGISTERDLG_H

#include "ui_gasregisterdlg.h"


class GasRegisterDlg : public QDialog, private Ui::GasRegister
{
	Q_OBJECT

public:
	GasRegisterDlg( QWidget *parent = 0 );

private slots:
	void attemptRegisterLicense();
	void registerFromFile();
	void cleanRegister();
};

#endif
