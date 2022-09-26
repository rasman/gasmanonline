#include "gasapplication.h"
#if QT_VERSION >=0x050000
#include <QtWidgets>
#else
#include <QtGui>
#if defined(Q_OS_MACX)
extern void qt_mac_set_menubar_icons(bool enable);
#endif
#endif
#include "gasmainwindow.h"
#include "gaspanelview.h"
#include "gasgraphview.h"
#include "gasoverlay.h"
#include "gassplashscreen.h"
#include "gasdoc.h"
#include "gaslicensemanager.h"
#include "gasregisterdlg.h"

GasApplication *gasApp = 0;
const QString GasApplication::DragMime("application/x-gaschildwindow");

#if 0 // #if !defined(Q_OS_MACX)

int PowMod(int base, int pwr)
{
	int r=1;
	while (pwr>0) {
		if (pwr%2)
			r = (int)(((long)r*base) % 211);
		base = (int)(((long)base*base) % 211);
		pwr /=2;
	}
	return r;
}

#endif

// GasApplication constructor
GasApplication::GasApplication( int &argc, char **argv )
	: QApplication( argc, argv ), gasmainwindow( 0 ), inifile(), expireDate(QDateTime::fromString("2008.1.1", "yyyy.M.d"))
{

	gasApp = this;
		
	inifile = checkFile("gasman.ini");
 
	for(int i = 1; i< arguments().count(); i++){
		filesToOpen<<arguments().at(i);
	}
	glm->instance()->validLicenseExists();
	connect(this, SIGNAL(lastWindowClosed()), this, SLOT(quit()));
}

GasApplication::~GasApplication()
{
	// Remove all temporary files
	foreach( QString file, temporaryFiles )
		if ( QFile::exists( file ) )
			QFile::remove( file );
	temporaryFiles.clear();
	
	// Delete main window
	delete gasmainwindow;
}

// if .ini or .xsl file do not exist where they should, copy the 
// fallback version from resource, returns file location as QString
QString GasApplication::checkFile(const QString& file, const QString& alias)
{
	QString dirPath = qApp->applicationDirPath();
#ifdef Q_OS_UNIX // Mac specific code
	dirPath += "/../Resources/";
#else // Win32/X11 anti-Mac specific code
	dirPath += "/";
#endif

	QString newName = dirPath + (alias.isEmpty() ? file : alias);

	if(!QFile::exists(newName))
	{
		QString srcName;

		if(file.compare("gasman.ini") == 0)
			srcName = QString(":/safeCopy/gasman_%1.ini").arg(QLocale::system().name());
		if(file.compare("gas.xsl") == 0)
			srcName = QString(":/safeCopy/gas_%1.xsl").arg(QLocale().name());
		if(file.compare("gasman_lg.png") == 0)
			srcName = ":/images/gasman_lg.png";

		QFile source(srcName);
		if(!source.copy(newName))
			return "";

		// for whatever reason, copy only sets read permissions for user
		// we must manually set write permissions so changes to the .ini can be made
		QFile dest(newName);
		dest.setPermissions(dest.permissions() | QFile::WriteOwner);
	}
	return newName;
}


/////////////////////////////////////////////////////////////////////////////
// GasApplication initialization

bool GasApplication::initialize()
{
	QIcon icon(":/images/gasman.png");      // 32x32
	icon.addFile(":/images/gasman_lg.png"); // 48x48
	setWindowIcon( icon );
	xslfile = gasApp->checkFile("gas.xsl", QString("language/gas_%1.xsl").arg(QLocale().name()));
	iconfile = gasApp->checkFile("gasman_lg.png");

#if defined(Q_OS_UNIX)
    // Mac specific code
//    qt_mac_set_menubar_icons(false);
	setAttribute(Qt::AA_DontShowIconsInMenus);
#endif

	QStringList colors = ReadProfile( "Colors", "Colors", QStringList() ).toStringList();
	// if default colors are not in the .ini file, add them
	if(colors.isEmpty())
	{
		QStringList addColors;
		addColors << tr("red") << tr("blue") << tr("green") << tr("orange") << tr("purple") << tr("black") << tr("white") << tr("yellow") << tr("cyan") << tr("darkBlue");
		WriteProfile("Colors","Colors",addColors);
	}

	// Translate global strings
	InitializeGlobalLists();

	// Get settings, etc.
	GasDoc::ReadProfile();
	GasPanelView::ReadProfile();
	GasGraphView::ReadProfile();
	GasOverlay::ReadProfile();
	
	// Construct main window
	gasmainwindow = new GasMainWindow();
	
	// Construct splash screen
	quint32 dw;
	QPixmap pixmap;
	if (!glm->instance()->validLicenseExists())
	{
		glm->instance()->clearRegister();
		pixmap = QPixmap( ":/images/splash-demo.png" );
/*		QPainter painter(&pixmap);
		painter.setPen(Qt::black);
		QFont font("Times New Roman", 12);
#ifdef Q_OS_MAC
		font.setPointSize(20);
#endif
	 
		painter.setFont(font);
		painter.drawText(0, pixmap.height()-45, pixmap.width(), 20 ,Qt::AlignCenter, tr("unregistered version"));
*/		 

		//dw = 1000 * qMax(ReadProfile( "Defaults", "SplashTime", 5 ).toUInt(), (uint)15);	
	
	}else{
		pixmap = QPixmap( ":/images/splash.png" );
		 
		// branding
		QPainter painter(&pixmap);
		painter.setPen(Qt::black);
		QFont font("Times New Roman", 12);
#ifdef Q_OS_MAC
		font.setPointSize(20);
#endif
		 
		painter.setFont(font);
		painter.drawText(0, pixmap.height()-90, pixmap.width(), 20 ,Qt::AlignCenter, glm->instance()->licOwner);
		painter.drawText(0, pixmap.height()-70, pixmap.width(), 20, Qt::AlignCenter, glm->instance()->licCompany); 
		painter.drawText(0, pixmap.height()-50, pixmap.width(), 20, Qt::AlignCenter, glm->instance()->licInfo); 
		painter.setPen(Qt::white);
	 
		painter.drawText(1, pixmap.height()-92, pixmap.width(), 20 ,Qt::AlignCenter, glm->instance()->licOwner);
		painter.drawText(1, pixmap.height()-72, pixmap.width(), 20, Qt::AlignCenter, glm->instance()->licCompany); 
		painter.drawText(1, pixmap.height()-52, pixmap.width(), 20, Qt::AlignCenter, glm->instance()->licInfo); 

		//dw = 1000 * ReadProfile( "Defaults", "SplashTime", 5 ).toUInt() ;	
	}

	dw = 1000 * ReadProfile( "Defaults", "SplashTime", 5 ).toUInt() ;	
	splash = new GasSplashScreen(  dw , gasmainwindow, pixmap);
	 
#ifndef Q_OS_MAC	  
	if(!glm->instance()->validLicenseExists()){
		QPushButton * bt = new QPushButton(tr("Register"), splash);
		QPoint pt(pixmap.height()-45,pixmap.width()-100);
		bt->move(pixmap.width()-100,pixmap.height()-45 );
		connect(bt,SIGNAL(clicked()), this,SLOT(registerProduct()));
	}
#endif
	splash->show();
	gasmainwindow->show();

//    QFileOpenEvent* e = new QFileOpenEvent("");
//    postEvent(this, e, Qt::LowEventPriority);

#ifdef Q_OS_MAC
	processEvents();// process any QFileOpenEvents before loading any files
#endif


	if(filesToOpen.size()>0){	 
	  foreach(QString file, filesToOpen){			
	    gasmainwindow->open(file);
	  }		 
	}

	// if no sims open, create a new one
	if (!gasmainwindow->allChildren().size() && !gasmainwindow->newFile() )
	  return false;
	
	gasmainwindow->repaint();	// HMF:On XP, dragging a bad file onto the GM icon (which pops
								// up the 'corrupt' dialog and then opens a new file) will not
								// show the new sim or enable the menus without this (or bored
								// user pumps a message...)

	return true;
}
void GasApplication::registerProduct()
{
	GasRegisterDlg dlg(splash);
	 
	dlg.exec();
}

#ifdef Q_OS_MAC

bool GasApplication::event(QEvent * event)
{
        if (event->type() == QEvent::FileOpen) {
            QFileOpenEvent * e = static_cast<QFileOpenEvent *>(event);

            if ( !e->file().isEmpty() ) {
                gasmainwindow->open(e->file());
                return true;
            }
//            else if ( !e->spontaneous() ) {
//                // we posted a blank one at startup
//                if(filesToOpen.size()>0){
//                    foreach(QString file, filesToOpen){
//                        gasmainwindow->open(file);
//                    }
//                }
//                // if no sims open, create a new one
//                if (!gasmainwindow->allChildren().size())
//                        gasmainwindow->newFile();

//                return true;
//            }

        }
        return QApplication::event(event);
}
#endif
// Read preferences from gasman.ini
QVariant GasApplication::ReadProfile( const QString &group, const QString &param, const QVariant &defVal )
{
    // TODO: embed default gasman.ini as a resource, using it as a fallback
    // settings file for the standard QSettings locations. This would allow
    // per-user settings while keeping sensible defaults

    GasSettings settings( getIniFile() );
    QVariant value;
    bool isEmptyGroup = group.isNull();
    if ( !isEmptyGroup ) settings.beginGroup( group );
    value = settings.value( param, defVal );
    if ( !isEmptyGroup ) settings.endGroup();
    return value;
}


// Write preferences to *.ini
void GasApplication::WriteProfile( const QString &group, const QString &param, const QVariant &val )
{
	// make sure we have a valid ini file before writing
	inifile = checkFile("gasman.ini");

    GasSettings settings( getIniFile() );
	bool isEmptyGroup = group.isNull();
	if ( !isEmptyGroup )
		settings.beginGroup( group );
	if ( group == "RecentFileList" )			// If group is RecentFileList,
		settings.remove( "" );					// Remove any/all sub-settings of this group
	if (val.type() == QMetaType::Float || val.type() == QVariant::Double )
		settings.setValue( param, floor(val.toDouble() * 1e6 + 0.5) / 1e6 );
	else
		settings.setValue( param, val );
	if ( !isEmptyGroup )
		settings.endGroup();
}

void GasApplication::addTemporaryFile( const QString &file )
{
	// Add temporary file "file" to list of temporary files.
	// Temporary files on this list will be removed after closing the application 
	temporaryFiles.append( file );
}

// make sure colors provided are valid
QStringList GasApplication::getColors() const
{
	QStringList colors;
	QStringList fromINI = gasApp->ReadProfile( "Colors", "Colors", QStringList() ).toStringList();
	foreach(QString color, listColors)
	{
		if(fromINI.contains(color, Qt::CaseInsensitive)){
			colors << color;
	 	}
	}
	return colors;
}

QStringList GasApplication::getAgents() const
{
	if(!glm->instance()->validLicenseExists())
		return AvailableAgents;
	QStringList agents;
	QStringList fromINI = gasApp->ReadProfile( "Agents", "Agents", QStringList() ).toStringList();
	foreach( QString sectionName, fromINI )
	{
		QString agent = gasApp->ReadProfile( sectionName, "LocalName", QString() ).toString();
		agents << (agent.length() > 0 ? agent : sectionName);
	}
	return agents;
}

QString GasApplication::getNextAvailableColor(const GasDoc * const thisDoc) const
{
	QStringList colors = getColors();
	int nColors = colors.length();
	int uses[50][3];

	for( int i=0; i<nColors; i++ )
		for( int j=0; j<3; j++ )
			uses[i][j] = 0;

	// Count uses by color:
	// uses[color][0=default, 1=otherDoc, 2=thisDoc]

	//DOES NOT WORK! WIPES OUT m_anesArray: foreach( GasAnesthetic* anes, thisDoc->m_anesArray )
	for( int j = 0; j < thisDoc->m_anesArray.length(); j++)
	{
		GasAnesthetic* anes = thisDoc->m_anesArray[j];
		int i = colors.indexOf(anes->m_defaultColor);
		if ( i >= 0 )
			uses[i][0] += 1;
	}

	foreach( GasDoc* doc, docs )
	{
		int useType = doc != thisDoc ? 1 : 2;
		foreach( QString color, doc->colors )
		{
			int i = colors.indexOf(color);
			if ( i >= 0 )
				uses[i][useType] += 1;
		}
	}

	// Find a disused color
	int minUsedEntry = -1, minUsedTotal = 65535;
	for( int i=0; i<nColors; i++ )
	{
		int total = 0;
		for( int j=0; j<3; j++ )
			total += uses[i][j];
		if (  total == 0 )
			return colors.at(i);
		if ( total < minUsedTotal )
		{
			minUsedTotal = total;
			minUsedEntry = i;
		}
	}

	// ...or a color we don't use
	for( int i=0; i<nColors; i++ )
	{
		if( uses[i][0] + uses[i][2] == 0 )
			return colors.at(i);
	}

	// ...or the least-used color
	return colors.at(minUsedEntry);
}
