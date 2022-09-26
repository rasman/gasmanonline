
#ifndef GASSPLASHSCREEN_H
#define GASSPLASHSCREEN_H

#include <QSplashScreen>


class GasMainWindow;

class GasSplashScreen : public QSplashScreen
{
	Q_OBJECT
public:
	GasSplashScreen( int time, GasMainWindow *mw, const QPixmap &pixmap);

private slots:
	void finish();					//Close splash screen and show main window
  void fire();
	void updateSplashTime();

protected:
	virtual void showEvent( QShowEvent *event );
	virtual void hideEvent( QHideEvent *event );

private:
	int splashTime;					//Time (msec) splash screen is displayed
	GasMainWindow *mainWin;			//Pointer to main window
	bool beenFired;
	QTimer* countdownTimer;

	void showMsg(const QString& msg);
	void showLicenseMsg();
	QString timerText();
};

#endif
