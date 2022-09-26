
#include <QtCore>
#include <QtGlobal>
#include <QObject>
#if QT_VERSION >=0x050000
#include <QtWidgets>
#include <QMessageBox>
#else
#include <QtGui>
#endif
#include "gasapplication.h"

#ifdef Q_OS_UNIX
#include "gasmacstyle.h"
#endif


int main( int argc, char **argv )
{
	QString version = QT_VERSION_STR;

	//Construct a GasMan application object
#if QT_VERSION >= 0x050700
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
#endif
	GasApplication app( argc, argv );

	// Stop the application if the file gasman.ini does not exist
    if ( !QFile::exists( app.getIniFile() ) ) {
        QMessageBox::critical( 0, GasApplication::tr( "Gas Man\xC2\xAE "), GasApplication::tr( "gasman.ini not found! The application will be closed." ) );
		return 0;
	}

	app.addLibraryPath(qApp->applicationDirPath());
#ifdef Q_OS_MACX
	GasApplication::setStyle(new GasMacStyle);
#endif

	QString locale = app.ReadProfile( "Application", "Locale", QLocale::system().name() ).toString();
	QLocale iniLocale(locale);
	if (iniLocale.name() == locale)
		QLocale::setDefault(iniLocale);
	else
	{
		locale = "en_US";
		QLocale defaultLocale(locale);
		QLocale::setDefault(defaultLocale);
	}

#ifdef Q_OS_MACX // Mac specific code	
	QString dirPath = "/../Resources/language";
#else // Win32/X11 anti-Mac specific code
	QString dirPath = "/language";
#endif

	QTranslator translator;
	////Load a translation file gasman_<locale>.qm
	translator.load( "gasman_" + locale, qApp->applicationDirPath() + dirPath );
	////Install a translation file gasman_<locale>.qm
	app.installTranslator( &translator );

	QTranslator qtTranslator; 
	////Load a translation file qt_<locale>.qm
	qtTranslator.load( "qt_" + locale, qApp->applicationDirPath() + dirPath ); 
	////Install a translation file qt_<locale>.qm
	app.installTranslator( &qtTranslator );

	//GasApplication initialization
	if ( !app.initialize() )
		return 0;
	
	//Enter the main event loop
	return app.exec();
}

#ifndef Q_OS_UNIX
int __stdcall WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, char*, int /*nShowCmd*/)
{
    int argc = 0;
	LPWSTR *szArglist = CommandLineToArgvW(GetCommandLineW(), &argc);
	if (argc > 0)
	{
		char **argList = new char*[argc];
		QList<QByteArray> stringList;
		for (int i = 0; i < argc; i++)
		{
			QByteArray a = QString::fromWCharArray(szArglist[i]).toLatin1();
			argList[i] = a.data();
			stringList << a;
		}
		return main(argc, argList);
		delete[] argList;
	}
    return main( argc, 0 );
 }
#endif
