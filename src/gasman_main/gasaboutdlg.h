
#ifndef GASABOUTDLG_H
#define GASABOUTDLG_H

#include "ui_gasaboutdlg.h"


class GasAboutDlg : public QDialog, private Ui::GasAbout
{
public:
	GasAboutDlg( QWidget *parent = 0 );
};

#endif
