
#ifndef GASDELDLG_H
#define GASDELDLG_H

#include "ui_gasdeldlg.h"


class GasDelDlg : public QDialog, private Ui::GasDel
{
	Q_OBJECT
public:
	GasDelDlg( QWidget *parent = 0 );

	void setDEL( double del );
	double DEL() const;

	void setMaxDEL( double max );
	double maxDEL() const;

	void setCO( double co );
	double CO() const;

	void setVA( double va );
	double VA() const;

	void setFGF( double fgf );
	double FGF() const;
};

#endif
