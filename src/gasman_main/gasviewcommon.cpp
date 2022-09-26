#include "gasviewcommon.h"
#include "gaschildwindow.h"

GasViewCommon::GasViewCommon( GasChildWindow * childWindow, QWidget *parent)
	: QWidget(parent), theChild(childWindow){}
