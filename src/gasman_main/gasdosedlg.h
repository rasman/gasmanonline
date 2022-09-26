
#ifndef GASDOSEDLG_H
#define GASDOSEDLG_H

#include "ui_gasdosedlg.h"


class GasDoseDlg : public QDialog, private Ui::GasDose
{
	Q_OBJECT
public:
	GasDoseDlg( QWidget *parent = 0 );

	void setDose( double value );
	double dose() const;
};

#endif
