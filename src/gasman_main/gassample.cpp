
#include "gassample.h"

#include "gasdoc.h"
#include "gas.h"

/////////////////////////////////////////////////////////////////////////////
// GasSample

GasSample::GasSample( Gas *pGas, RES_ARRAY& fResults )
{
	GasDoc *pDoc = pGas->m_pDoc;
	m_dwBaseTime = pDoc->m_dwCalcTime;
	m_fDEL		= pGas->m_fDEL;
	m_fFGF		= pDoc->m_fFGF;
	m_fVA		= pDoc->m_fVA;
	m_fCO		= pDoc->m_fCO;
	m_fWeight	= pDoc->m_fWeight;
	for ( int i=0; i < MAX_RESULT; ++i )
		m_fResults[i] = fResults[i];
	m_fUnitDose	= pGas->m_fUnitDose;
	m_wAnesID	= (quint16)pGas->m_nAgent;
	m_bRtnEnb	= pDoc->m_bRtnEnb;
	m_bUptEnb	= pDoc->m_bUptEnb;
	m_bVapEnb	= pDoc->m_bVapEnb;
	 
		 
	if ( pDoc->m_strCircuit.at(0) == tr( "Open", "open circuit" ).at(0) ) /*open*/
		m_uCktType = OPEN_CKT;
	else if ( pDoc->m_strCircuit.at(0) == tr( "Semi-closed" ).at(0) ) /*semi*/
		m_uCktType = SEMI_CKT;
	else if ( pDoc->m_strCircuit.at(0) == tr( "Closed" ).at( 0 ) ) /*closed*/
		m_uCktType = CLOSED_CKT;
	else if ( pDoc->m_strCircuit.at(0) == tr( "Ideal" ).at( 0 ) ) /*ideal*/
		m_uCktType = IDEAL_CKT;
	
	m_uInjections = pGas->m_nInjections;
	m_bFlush = (pDoc->m_bFlush!=false);
}

GasSample::GasSample()
{
}

GasSample::~GasSample()
{
}

GasSample::GasSample(const GasSample& otherSamp)
{
	m_dwBaseTime		= otherSamp.m_dwBaseTime;
	m_fDEL				= otherSamp.m_fDEL;
	m_fCO				= otherSamp.m_fCO;
	m_fFGF				= otherSamp.m_fFGF;
	m_fVA				= otherSamp.m_fVA;
	m_fUnitDose			= otherSamp.m_fUnitDose;
	m_fWeight			= otherSamp.m_fWeight;
	m_wAnesID			= otherSamp.m_wAnesID;
	m_bRtnEnb			= otherSamp.m_bRtnEnb;
	m_bUptEnb			= otherSamp.m_bUptEnb;
	m_bVapEnb			= otherSamp.m_bVapEnb;
	m_uCktType			= otherSamp.m_uCktType;
	m_uInjections		= otherSamp.m_uInjections;
	m_bFlush			= otherSamp.m_bFlush;
	for (int i=0; i<MAX_RESULT; i++)
		m_fResults[i] = otherSamp.m_fResults[i];
}

/////////////////////////////////////////////////////////////////////////////
// GasSample serialization
void GasSample::Serialize(QDataStream &stream, bool storing, quint32 )
{	
	if (storing) {
		stream << m_dwBaseTime;
		stream << m_fDEL << m_fFGF << m_fVA << m_fCO << m_fWeight;
		stream << m_fUnitDose << m_wAnesID;
		stream << (quint16)((m_bVapEnb<<9) + (m_bFlush<<8) + (m_uInjections<<4) + (m_uCktType<<2) + (m_bUptEnb<<1) + m_bRtnEnb);
		for (int i=0; i<MAX_RESULT; ++i)
			stream << m_fResults[i];
	}
	else {
		stream >> m_dwBaseTime;
		stream >> m_fDEL >> m_fFGF >> m_fVA >> m_fCO >> m_fWeight;
		stream >> m_fUnitDose >> m_wAnesID;
		{
			quint16 w;
			stream >> w;
			m_bRtnEnb		= w&1;   w>>=1;
			m_bUptEnb		= w&1;   w>>=1;
			m_uCktType		= w&3;   w>>=2;
			m_uInjections	= w&0xF; w>>=4;
			m_bFlush		= w&1;   w>>=1;
			m_bVapEnb		= w&1;
		}
		for (int i=0; i<MAX_RESULT; ++i)
			stream >> m_fResults[i];
	}
}

