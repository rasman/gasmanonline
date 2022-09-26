
#ifndef GASANES_H
#define GASANES_H

#include <QObject>
#include "gasglobal.h"
#include "gasptrlist.h"


class GasAnesthetic : public QObject
{
	Q_OBJECT
public:
	GasAnesthetic();
	virtual ~GasAnesthetic();

// Operations
public:
	GasAnesthetic& operator=( const GasAnesthetic& otherAnes );
	bool ReadProfile();																	// Look up above in gas.ini
	void Serialize( QDataStream &stream, bool storing, quint32 serializationFlags );
	inline float ToMlLiquid( const float litersGas ) const {
		return m_wVolatility > 0 ? 1000 * litersGas / m_wVolatility : litersGas;
	};

private:
	bool Erm( const QString &strParam );
	QString DefaultColorFromProfile( const QString& name );

// Attributes
public:
	QString m_strSectionName;		// INI uses this name
	QString m_strName;				// User sees this name
	QString	m_strFmt;				// Format string, like "%4.1f"
	QString m_defaultColor;			// What the ini tells us
	quint16	m_wVolatility;			// Gas produced for liquid injection
	quint16	m_wHighDEL;				// Highest gauge setting for DEL
	quint16	m_wHighFGF;
	quint16	m_wHighVA;
	quint16	m_wHighCO;
	float m_fLambdaBG;				// Blood/Gas solubility
	float m_fMAX_DEL;				// Highest allowed DEL setting
	float m_fDefDEL;				// Default vaporizer setting
	float m_fAmbient;				// float-ing around
	float m_fMAC;					// Minimum Alveolar Concentration (dose)
	float m_fDefUnitDose;			// Default liquid injected
	float m_fBottleCost;			// $ per bottle
	int	 m_nBottleSize;				// in ml
	bool m_bIsLiquid;				// Set if anesthetic is injectable/vaporized
	float m_fSolubility[MAX_COMPART];	// Lambda by compartment	
};

/////////////////////////////////////////////////////////////////////////////
//An Array of Anesthetic parameters

class GasAnesArray : public GasPtrList<GasAnesthetic>
{
public:
	void Insert( GasAnesthetic *pAnes );			// Put in sorted order
	int Lookup( const QString &strName );			// Do linear search for localized name
	int LookupSection( const QString &strName );	// Do linear search for section (canonical) name
	int ReadProfile();								// Read gas.ini
};


#endif
