#include "gasglobal.h"

#include <QtCore>
#include <QDebug>
#include <QColor>
static QString timestamp()
{
    int stamp = 0;
    QDate gasmanepoch(2000, 1, 1);
    QString todaystring = __DATE__;

#if 0
    todaystring.replace("  ", " "); // fix up space padded days
    QDate today = QDate::fromString(todaystring, "MMM d yyyy");
#else
	// This works internationally, presuming the build is done in the USA

    QStringList todayItems = todaystring.split(" ", Qt::SkipEmptyParts);
	int month = QString("    JAN FEB MAR APR MAY JUN JUL AUG SEP OCT NOV DEC").indexOf(todayItems[0].toUpper()) / 4;
	QDate today(todayItems[2].toInt(), month, todayItems[1].toInt());
#endif

    if (today.isValid() &&( today > gasmanepoch)) {
        stamp = gasmanepoch.daysTo(today);
    }

    return QString::number(stamp);
}

QString gasmanVersion = QString("4.3.%1").arg(timestamp());

QStringList listCompart = ( QStringList() << "CKT" << "ALV" << "VRG" << "MUS"
                            << "FAT" << "VEN" );	// For Internal Use; do not translate

QStringList listTargetCompart;

QStringList listCircuit;

QStringList listSpeed;

QStringList listTime;

QStringList listColors;

static QStringList listQColors;

QString ToQColor(const QString &color)
{
	for (int i=0; i<listColors.length(); i++)
		if (color == listColors.at(i))
			return listQColors.at(i);
	return QColor::colorNames().at(Qt::black);
}


void InitializeGlobalLists()
{
	listTargetCompart = ( QStringList()
		<< QObject::tr( "CKT" )
		<< QObject::tr( "ALV" )
		<< QObject::tr( "VRG" )
		<< QObject::tr( "MUS" )
		<< QObject::tr( "FAT" )
		<< QObject::tr( "VEN" ) );

	listCircuit = ( QStringList() << QObject::tr( "Open", "open circuit" )
                            << QObject::tr( "Semi-closed" )
                            << QObject::tr( "Closed" )
                            << QObject::tr( "Ideal" ) );

	listSpeed = ( QStringList() << "1x" << "2x" << "5x" << "10x"
                          << "20x" << "60x" << "120x"
                          << QObject::tr("AFAP") );

	listTime =  ( QStringList() << QObject::tr( "5 Min" )
                          << QObject::tr( "10 Min" ) << QObject::tr( "15 Min" )
                          << QObject::tr( "30 Min" ) << QObject::tr( "45 Min" )
                          << QObject::tr( "1 Hr" )  << QObject::tr( "2 Hr" )
                          << QObject::tr( "6 Hr" ) << QObject::tr( "12 Hr" )
						  << QObject::tr( "1 Day" ) );

	listColors = ( QStringList()  << QObject::tr("red") << QObject::tr("darkRed")
                         << QObject::tr("blue") << QObject::tr("darkBlue")
						 << QObject::tr("green") << QObject::tr("darkGreen")
						 << QObject::tr("orange")<< QObject::tr("purple")
						 << QObject::tr("black") << QObject::tr("white")
						 << QObject::tr("yellow") << QObject::tr("cyan") );

	listQColors = ( QStringList()  << QString("red") << QString("darkRed")
                         << QString("blue") << QString("darkBlue")
						 << QString("green") << QString("darkGreen")
						 << QString("orange")<< QString("purple")
						 << QString("black") << QString("white")
						 << QString("yellow") << QString("cyan") );
}

//This routine returns the base name of the file without the file path
QString completeBaseName( const QString &fullFileName )
{
    return QFileInfo( fullFileName ).completeBaseName();
}

//This routine returns the name of the file without the file path
QString fileName( const QString &fullFileName )
{
    return QFileInfo( fullFileName ).fileName();
}

QList<GasDoc *> docs;

