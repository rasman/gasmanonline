
#include "gassplashscreen.h"
#include "gasapplication.h"
#include "gaslicensemanager.h"

#if QT_VERSION >=0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif
#include "gasmainwindow.h"

//This file was added by the Splash Screen component

//GasSplashScreen constructor
GasSplashScreen::GasSplashScreen( int time, GasMainWindow *mw, const QPixmap &pixmap)
	: QSplashScreen( mw, pixmap, Qt::WindowStaysOnTopHint ), splashTime( time ), mainWin( mw ), 
		beenFired(false), countdownTimer(new QTimer(this))
{
	setWindowModality(Qt::WindowModal);
	setAttribute( Qt::WA_DeleteOnClose );
	splashTime = splashTime/1000;
  if(!glm->instance()->validLicenseExists())
	{
		connect(countdownTimer, SIGNAL(timeout()), this, SLOT(updateSplashTime()));
		showMsg(timerText());
		countdownTimer->start(1000);
	}
 
 
}

void GasSplashScreen::showMsg(const QString& msg)
{
	showMessage(msg, Qt::AlignLeft, Qt::white);
 	 
}
void GasSplashScreen::showLicenseMsg()
{
	showMessage(tr("This Software Licensed To:\n%1\n%2").arg(glm->instance()->licOwner).arg(glm->instance()->licCompany),Qt::AlignHCenter|Qt::AlignBottom, Qt::white);
}

QString GasSplashScreen::timerText()
{
	return QString::number(splashTime) + tr(" seconds remaining...");
}

void GasSplashScreen::showEvent( QShowEvent *event )
{
	if(!glm->instance()->validLicenseExists())
		QTimer::singleShot( splashTime*1000, this, SLOT( fire() ) );		//Start splash timer
	else
	{
		fire();
		QTimer::singleShot( splashTime*1000, this, SLOT( finish() ) );		//Start splash timer	
	}
	QSplashScreen::showEvent( event );
}

void GasSplashScreen::updateSplashTime()
{
	splashTime--;
	 
	if(splashTime > 0)
	{
		showMsg(timerText());
	//	showLicenseMsg();
	}else{
		countdownTimer->stop();
		showMsg(tr("Click to continue"));
	//	showLicenseMsg();
	}
}

void GasSplashScreen::hideEvent( QHideEvent *event )
{
	if(beenFired)
		QSplashScreen::hideEvent(event);
	else
		show();
}

//Close splash screen and show main window
void GasSplashScreen::finish()
{
	if(beenFired)
		QSplashScreen::finish( mainWin );
	else
		show();
}

void GasSplashScreen::fire()
{
	beenFired = true;
}
