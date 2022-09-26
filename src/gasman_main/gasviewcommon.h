#ifndef GASVIEWCOMMON_H
#define GASVIEWCOMMON_H


#include <QWidget>

class GasChildWindow;

class GasViewCommon : public QWidget
{
	Q_OBJECT

public:
	GasViewCommon( GasChildWindow * childWindow, QWidget *parent = 0 );
	inline GasChildWindow* getChildWindow(){ return theChild; }
	virtual bool tryGetGas(int& nGas) = 0;

protected:
	GasChildWindow* theChild;
};

#endif
