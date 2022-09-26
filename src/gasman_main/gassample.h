
#ifndef GASSAMPLE_H
#define GASSAMPLE_H

#include <QObject>
#include "gasglobal.h"
#include "gasptrlist.h"


enum ACktType
{
	OPEN_CKT,
	SEMI_CKT,
	CLOSED_CKT,
	IDEAL_CKT
};

class Gas;

class GasSample: public QObject
{
	Q_OBJECT
public:
	GasSample();
	GasSample( Gas* pGas, RES_ARRAY &fResults );
	GasSample(const GasSample& otherSamp);
	virtual ~GasSample();
	void Serialize(QDataStream &stream, bool storing, quint32 serializationFlags );

// Attributes
public:
	quint32		m_dwBaseTime;		//When this block starts
	float		m_fDEL;				//DEL setting for this block
	float		m_fCO;				//CO for this block
	float		m_fFGF;				//FGF...
	float		m_fVA;
	float		m_fUnitDose;
	float		m_fWeight;
	quint16		m_wAnesID;			//Index into CGasDoc::m_anesArray
	unsigned	m_bRtnEnb:1;
	unsigned	m_bUptEnb:1;
	unsigned	m_uCktType:2;
	unsigned	m_uInjections:4;
	unsigned	m_bFlush:1;
	unsigned	m_bVapEnb:1;
	RES_ARRAY	m_fResults;
};

typedef GasPtrList<GasSample> GasSampArray;

#endif
