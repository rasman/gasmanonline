
#ifndef GASCOSTDLG_H
#define GASCOSTDLG_H

#include "gasglobal.h"
#include "ui_gascostdlg.h"


class GasCostDlg : public QDialog, private Ui::GasCost
{
	Q_OBJECT
public:
	GasCostDlg( QWidget *parent = 0 );

	void setBottleCost( double bCost );
	double bottleCost() const;

	void setBottleSize( int bSize );
	int bottleSize() const;
};

#endif
