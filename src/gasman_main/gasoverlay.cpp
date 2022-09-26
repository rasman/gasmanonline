#include "gasoverlay.h"

#if QT_VERSION >=0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif
#include "gasapplication.h"
#include "gasmainwindow.h"
#include "gasgraphview.h"
#include "gaschildwindow.h"
#include "gasdoc.h"
#include "gaspanelview.h"
#include "gastabwidget.h"
static double fFixedYSize;
static double fSlope;
GasOverlay::ANumerator GasOverlay::m_nWhich;
GasOverlay::ADenominator GasOverlay::m_nOverWhich;

#define MAX_OVLY_HEIGHT 500000.0F

enum ADistinction {
	DIF_AGENT,
	DIF_WEIGHT,
	DIF_CKT,
	DIF_CO,
	DIF_VA,
	DIF_MAX
};


//static QList<QColor> co; 
GasOverlay::GasOverlay( QWidget *parent )
	: QWidget( parent, Qt::Window | Qt::WindowMaximizeButtonHint  ), m_nCount( 0 ), needUpdate( false )
{
	setupUi( this );
	setWindowModality( Qt::ApplicationModal );
	setAttribute( Qt::WA_DeleteOnClose );
	
	m_Graph->setReceiver( this );
	m_Graph->setType(GRAPHTYPE_OVERLAY);
	m_fYSize = fFixedYSize;
	m_fYTickEvery = 1.0F;
	m_fYLabelEvery = fFixedYSize;
	m_bScanning = false;
	m_par = parent;
	bgShow = new QButtonGroup( this );
	bgShow->addButton( rbDEL, ShowDEL );
	bgShow->addButton( rbCKT, ShowCKT );
	bgShow->addButton( rbALV, ShowALV );
	bgShow->addButton( rbVRG, ShowVRG );
	bgShow->addButton( rbMUS, ShowMUS );
	bgShow->addButton( rbFAT, ShowFAT );
	bgShow->addButton( rbVEN, ShowVEN );
	bgShow->addButton( rbCost, ShowCost );
	bgShow->button( m_nWhich )->setChecked( true );


	bgOver = new QButtonGroup( this );
	bgOver->addButton( rbOverDEL, OverDEL );
	bgOver->addButton( rbOverCKT, OverCKT );
	bgOver->addButton( rbOverALV, OverALV );
	bgOver->addButton( rbOverVRG, OverVRG );
	bgOver->addButton( rbOverMUS, OverMUS );
	bgOver->addButton( rbOverFAT, OverFAT );
	bgOver->addButton( rbOverVEN, OverVEN );
	bgOver->addButton( rbOverMAC, OverMAC );
	bgOver->addButton( rbOverOne, OverOne );
	bgOver->button( m_nOverWhich )->setChecked( true );

	//create connections
	connect( bgOver, SIGNAL( buttonClicked( QAbstractButton * ) ), this, SLOT( selectionClicked( QAbstractButton * ) ) );
	connect( bgShow, SIGNAL( buttonClicked( QAbstractButton * ) ), this, SLOT( selectionClicked( QAbstractButton * ) ) );
	connect( buttonBox, SIGNAL( rejected() ), this, SLOT( close() ) );
	connect( buttonBox, SIGNAL( helpRequested() ), this, SLOT( help() ) );

	connect( m_Graph, SIGNAL( endDrawLines() ), this, SLOT( endPainting() ) );
	
	resize( minimumSizeHint() );
//	co <<Qt::red<< Qt::blue<<Qt::green<< Qt::magenta << Qt::black<<Qt::white<< Qt::yellow<< Qt::cyan<<Qt::darkBlue;
}

void GasOverlay::ReadProfile()
{
	QStringList lstShow;
	lstShow.append( "DEL" );
	lstShow.append( "CKT" );
	lstShow.append( "ALV" );
	lstShow.append( "VRG" );
	lstShow.append( "MUS" );
	lstShow.append( "FAT" );
	lstShow.append( "VEN" );
	lstShow.append( "COST" );

	QStringList lstOver;
	lstOver.append( "DEL" );
	lstOver.append( "CKT" );
	lstOver.append( "ALV" );
	lstOver.append( "VRG" );
	lstOver.append( "MUS" );
	lstOver.append( "FAT" );
	lstOver.append( "VEN" );
	lstOver.append( "MAC" );
	lstOver.append( "ONE" );
	
	fFixedYSize = gasApp->ReadProfile( "Defaults", "OverlayHeight", 0 ).toDouble();
	if ( fFixedYSize < 1.0F || fFixedYSize > 1000.0F ) fFixedYSize = 5.0F;
	
	fSlope = gasApp->ReadProfile( "Defaults", "OverlaySlope", 0 ).toDouble();
	if ( fSlope < 1.0F || fSlope > 50.0F ) fSlope = 10.0F;
	
	QString str = gasApp->ReadProfile( "Defaults", "OverlayShow", "ALV" ).toString();
	m_nWhich = (ANumerator)lstShow.indexOf( str.toUpper() );
	
	str = gasApp->ReadProfile( "Defaults", "OverlayOver", "ONE" ).toString();
	m_nOverWhich = (ADenominator)lstOver.indexOf( str.toUpper() );
}

//Initialization
void GasOverlay::init( GasGraphView *pGasGraph )
{	
	GasGraph *pCtl = pGasGraph->IARMGraph();
	/* GasChildWindow * cur = */gasMainWindow->activeChildForced();
	bool bMAC_OK = true;

	float f, fXTickEvery, fXLabelEvery;
	float fXVal   =  pCtl->GetXVal();
	fXVal	+= pCtl->GetXSize();
	float fOrigin =  pCtl->GetOrigin();
	GasDoc *pDoc;

	int nCount = 0;

	// BEGIN HACK: strip out multiple views of same agent so they don't mess up naming scheme
	QMultiMap<GasDoc*, int> agentsAdded;
	GasChildren strippedList;
	foreach(QMdiSubWindow * w, gasMainWindow->getSubWins()){
		if(w->isMinimized())
			continue;
		GasTabWidget * tab = qobject_cast<GasTabWidget*>(w->widget());	
	    for(int i = 0; i< tab->count(); i++){
			GasChildWindow * cw  = qobject_cast<GasChildWindow*>(tab->widget(i));
			int nGas;
			if ( cw->getViewType()==GRAPH  && cw->tryGetGas(nGas) && !agentsAdded.values(cw->doc()).contains(nGas) )
			{
				strippedList<<cw;
				agentsAdded.insert(cw->doc(), nGas);
			}
		}
	}

	QMap<QString, GasChildWindow*> sortList;
	foreach(GasChildWindow * w , strippedList){
		sortList.insert(w->title(),w);
	}
 
	strippedList.clear();
	strippedList= sortList.values();
	// END HACK

	GasChildIterator it( strippedList );
	while ( it.hasNext() && nCount < int( sizeof(m_pDocs)/sizeof(GasDoc *) ) ) { //For all the documents
		
		GasChildWindow *pChild = it.next();
		pDoc = pChild->doc();

		for ( int ng = 0; ng < pDoc->m_gasArray.size() && nCount < int( sizeof(m_pDocs)/sizeof(GasDoc *) ); ++ng ) {//For all gasses in the document
			int nGas;
			if (!pChild->tryGetGas(nGas) || nGas != ng)
				continue;
			f = pDoc->GetTime()/60.0F;
			if (f >= fOrigin)
			{//If there are values in the interval examined
				m_pDocs[nCount] = pDoc;
				m_nGas[nCount] = ng;
				m_fMAC[nCount] = f = pDoc->GetAgent(ng)->m_fMAC;
				if (f <= 0.0F) bMAC_OK = false;
				nCount++;
			}
		}

	}
	int nDistinctions;
	int nDoc, nUnique, i;
	QString str;
	QString strDif[10];

	for ( nDistinctions = 1; nDistinctions < 1 << DIF_MAX; ++nDistinctions ) {//Figure out how to label the lines; i.e., find minimal distinction
		nUnique = 0;
		for ( nDoc = 0; nDoc < nCount; ++nDoc ) {
			pDoc = m_pDocs[nDoc];
			str = "";
			if ( nDistinctions&(1<<DIF_AGENT)) {
				QString temp;
				temp.fill( '+', pDoc->m_gasArray.size() - 1 );
				str += pDoc->GetAgent(m_nGas[nDoc])->m_strName + temp;
			}
			
			if (nDistinctions&(1<<DIF_WEIGHT))
				str += QString::number((int)(pDoc->GetWeight()*10));
			
			if (nDistinctions&(1<<DIF_CKT))
				str += (pDoc->GetCircuit())[0];
			
			if (nDistinctions&(1<<DIF_CO))
				str += QString::number((int)(pDoc->GetCO()*10));
						
			if (nDistinctions&(1<<DIF_VA))
				str += QString::number((int)(pDoc->GetVA()*10));
			
			for ( i = 0; i < nUnique; ++i )
				if (str == strDif[i]) break;
			if ( i >= nUnique) strDif[nUnique++] = str;
			if ( nUnique <= nDoc ) break;
		}
		if (nUnique == nCount) break;
	}

	for ( nDoc = 0; nDoc < nCount; ++nDoc )	{//Build the label for each dataset
		pDoc = m_pDocs[nDoc];
		str = pDoc->title();
		str += " (";
		//if (nDistinctions&(1<<DIF_AGENT)) {
		QString temp;
		temp.fill( '+', pDoc->m_gasArray.size() - 1 );
		str += pDoc->GetAgent(m_nGas[nDoc])->m_strName + temp + "/";
		//}
		if (nUnique == nCount) {
			if (nDistinctions&(1<<DIF_WEIGHT))
				str = str + QString::number( pDoc->GetWeight() ) + tr(" Kg") + "/";
			
			if (nDistinctions&(1<<DIF_CKT))
				str += pDoc->GetCircuit() + "/" ;
			
			if (nDistinctions&(1<<DIF_CO))
				str = str + tr( "CO" ) + "=" + QString::number( pDoc->GetCO() ) + "/";
			
			if (nDistinctions&(1<<DIF_VA))
				str = str + tr("VA") + "=" + QString::number(pDoc->GetVA()) + "/";			
		}
		str.replace( str.length()-1, 1, ")" );
		QLabel *lGraph;
		QColor color;

		switch (nDoc)
		{
		case 0: lGraph = lGraph_0; color = ToQColor(pDoc->colors.at(m_nGas[nDoc])); m_Graph->SetColor0(color);break;
		case 1: lGraph = lGraph_1; color = ToQColor(pDoc->colors.at(m_nGas[nDoc])); m_Graph->SetColor1(color);break;
		case 2: lGraph = lGraph_2; color = ToQColor(pDoc->colors.at(m_nGas[nDoc])); m_Graph->SetColor2(color);break;
		case 3: lGraph = lGraph_3; color = ToQColor(pDoc->colors.at(m_nGas[nDoc])); m_Graph->SetColor3(color);break;
		case 4: lGraph = lGraph_4; color = ToQColor(pDoc->colors.at(m_nGas[nDoc])); m_Graph->SetColor4(color);break;
		default:Q_ASSERT(false);
		}

		lGraph->setText( str );
		QPalette p = lGraph->palette();
		p.setColor( QPalette::WindowText, color );
		lGraph->setPalette( p );
	}

	if ( !bMAC_OK ) rbOverMAC->setEnabled(false);

	m_Graph->SetLoading( true  );

	m_fXSize       = pCtl->GetXSize();
	fXTickEvery    = pCtl->GetXTickEvery();
	fXLabelEvery   = pCtl->GetXLabelEvery();
	m_fXValEvery   = pCtl->GetXValEvery();
	m_fYSize       = pCtl->GetYSize();
	m_fYTickEvery  = pCtl->GetYTickEvery();
	m_fYLabelEvery = pCtl->GetYLabelEvery();
	QColor lPaperColor    = pCtl->GetPaperColor();
	qint16 lLineLabelType = pCtl->GetLineLabelType();
	qint16 lLineWidth     = pCtl->GetLineWidth();

	m_Graph->SetOrigin(fOrigin);
	m_Graph->SetXSize(m_fXSize);
	m_Graph->SetXTickEvery(fXTickEvery);
	m_Graph->SetXLabelEvery(fXLabelEvery);
	m_Graph->SetXValEvery(m_fXValEvery);
	m_Graph->SetYSize(m_fYSize);
 
	m_Graph->SetYTickEvery(m_fYTickEvery);
	m_Graph->SetYLabelEvery(m_fYLabelEvery);
	m_Graph->SetPaperColor(lPaperColor);
	m_Graph->SetLineLabelType(lLineLabelType);
	m_Graph->SetLineWidth(lLineWidth);
	m_Graph->SetBMarginPct(20);
	m_Graph->SetRMarginPct(5);
	m_Graph->SetTMarginPct(5);
	m_Graph->SetLMarginPct(17);
	m_Graph->SetVals(nCount);
	m_Graph->SetXVal(fXVal);
	m_Graph->SetLoading(false);
}

void GasOverlay::endPainting()
{
	if ( !needUpdate )
		return;

	needUpdate = false;
	
	m_Graph->SetLoading( true );
	m_bScanning = false;
	
	float f1, f2, f3;

	if ( m_nCount ) {
		f1 = m_fMax;

		if      (f1 <= 0.1F )       {f1 = 0.10001F;                       f2 = 0.025F;    f3 = 0.05F;}
		else if (f1 <= 0.5F )       {f1 = (float)ceil(f1*10)/10+0.00001F; f2 = 0.1F;      f3 = (float)ceil(f1*10)/10;}
		else if (f1 <= 1.0F )       {f1 = 1.0F;                           f2 = 0.25F;     f3 = 0.5F;}
		else if (f1 <= 5.0F )       {f1 = (float)ceil(f1);                f2 = 1.0F;      f3 = f1;}
		else if (f1 <= 10.0F)       {f1 = 10.0F;                          f2 = 1.0F;      f3 = 5.0F;}
		else if (f1 <= 50.0F)       {f1 = (float)ceil(f1/10)*10;          f2 = 10.0F;     f3 = f1;}
		else if (f1 <= 100.0F)      {f1 = 100.0F;                         f2 = 10.0F;     f3 = 50.0F;}
		else if (f1 <= 500.0F)      {f1 = (float)ceil(f1/100)*100;        f2 = 100.0F;    f3 = f1;}
		else if (f1 <= 1000.0F)     {f1 = 1000.0F;                        f2 = 100.0F;    f3 = 500.0F;}
		else if (f1 <= 5000.0F)     {f1 = (float)ceil(f1/1000)*1000;      f2 = 1000.0F;   f3 = f1;}
		else if (f1 <= 10000.0F)    {f1 = 10000.0F;                       f2 = 1000.0F;   f3 = 5000.0F;}
		else if (f1 <= 50000.0F)    {f1 = (float)ceil(f1/10000)*10000;    f2 = 10000.0F;  f3 = f1;}
		else if (f1 <= 100000.0F)   {f1 = 100000.0F;                      f2 = 10000.0F;  f3 = 50000.0F;}
		else                        {f1 = MAX_OVLY_HEIGHT;                f2 = 100000.0F; f3 = f1;}

		m_Graph->SetYSize(f1);
		m_Graph->SetYTickEvery(f2);
		m_Graph->SetYLabelEvery(f3);
		m_fMax = f1;
	}
	
	// if ( bgOver->checkedId() == OverMAC)
	if ( bgOver->checkedId() != OverOne && m_fMax > 1.0F && m_fMax <= 50 )
		m_Graph->SetMac(1.0F);
	else
		m_Graph->SetMac(-1.0F);
	m_Graph->SetLoading( false );
}


void GasOverlay::UpdateData( bool bSaveAndValidate )
{	
	needUpdate = true;
	
	if ( bSaveAndValidate && bgShow->checkedId() == ShowCost ) { //Cost is always over one
		if ( bgOver->checkedId() != OverOne )
			bgOver->button( OverOne )->setChecked( true );
	}

	m_nWhich = (ANumerator)bgShow->checkedId();
	m_nOverWhich = (ADenominator)bgOver->checkedId();

	m_Graph->SetLoading( true );
	m_Graph->SetYSize(m_fMax=m_fYSize);
 
	m_Graph->SetYTickEvery(m_fYTickEvery);
	m_Graph->SetYLabelEvery(m_fYLabelEvery);
	m_Graph->SetLoading( false );

	if ( bgOver->checkedId() != OverOne || bgShow->checkedId() == ShowCost )	{
		m_bScanning = true;
		m_fSum = m_fSquareSum = 0.0F;
		m_fSlope = fSlope * m_fXValEvery * 500 / m_fXSize;
		m_fMax = -1.0F;
		for ( quint32 i = 0; i < sizeof(m_fLast)/sizeof(m_fLast[0]); ++i )
			m_fLast[i] = -1.0F;

		m_nCount = 0;

		m_Graph->update();
	}
}

void GasOverlay::selectionClicked( QAbstractButton * )
{
	UpdateData( true );
}

void GasOverlay::help()
{
	(static_cast<GasMainWindow*>(m_par))->showHelpTopic();
	 
}

void GasOverlay::customEvent( QEvent *evnt )
{
	if ( evnt->type() == (QEvent::Type)(QEvent::User + 2) ) {
		GasGraphEvent *gasGraphEvent = static_cast<GasGraphEvent * >( evnt );
		if ( !gasGraphEvent ) return;

		float *pf = gasGraphEvent->data();
		GasGraph *receiver = gasGraphEvent->receiver();

		if ( receiver == m_Graph )
			OnYValReqGraph(pf, pf+1, pf+2);
		else {
			*pf = -1.0F;
			Q_ASSERT( false );
		}
	}
}

void GasOverlay::OnYValReqGraph( float * X, float *Y, float * /*lastX*/ ) 
{	
	int ny = (int)*Y;
	GasDoc *pDoc = m_pDocs[ny];
	int nGas = m_nGas[ny];

	float x = *X;
	float y = pDoc->GetTime()/60.0F;
	if (x > y)
		y = 0.0F;
	else
	{
		switch(m_nOverWhich)
		{
		case OverDEL: y = pDoc->GetDEL(x, nGas); break;
		case OverCKT: y = pDoc->GetCKT(x, nGas); break;
		case OverALV: y = pDoc->GetALV(x, nGas); break;
		case OverVRG: y = pDoc->GetVRG(x, nGas); break;
		case OverMUS: y = pDoc->GetMUS(x, nGas); break;
		case OverFAT: y = pDoc->GetFAT(x, nGas); break;
		case OverVEN: y = pDoc->GetVEN(x, nGas); break;
		case OverMAC: y = m_fMAC[ny]; break;
		case OverOne: y = 1.0F; break;
		default: Q_ASSERT(false);
		}

		switch (m_nWhich)
		{
		case ShowDEL: x = pDoc->GetDEL(x, nGas); break;
		case ShowCKT: x = pDoc->GetCKT(x, nGas); break;
		case ShowALV: x = pDoc->GetALV(x, nGas); break;
		case ShowVRG: x = pDoc->GetVRG(x, nGas); break;
		case ShowMUS: x = pDoc->GetMUS(x, nGas); break;
		case ShowFAT: x = pDoc->GetFAT(x, nGas); break;
		case ShowVEN: x = pDoc->GetVEN(x, nGas); break;
		case ShowCost: x = pDoc->GetDeliveredCost(x, nGas); break;
		default: Q_ASSERT(false);
		}
	}

	if (y == 0.0F)
		*X = y = -1.0F;
	else if (x == y)
		*Y = y = 1.0F;
	else if ((x>0.0F) != (y>0.0F))
		*Y = y = 0.0F;
	else
		*Y = y = x/y;

	if (y > MAX_OVLY_HEIGHT*2.0F)
		*X = y = -1.0F;

	if (m_bScanning)
	{
		if (y>=0.0F && m_fLast[ny]>=0.0F)
		{// if we have two consecutive values
			m_nCount++;
			if (fabs(y-m_fLast[ny])<m_fSlope)
				if (m_fLast[ny]>m_fMax)
					m_fMax = m_fLast[ny];
			if (y>m_fMax)
				m_fMax = y;
		}
		m_fLast[ny] = y;
		*X = -1.0F;
	}
	else if (y > m_fMax)
		*X = -1.0F;
}
