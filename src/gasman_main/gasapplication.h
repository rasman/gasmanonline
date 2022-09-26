
#ifndef GASAPPLICATION_H
#define GASAPPLICATION_H

#include <QtCore>
#include <QApplication>
#include <QVariant>
#include <QStringList>
#include <QDateTime>
#include <QSplashScreen>
#include <QStack>
#include <QDropEvent>
#include "gasglobal.h"

class GasMainWindow;
class GasDoc;

class GasSettings : public QSettings
{
	Q_OBJECT

public:
	GasSettings( const QString &fileName ) : QSettings(fileName, QSettings::IniFormat)
	{
		QSettings::setIniCodec("UTF-8");
	}
};

class GasApplication : public QApplication
{
	Q_OBJECT
public:
	GasApplication( int &argc, char **argv );
	virtual ~GasApplication();

	//Read preferences from gasman.ini
	QVariant ReadProfile( const QString &group, const QString &param, const QVariant &defVal );
	//Write preferences to gasman.ini
	void WriteProfile( const QString &group, const QString &param, const QVariant &val );

	bool initialize();	//Initialization

	void addTemporaryFile( const QString &file );
	inline QString getIniFile() const { return inifile; }
	inline QString getXslFile() const { return xslfile; }
	inline QString getIconFile() const { return iconfile; }

	QStringList getColors() const;	
	QString getNextAvailableColor(const GasDoc * const thisDoc) const;
    QString checkFile(const QString& file, const QString& alias = QString());

	QStringList getAgents() const;
private slots:
	void registerProduct();
private:
	GasMainWindow *gasmainwindow;		//Pointer to main window
	QStringList temporaryFiles;			//List of temporary files
	QStringList filesToOpen; // if the application opens with a file specified.
	QSplashScreen *splash;

	QStack<QWidget*> tabStack;

	QString inifile;					//Path to gasman.ini
	QString xslfile;					//Path to gas.xsl
	QString iconfile;					//Path to gasman_lg.png
#ifdef Q_OS_MAC
	bool  event(QEvent * event);
#endif

public:
	QDateTime expireDate;
	static const QString DragMime;

	inline int pushGasTab(QWidget* const &tabWidget) {tabStack.push(tabWidget); return tabStack.size(); }
	inline QWidget* popGasTab() { return tabStack.pop(); }
	inline bool isTopGasTab(QByteArray ba) { return QLatin1String(ba) == QLatin1String(QByteArray::number(tabStack.size())); }
	bool IsGoodDrop(QDropEvent * e) { return e->mimeData()->hasFormat(DragMime) && isTopGasTab(e->mimeData()->data(DragMime)); }
};

extern GasApplication *gasApp;

#endif

