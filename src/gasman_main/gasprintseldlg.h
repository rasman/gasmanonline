
#ifndef GASPRINTSELDLG_H
#define GASPRINTSELDLG_H

#include "ui_gasprintselectdlg.h"


class GasPrintSelDlg : public QDialog, private Ui::GasPrintSelect
{
	Q_OBJECT
public:
	GasPrintSelDlg( QWidget *parent = 0 );
	~GasPrintSelDlg();

	void setGraphs( bool set );
	bool isGraphs() const;

	void setPrintout( bool set );
	bool isPrintout() const;
};

#endif
