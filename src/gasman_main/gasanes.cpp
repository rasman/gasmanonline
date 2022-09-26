#include "gasanes.h"
#include <QObject>
#if QT_VERSION >=0x050000
#include <QMessageBox>
#else
#include <QtGui>
#endif
#include "gasapplication.h"
#include "gaslicensemanager.h"
#include "gasdoc.h"


/////////////////////////////////////////////////////////////////////////////
// Anesthetic construction/destruction

GasAnesthetic::GasAnesthetic()
{
	m_strSectionName = "Unknown";
	m_strName = tr( "Unknown" );
	m_strFmt = "%4.2f";
	m_defaultColor = QString();
	m_fLambdaBG = 1.4F;
	m_fMAX_DEL = 5.0F;
	m_fDefDEL = 0.0F;
	m_fAmbient = 0.0F;
	m_fMAC = 0.0F;
	m_fSolubility[CKT] = 1.0F;
	m_fSolubility[ALV] = 1.0F;
	m_fSolubility[VRG] = 2.6F;
	m_fSolubility[MUS] = 5.2F;
	m_fSolubility[FAT] = 60.0F;
	m_fSolubility[VEN] = m_fLambdaBG;
	m_wVolatility = 0;
	m_fDefUnitDose = 0.0F;
	m_bIsLiquid = false;
	m_fBottleCost = 0.0F;
	m_nBottleSize = 0;
	m_wHighFGF = 0;
	m_wHighVA = 0;
	m_wHighCO = 0;
}

GasAnesthetic::~GasAnesthetic()
{
}

/////////////////////////////////////////////////////////////////////////////
//Anesthetic Implementation

// Assignment

GasAnesthetic& GasAnesthetic::operator=(const GasAnesthetic& otherAnes)
{
	if (&otherAnes == this) return *this;
	m_strSectionName = otherAnes.m_strSectionName;
	m_strName = otherAnes.m_strName;
	m_strFmt = otherAnes.m_strFmt;							
	m_fLambdaBG = otherAnes.m_fLambdaBG;
	m_fMAX_DEL = otherAnes.m_fMAX_DEL;
	m_fDefDEL = otherAnes.m_fDefDEL;
	m_fAmbient = otherAnes.m_fAmbient;
	m_fMAC = otherAnes.m_fMAC;
	for (int i=0; i<MAX_COMPART; i++)
		m_fSolubility[i] = otherAnes.m_fSolubility[i];
	m_wVolatility = otherAnes.m_wVolatility;
	m_fDefUnitDose = otherAnes.m_fDefUnitDose;
	m_bIsLiquid = otherAnes.m_bIsLiquid;
	m_fBottleCost = otherAnes.m_fBottleCost;
	m_nBottleSize = otherAnes.m_nBottleSize;
	m_defaultColor = otherAnes.m_defaultColor;
	return *this;
}

// Read in profile of anesthetic.  Return false if required parameter
// missing, or on format error.

bool GasAnesthetic::Erm( const QString &strParam )
{
	QString strMessage = tr( "<p>Error reading '%1' parameter for anesthetic '%2'. Agent ignored." ).arg( strParam ).arg( m_strSectionName );
	QMessageBox::critical( 0, tr( "Gas Man\xC2\xAE"), strMessage );
	return false;
}

bool GasAnesthetic::ReadProfile()
{
	// Read in the profile strings

	QString str = gasApp->ReadProfile( m_strSectionName, "LocalName", QString() ).toString();
	if ( str.length() )
		m_strName = str;
	else
		m_strName = m_strSectionName;

	str = gasApp->ReadProfile( m_strSectionName, "Format", QString() ).toString();
	int i = str.length();
	if ( i != 0 ) {
		if ( i != 5 ) return Erm( "Format" );
		if ( !( str[1].isDigit() && str[3].isDigit() ) ) return Erm( "Format" );
		if ( str[0] != '%' || str[2] != '.' || str[4] != 'f' ) return Erm( "Format" );
		m_strFmt = str;
	}

	m_fSolubility[VEN] = m_fLambdaBG = (float)gasApp->ReadProfile( m_strSectionName, "Lambda", 0.0 ).toDouble();
	if ( m_fLambdaBG <= 0.0F || m_fLambdaBG > 100.0F ) return Erm( "Lambda" );

	str = gasApp->ReadProfile( m_strSectionName, "Max", QString() ).toString();
	if ( str.length() ) {
		m_fMAX_DEL = str.toFloat();
		if ( m_fMAX_DEL <= 0.0F || m_fMAX_DEL > 2000.0F ) return Erm( "Max" );
	}

	str = gasApp->ReadProfile( m_strSectionName, "High", QString() ).toString();
	if ( str.length() ) {
		m_wHighDEL = str.toUInt();
		if ( m_wHighDEL <= 0 || m_wHighDEL > (quint16)m_fMAX_DEL ) return Erm( "High" );
	}
	else m_wHighDEL = (quint16)(int)m_fMAX_DEL;

	str = gasApp->ReadProfile( m_strSectionName, "Default", QString() ).toString();
	if ( str.length() ) {
		m_fDefDEL = str.toFloat();
		if ( m_fDefDEL < 0.0F || m_fDefDEL > m_fMAX_DEL ) return Erm( "Default" );
	}
	else m_fDefDEL = m_fMAX_DEL/2.0F;
	
	str = gasApp->ReadProfile( m_strSectionName, "Ambient", QString() ).toString();
	if ( str.length() ) {
		m_fAmbient = str.toFloat();
		if ( m_fAmbient <= 0.0F || m_fAmbient > m_fMAX_DEL ) return Erm( "Ambient" );
	}

	str = gasApp->ReadProfile( m_strSectionName, "MAC", QString() ).toString();
	if ( str.length() ) {
		m_fMAC = str.toFloat();
		if ( m_fMAC <= 0.0F || m_fMAC > m_fMAX_DEL ) return Erm( "Mac" );
	}

	str = gasApp->ReadProfile( m_strSectionName, "Volatility", QString() ).toString();
	if ( str.length() ) {
		m_wVolatility = str.toUInt();
		if ( m_wVolatility <= 0 || m_wVolatility > 1000 ) return Erm( "Volatility" );
	}

	str = gasApp->ReadProfile( m_strSectionName, "DefUnitDose", QString() ).toString();
	if ( str.length() ) {
		m_fDefUnitDose = str.toFloat();
		if ( m_fDefUnitDose <= 0.0F || m_fDefUnitDose > 50.0F ) return Erm( "DefUnitDose" );
	}

	m_bIsLiquid = m_fDefUnitDose != 0.0F;

	str = gasApp->ReadProfile( m_strSectionName, "BottleCost", QString() ).toString();
	if ( str.length() ) {
		m_fBottleCost = str.toFloat();
		if ( m_fBottleCost <= 0.0F || m_fBottleCost > MAX_BOTTLE_COST ) return Erm( "BottleCost" );
	}

	str = gasApp->ReadProfile( m_strSectionName, "BottleSize", QString() ).toString();
	if ( str.length() ) {
		m_nBottleSize = str.toInt();
		if ( m_nBottleSize <= 0 || m_nBottleSize > MAX_BOTTLE_SIZE ) return Erm( "BottleSize" );
	}

	m_defaultColor = DefaultColorFromProfile( m_strSectionName );

	for ( int i = 0; i < MAX_COMPART; ++i ) {
		str = gasApp->ReadProfile( m_strSectionName, listCompart[i], QString() ).toString();
		if ( !str.length() ) {
			if (!(i==VRG||i==MUS||i==FAT)) continue;
			return Erm( listCompart[i] );
		}
		m_fSolubility[i]= str.toFloat();
		if ( m_fSolubility[i] <= 0.0F || m_fSolubility[i] > 500.0F ) return Erm( listCompart[i] );
	}
	
	return true;
} 

QString GasAnesthetic::DefaultColorFromProfile( const QString& name )
{
	QString str = gasApp->ReadProfile(name, "Color", QString()).toString();
	if ( str.length() ) {
		QStringList colors = gasApp->getColors();
		QStringList test = colors.filter(str, Qt::CaseInsensitive);
		for( int i=test.length()-1; i>=0; --i)
			if( test.at(i).length() != str.length() )
				test.removeAt(i);
		if ( test.length() != 1 || !test.contains(str, Qt::CaseInsensitive) )
		{
			Erm( "Color" );
			return QString();
		}
		str = test.at(0);
	}
	return str;
}


/////////////////////////////////////////////////////////////////////////////
// GasAnesthetic serialization

void GasAnesthetic::Serialize( QDataStream &stream, bool storing, quint32 serializationFlags )
{	
	if ( storing ) {
		stream << m_strSectionName << m_strName << m_strFmt;
		stream << m_wHighDEL;
		stream << m_fLambdaBG << m_fMAX_DEL << m_fDefDEL << m_fAmbient << m_fMAC
		   << m_wVolatility << m_fDefUnitDose << m_fBottleCost << (quint16)m_nBottleSize;
		//stream.writeRawData( (const char *)m_fSolubility, sizeof( m_fSolubility ) );
		for ( int i=0; i<MAX_COMPART; ++i )
				stream << m_fSolubility[i];
	}
	else {
		quint16 w;
		stream >> m_strSectionName;
		if ( (serializationFlags & GasDoc::SER_LOCAL_AGENT) != 0 )
			stream >> m_strName;
		else
			m_strName = m_strSectionName;
		stream >> m_strFmt;
		stream >> m_wHighDEL;
		stream >> m_fLambdaBG >> m_fMAX_DEL >> m_fDefDEL >> m_fAmbient >> m_fMAC
		   >> m_wVolatility >> m_fDefUnitDose >> m_fBottleCost >> w;
		m_nBottleSize = w;
		//stream.readRawData( (char *)m_fSolubility, sizeof(m_fSolubility) );
		for ( int i=0; i<MAX_COMPART; ++i )
				stream >> m_fSolubility[i];
		m_bIsLiquid = m_fDefUnitDose != 0.0F;
		m_defaultColor = DefaultColorFromProfile( m_strName );
	}
}


/////////////////////////////////////////////////////////////////////////////
// AnesArray

// Overloaded operator to treat string as subscript

int GasAnesArray::Lookup( const QString &strName )
{
	int i;

	for ( i=0; i < size() && at(i)->m_strName < strName; i++ );
	if ( i >= size() || at(i)->m_strName > strName ) return -1;
	return i;
}

int GasAnesArray::LookupSection( const QString &strName )
{
	int i;

	for ( i=0; i < size() && at(i)->m_strSectionName != strName; i++ );
	return i < size() ? i : -1;
}

void GasAnesArray::Insert( GasAnesthetic *pAnes )
{
	int i;

	for ( i = 0; i < size() &&((GasAnesthetic *)at(i))->m_strName < pAnes->m_strName; i++ );
	insert(i, pAnes);
}

// Reads in the parameters for each anesthetic and returns count of same.
// MAY THROW MEMORY EXCEPTION!!

int GasAnesArray::ReadProfile()
{
	int nAnesCount = 0;
	
	clear();

	QStringList strList = gasApp->ReadProfile( "Agents", "Agents", QStringList() ).toStringList();
	for ( int i = 0; i < strList.size(); ++i ) {
		if(!glm->instance()->validLicenseExists())
			if ( !AvailableAgents.contains( strList[i], Qt::CaseInsensitive ) ) continue;
		GasAnesthetic *pAnes = new GasAnesthetic;
		pAnes->m_strSectionName = strList[i];
		if ( pAnes->ReadProfile() ) {
			Insert( pAnes );
			nAnesCount++;
		}
		else
			delete pAnes;
	}
	return nAnesCount;
}

