
#ifndef GASOVERLAYDLG_H
#define GASOVERLAYDLG_H

#include "ui_gasoverlay.h"


class GasGraphView;
class GasDoc;

class GasOverlay : public QWidget, private Ui::GasOverlay
{
	Q_OBJECT
public:
	GasOverlay( QWidget *parent = 0 );

	void init( GasGraphView *pGasGraph );			//Initialization
	static void ReadProfile();

	void UpdateData( bool bSaveAndValidate = true );

private slots:
	void help();
	void selectionClicked( QAbstractButton * );
	void endPainting();

protected:
	virtual void customEvent( QEvent *evnt );

private:
	void OnYValReqGraph( float * X, float *Y, float *lastX );

	enum ANumerator {
		ShowDEL = 0,
		ShowCKT,
		ShowALV,
		ShowVRG,
		ShowMUS,
		ShowFAT,
		ShowVEN,
		ShowCost
	};

	enum ADenominator {
		OverDEL = 0,
		OverCKT,
		OverALV,
		OverVRG,
		OverMUS,
		OverFAT,
		OverVEN,
		OverMAC,
		OverOne
	};

public:
	static ANumerator m_nWhich;
	static ADenominator m_nOverWhich;

private:
	QButtonGroup *bgShow;
	QButtonGroup *bgOver;
	GasDoc *m_pDocs[5];						//Up to 5 simultaneous sims
	int m_nGas[5];							//0=primary, 1=secondary
	float m_fMAC[5];						//For normalization by MAC
	float m_fXSize;
	float m_fXValEvery;
	float m_fYSize;							//From graph that called us
	float m_fYTickEvery;
	float m_fYLabelEvery;
	float m_fLast[5];
	float m_fMax;
	float m_fSlope;
	bool m_bScanning;
	float m_fSum;
	float m_fSquareSum;						//...in these variables
	int m_nCount;
	bool needUpdate;
	QWidget* m_par;
};

#endif
