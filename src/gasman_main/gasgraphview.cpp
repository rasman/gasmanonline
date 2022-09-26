#include "gasgraphview.h"

#if QT_VERSION >=0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif
#include <QtSvg>
#include "gasapplication.h"
#include "gasmainwindow.h"
#include "gaschildwindow.h"
#include "gasdoc.h"
#include "gaspanelview.h"
#include "semaphoreholder.h"


int GasGraphView::m_nLineLabelType;
int GasGraphView::m_nLineWidth;
int GasGraphView::m_nScrollPercent;
bool GasGraphView::m_bDfltScrollEnb;				// from INI file
bool GasGraphView::m_bGraphpaper;
int GasGraphView::m_nDfltScaleMinutes;
QString GasGraphView::m_szScale = QObject::tr( "15 Min" );

GasGraphView::GasGraphView( GasDoc *doc, GasChildWindow * childWindow, QWidget *parent  )
	: GasViewCommon( childWindow, parent ), gasdoc( doc )
{
	 
	setupUi( this );
	m_childwindow = childWindow;
	short h = (short)(fontMetrics().height()*7/8);
	m_FGFGraph->SetFontHeight(h);
	m_DELGraph->SetFontHeight(h);
	m_IARMGraph->SetFontHeight(h);
	m_FGFGraph->setType(GRAPHTYPE_OTHER);
	m_DELGraph->setType(GRAPHTYPE_OTHER);
	m_IARMGraph->setType(GRAPHTYPE_IARM);
	m_FGFGraph->setReceiver( this );
	m_DELGraph->setReceiver( this );
	m_IARMGraph->setReceiver( this );

	m_sizeOrig = QSize( -1, -1 );					// No size yet

	m_nGas = -1;									// No previous agent
	m_strScale = m_szScale;
	m_nScaleMinute = m_nDfltScaleMinutes;			// Start with short view
	m_bScrollEnb = m_bDfltScrollEnb;				// And default autoscroll
	m_dwTempmark = 0;
	
	viewComboBox->addItems(listTime);

	//create connections
	connect( hsbGraph, SIGNAL( valueChanged( int ) ), this, SLOT( scrollMoved( int ) ) );
	 
 	connect(viewComboBox, SIGNAL(currentIndexChanged(const QString&)), 
 					this, SLOT(ChangeScale(const QString&)));
	
	m_bColl = false;
	CKTscroll->setVisible(false);
	connect(CKTscroll,SIGNAL(valueChanged(int)), m_IARMGraph, SLOT(scrollMoved(int )));
	
	connect(m_IARMGraph,SIGNAL(zoomChanged()), this, SLOT(onZoomChanged()));

	connect(checkBoxCKT,SIGNAL(stateChanged(int)),this,SLOT(onDrawCKT(int)));
	connect(checkBoxALV,SIGNAL(stateChanged(int)),this,SLOT(onDrawALV(int)));
	connect(checkBoxVRG,SIGNAL(stateChanged(int)),this,SLOT(onDrawVRG(int)));
	connect(checkBoxMUS,SIGNAL(stateChanged(int)),this,SLOT(onDrawMUS(int)));
	connect(checkBoxFAT,SIGNAL(stateChanged(int)),this,SLOT(onDrawFAT(int)));

	connect(bt_coll,SIGNAL(clicked()),this, SLOT(onCollapse()));

	bt_coll->setIconSize(QSize(15,15));
	bt_coll->setIcon(QIcon(":/images/collapse.png"));
	bt_coll->setToolTip(tr("Press \"H\" to hide/show this button \nHotkey to Collapse/Expand is \"C\""));
	bt_coll->setVisible(false);
	//QShortcut * collapse = new QShortcut(this);
	//collapse->setAutoRepeat(false);
	//collapse->setKey(tr("C"));
	//connect(collapse, SIGNAL(activated()), this , SLOT(onCollapse()));

	//QShortcut * hide = new QShortcut(this);
	//hide->setAutoRepeat(false);
	//hide->setKey(tr("H"));
	//connect(hide, SIGNAL(activated()), this , SLOT(hideCollapse()));
	setFocusPolicy(Qt::StrongFocus);
	m_scaleChangeSemaphore.release(1);
	m_bToImage = false;
}

bool GasGraphView::tryGetScaleString(const int scaleMinutes, QString& scaleString)
{
	bool ok = false;
	switch (scaleMinutes)
	{
	case 5: case 10: case 15: case 30: case 45:
		scaleString = QString( QObject::tr( "%1 Min" ) ).arg( scaleMinutes );
		ok = true;
		break;
	case 60: case 120: case 360: case 720:
		scaleString = QString( QObject::tr( "%1 Hr" ) ).arg( scaleMinutes / 60 );
		ok = true;
		break;
	case 1440:
		scaleString = QString( QObject::tr( "%1 Day" ) ).arg( scaleMinutes / (24*60) );
		ok = true;
		break;
	default:
		scaleString = QObject::tr( "15 Min" ); // Default result for round-trip
	}
	return ok;
}
 
bool GasGraphView::tryGetScaleMinutes(const QString& scaleString, int& scaleMinutes)
{
	bool ok = false;
	int pos = scaleString.indexOf( ' ' );
	if ( pos >= 0 ) {
		int nScale = scaleString.left( pos ).toInt();
		if ( (pos = scaleString.indexOf( QRegExp( QObject::tr( "[DHM]" ) ), pos ) ) != -1 ) {
			if ( scaleString.at(pos) == QObject::tr( "[DHM]" ).at(1) )
				nScale *= 24*60;
			else if ( scaleString.at(pos) == QObject::tr( "[DHM]" ).at(2) )
				nScale *= 60;
			switch (nScale)
			{
			case 5: case 10: case 15: case 30: case 45:
			case 60: case 120: case 360: case 720: case 1440:
				scaleMinutes = nScale;
				ok = true;
				break;
			default:
				scaleMinutes = 15; // Default result for round-trip
			}
		}
	}
	return ok;
}

void GasGraphView::ReadProfile()
{
	m_bGraphpaper = gasApp->ReadProfile( "Defaults", "GraphPaper", true ).toBool();
	m_bDfltScrollEnb = gasApp->ReadProfile( "Defaults", "Scroll", true ).toBool();
	m_nLineLabelType = gasApp->ReadProfile( "Defaults", "LineLabels", 1 ).toInt();
	m_nLineWidth = gasApp->ReadProfile( "Defaults", "LineWidth", 1 ).toInt();
	m_nScrollPercent = gasApp->ReadProfile( "Defaults", "ScrollPercent", 90 ).toInt();
	if (m_nScrollPercent > 100 || m_nScrollPercent < 25) m_nScrollPercent = 90;
	m_nDfltScaleMinutes = gasApp->ReadProfile( "Defaults", "GraphMinutes", 15 ).toInt();
	if ( !tryGetScaleString(m_nDfltScaleMinutes, m_szScale) )
	{
		bool ok = tryGetScaleMinutes(m_szScale, m_nDfltScaleMinutes);
		if (!ok)
			Q_ASSERT(ok);	// Round trip always works, right?
	}
}

//Initializes the graph view
void GasGraphView::InitialUpdate(int nGas)
{	
	m_nGas = nGas;
	m_strAgent = gasdoc->GetAgent(m_nGas)->m_strName;
	agent->setText("<b>"+m_strAgent+"</b>");
	m_fMAC = gasdoc->GetAgent(m_nGas)->m_fMAC;
	m_strCircuit = gasdoc->GetCircuit();

	// Initial updates during file opens get old view size
	QString szScale;
	if ( tryGetScaleString(gasdoc->save_nScaleMinutes, szScale) ) {
		m_nScaleMinute = gasdoc->save_nScaleMinutes;
		m_strScale = szScale;
	}
	viewComboBox->setCurrentIndex( viewComboBox->findText( m_strScale, Qt::MatchFixedString ) );

	int nMaxPos = (int)(gasdoc->GetTime()/6.0F);
	int nMinPos = qMin( m_nScaleMinute*10, nMaxPos );
	hsbGraph->setRange( nMinPos, nMaxPos );
	hsbGraph->setValue( nMaxPos );

	m_colorPaper = m_IARMGraph->GetPaperColor();
	m_FGFGraph->SetColor0( Qt::black );
	m_DELGraph->SetColor0( Qt::black );

	UpdateData( false );

	//Get the original size for scaling.
	m_sizeOrig = theChild->panelView()->size();

	// Set the state of the scrollbars (affects DrawGraphs())

	if ( gasdoc->GetState() != RUNNING_STATE )	// If we're not running...
		EnableScroll();							// ...user can scroll
	else
		DisableScroll();

	DrawGraphs();								//Label axes and fill in

	//Match the graph label colors to the corresponding graph color
	
	QPalette p = checkBoxCKT->palette();
	p.setColor( QPalette::WindowText, m_IARMGraph->GetColor4() );
	 
	checkBoxCKT->setPalette(p);

	p = checkBoxALV->palette();
	p.setColor( QPalette::WindowText, m_IARMGraph->GetColor3() );
	 
	checkBoxALV->setPalette( p );

	p = checkBoxVRG->palette();
	p.setColor( QPalette::WindowText, m_IARMGraph->GetColor2() );
	 
	checkBoxVRG->setPalette( p );

	p = checkBoxMUS->palette();
	p.setColor( QPalette::WindowText, m_IARMGraph->GetColor1() );
	 
	checkBoxMUS->setPalette( p );

	p = checkBoxFAT->palette();
	p.setColor( QPalette::WindowText, m_IARMGraph->GetColor0() );
	 
	checkBoxFAT->setPalette( p ); 
}

void GasGraphView::UpdateData( bool bSaveAndValidate )
{	
	if ( !bSaveAndValidate )
		DisplayExtraData();
}

void GasGraphView::DisplayExtraData()
{
	if ( m_strCircuit[0] == tr( "Open", "open circuit" ).at(0) ) {
		lFGF->setText( tr( "VA\nL/M" ) );
		lDEL->setText( tr( "CO\nL/M" ) );
	 
	}
	else {
		lFGF->setText( tr( "FGF\nL/M" ) );
		lDEL->setText( tr( "DEL\n%atm" ) );
	}
}

void GasGraphView::enableScrolling( bool set )
{
	UnsetTempmark();
	m_bScrollEnb = set;
	if ( gasdoc->GetState() == RUNNING_STATE )
	{
		SetGraphOrigin();
		SetTempmark();
		EnableAutoScroll( m_bScrollEnb );
	}
}

// Draw all graphs. (see note below).

void GasGraphView::DrawGraphs( bool needReapint )
{
	float fXVal;

	if ( m_sizeOrig.width() < 0 ) return;					// Not ready yet...

	if ( hsbGraph->isEnabled() )
		fXVal = (float)( hsbGraph->value() ) / 10.0F;
	else
		fXVal = gasdoc->GetTime() / 60.0F;

	//Set up Y axes

	GasPanelView *panel = theChild->panelView();

 

 	SetIARMmax( panel->highDELValue() );
 	SetDELmax(   panel->highDELValue() ); //...................................
	 
 	SetFGFmax( 	panel->highFGFValue() );
	
 	SetVAmax( panel->highVAValue() );
 	SetCOmax( panel->highCOValue() );

	//Set up X axes

	DrawGraph( m_FGFGraph, fXVal, m_strCircuit[0] == tr( "Open", "open circuit" ).at(0) ? 83:157, needReapint );
	DrawGraph( m_DELGraph, fXVal, m_strCircuit[0] == tr( "Open", "open circuit" ).at(0) ? 79:151, needReapint );
	DrawGraph( m_IARMGraph, fXVal, 89, needReapint );
}

// These routines are called by our friend, the Panel, when the user
// changes scales.

void GasGraphView::SetIARMmax( int nMax )
{
	SetGraphMax( m_IARMGraph, nMax );
}

void GasGraphView::SetDELmax( int nMax )
{
	 
	if ( m_strCircuit[0] != tr( "Open", "open circuit" ).at(0) )
		SetGraphMax( m_DELGraph, nMax );
}

void GasGraphView::SetFGFmax( int nMax )
{
	if ( m_strCircuit[0] != tr( "Open", "open circuit" ).at(0) ){
//		if(gasdoc->m_fFGFSetting ==0 )
			SetGraphMax( m_FGFGraph, nMax );
//		else
//			SetGraphMax(m_FGFGraph, gasdoc->m_fFGFSetting);
	}
}

void GasGraphView::SetCOmax( int nMax )
{
	if ( m_strCircuit[0] == tr( "Open", "open circuit" ).at(0) )
		SetGraphMax( m_DELGraph, nMax );
}

void GasGraphView::SetVAmax( int nMax )
{
	if ( m_strCircuit[0] == tr( "Open", "open circuit" ).at(0) )
		SetGraphMax( m_FGFGraph, nMax );
}

void GasGraphView::SetGraphMax( GasGraph *p, int nMax )
{
	 
	bool bWasLoading = (bool)p->GetLoading();
	float fYTick;

	if      (nMax >= 500) fYTick = 100.0F;
	else if (nMax >= 200) fYTick = 50.0F;
	else if (nMax >= 100) fYTick = 20.0F;
	else if (nMax >=  50) fYTick = 10.0F;
	else if (nMax >=  20) fYTick = 5.0F;
	else if (nMax >=  10) fYTick = 2.0F;
	else                  fYTick = 1.0F;

	p->SetLoading( true );
	p->SetYSize( (float)nMax );
 
	p->SetYLabelEvery( (float)nMax );
	p->SetYTickEvery( fYTick );
	p->SetLoading( bWasLoading );
}

void GasGraphView::DrawGraph( GasGraph* pCtl, float fXVal, int nPoints, bool needReapint )
{
	QColor color = m_bGraphpaper ? m_colorPaper : ( palette().color( QPalette::Window ) );
	bool bWasLoading = pCtl->GetLoading();
	float t=0.0F, l=0.0F;

	pCtl->SetLoading( true );
	 
	pCtl->SetXSize( (float)  m_nScaleMinute );
	if ( pCtl == m_IARMGraph ) {
		switch ( m_nScaleMinute)
		{
		case 5: t=1.0F; l=1.0F; break;
		case 10: t=1.0F; l=2.0F; break;
		case 15: t=1.0F; l=5.0F; break;
		case 30: t=5.0F; l=10.0F; break;
		case 45: t=5.0F; l=15.0F; break;
		case 60: t=10.0F; l=20.0F; break;
		case 120: t=10.0F; l=30.0F; break;
		case 360: t=30.0F; l=60.0F; break;
		case 720: t=60.0F; l=120.0F; break;
		case 1440: t=60.0F; l=240.0F; break;
		default: Q_ASSERT( false );
		}
		pCtl->SetXLabelEvery( l );
		pCtl->SetXTickEvery( t );
		pCtl->SetMac( m_fMAC );
		pCtl->SetLineLabelType( m_nLineLabelType );
	} else {
		pCtl->SetXLabelEvery( 0 );
		pCtl->SetXTickEvery( 0 );
		pCtl->SetMac( 0 );
	}
	pCtl->SetLineWidth( m_nLineWidth );
	pCtl->SetPaperColor( color );
	pCtl->SetXValEvery( (float)m_nScaleMinute / nPoints );
	pCtl->SetXVal( fXVal );
	pCtl->SetLoading( bWasLoading );
	if ( needReapint )
		pCtl->repaint();
}

void GasGraphView::ChangeScale( const QString &scale )
{
	SemaphoreHolder holder( &m_scaleChangeSemaphore );
	if ( !holder.IsAcquired() )
		return;				// exit on recursion

	int newScale;
	if ( !tryGetScaleMinutes( scale, newScale ) )
		Q_ASSERT( false );	// passed scale not valid

	// Round trip to guarantee valid scale
	QString formattedScale;
	tryGetScaleString( newScale, formattedScale );

	viewComboBox->setCurrentIndex( viewComboBox->findText( formattedScale, Qt::MatchFixedString ) );

	if ( newScale != m_nScaleMinute || formattedScale != m_strScale ) {
		m_nScaleMinute = newScale;
		m_strScale = formattedScale;

		// New default for this document
 		gasdoc->save_nScaleMinutes = (quint16)m_nScaleMinute;

		RescaleScroll();
		UnsetTempmark();
		SetGraphOrigin();
		SetTempmark();
		DrawGraphs();
	}
}

// From a user's point of view, enabling scrolling has a very different
// meaning.  It means "make the graph move so that the latest data is
// always visible."  This is a toggle which the user may set.  In response,
// we tell the graph widget to ignore x values not visible, manually
// position the graphs, and set a temporary bookmark when the graph hits
// the end.

void GasGraphView::EnableAutoScroll( bool bHow )
{
	m_FGFGraph->SetScrollEnb( bHow );
	m_DELGraph->SetScrollEnb( bHow );
	m_IARMGraph->SetScrollEnb( bHow );
}

// Enabling a scrollbar sets its range according to the graphing scale.
// Disabling a scrollbar invalidates its range.  The scrollbars should
// always reflect the state of the user interface -- that is, if the bar
// is enabled, then scrolling is valid (which should mean the sim is
// halted), and the graph should be set according to the scroller.
// SO ALWAYS SET CORRECT SCROLLER STATE BEFORE CALLING DrawGraph(S)

void GasGraphView::EnableScroll()
{
	//NOTE: scale is tenths of a minute
	int nMaxPos = (int)(gasdoc->GetTime() / 6.0F );
	 
	int nMinPos = qMin(m_nScaleMinute * 10 / m_IARMGraph->zoom, nMaxPos);
 
	hsbGraph->setVisible( nMaxPos != nMinPos );
	 
		hsbGraph->setRange( nMinPos, nMaxPos );
		hsbGraph->setValue( nMaxPos );
		hsbGraph->setEnabled( true );
 
	EnableAutoScroll( true );	//(or else the scroll bar is useless because
	// the graphs won't move.)
}

void GasGraphView::DisableScroll()
{                                        
	hsbGraph->setEnabled( false );
	EnableAutoScroll( m_bScrollEnb );	//(the user doesn't control the scrolling
	// while the sim is running.)
}

void GasGraphView::RescaleScroll()	// Redraw to scale, iff enabled
{ 
	//NOTE: scale is tenths of a minute
	int nMaxPos = (int)(gasdoc->GetTime() /6.0F );

	int nMinPos = qMin( m_nScaleMinute * 10 /m_IARMGraph->zoom, nMaxPos );
	int nOldPos = hsbGraph->value();


	hsbGraph->setRange( nMinPos, nMaxPos );
	if ( nOldPos < nMinPos )
		nOldPos = nMinPos;
	else if ( nOldPos > nMaxPos )
		nOldPos = nMaxPos;

	hsbGraph->setValue( nOldPos );

	EnableScroll();
}

// If scrolling IS disabled, then we stop when we fill up the graph.  Call
// SetGraphOrigin to make space on the graph to fill up if running and not
// scrolling.  Call Set/UnsetTempmark to place/remove a temporary bookmark
// at the end of the graph.

void GasGraphView::SetGraphOrigin()
{	
	float fStride, fOverlap, fOrig;

	UnsetTempmark();			// no temp bookmark yet

	if ( m_bScrollEnb || gasdoc->GetState() != RUNNING_STATE )
		return;	// graphs can't fill up if scrolling or not running

	// Compute the correct origin

	fStride = (float)floor((m_nScrollPercent*m_nScaleMinute+50)/100.0F);
	fOverlap = m_nScaleMinute - fStride;
	fOrig = (float)floor((gasdoc->GetTime()/60.0F-fOverlap)/fStride)*fStride;
	if (fOrig < 0.0F) fOrig = 0.0F;

	// If it's a new one, set it (redraws graph)

	if ( m_FGFGraph->GetOrigin() == fOrig ) return;

	m_FGFGraph->SetOrigin(fOrig);
	m_DELGraph->SetOrigin(fOrig);
	m_IARMGraph->SetOrigin(fOrig);
}

void GasGraphView::SetTempmark()
{
	Q_ASSERT( m_dwTempmark == 0L );	// Set twice in a row w/out Unset

	if ( m_bScrollEnb || gasdoc->GetState() != RUNNING_STATE )
		return;	// graphs can't fill up if scrolling or not running

	// Don't really do this if we are not visible
	if ( this->getChildWindow()->getViewType() != GRAPH )
		return;

	int i;
	if ( ( i = gasdoc->m_nTempmarks ) < MAX_BKMK ) {
		float fOrig = m_IARMGraph->GetOrigin();
		m_dwTempmark = (long)((fOrig+m_nScaleMinute)*60.0F)*1000L;
		gasdoc->m_dwTempmarks[i++] = m_dwTempmark;
		gasdoc->m_nTempmarks = i;
	}
}

void GasGraphView::UnsetTempmark()
{
	if ( m_dwTempmark == 0L )
		return;

	bool bFound = false;
	for ( int i=0; i < gasdoc->m_nTempmarks; ++i ) {
		if ( bFound )
			gasdoc->m_dwTempmarks[i-1] = gasdoc->m_dwTempmarks[i];
		else
			bFound = ( gasdoc->m_dwTempmarks[i]==m_dwTempmark );
	}
	if ( bFound )
		gasdoc->m_nTempmarks--;

	m_dwTempmark = 0L;
}

void GasGraphView::scrollMoved( int value )
{
	float fXVal = (float)value/10.0F;	// NOTE: Scale is tenths of minute
	
	m_FGFGraph->SetXVal( fXVal );
	m_DELGraph->SetXVal( fXVal );
	m_IARMGraph->SetXVal( fXVal );
}
 
void GasGraphView::customEvent( QEvent *evnt )
{
	if ( evnt->type() == (QEvent::Type)GasUpdateQEvent ) {
		
		GasUpdateEvent *gasevent = static_cast<GasUpdateEvent * >( evnt );
		if ( !gasevent ) return;
		
		GasDoc *pDoc = gasevent->document();
		if ( !pDoc ) return;

		int nGas = gasevent->gas();
		if ( nGas && (( nGas - 1 ) != m_nGas ) ) return;

		switch ( gasevent->type() )
		{
		case GasUpdateEvent::UPD_AGENT:
			m_strAgent = pDoc->GetAgent( m_nGas )->m_strName;
			m_fMAC = pDoc->GetAgent( m_nGas )->m_fMAC;
			agent->setText("<b>"+m_strAgent+"</b>");
			// Incase the panel hasn't gotten this yet...
			theChild->panelView()->setHighDELValue( pDoc->GetAgent( m_nGas )->m_wHighDEL );
			UpdateData( false );
			DrawGraphs();
			break;
		case GasUpdateEvent::UPD_CIRCUIT: {
				bool bWasOpen = m_strCircuit[0] == tr( "Open", "open circuit" ).at(0);
				m_strCircuit = pDoc->GetCircuit();
				if ( bWasOpen != ( m_strCircuit[0] == tr( "Open", "open circuit" ).at(0) ) ) {	//Damn!
					UpdateData( false );
					DrawGraphs();
				}
			}
			break;
		case GasUpdateEvent::UPD_SPEED:
			break;
		case GasUpdateEvent::UPD_DEL:
			break;
		case GasUpdateEvent::UPD_FGF:
			break;
		case GasUpdateEvent::UPD_VA:
			break;
		case GasUpdateEvent::UPD_CO:
			break;
		case GasUpdateEvent::UPD_WEIGHT:
			break;
		case GasUpdateEvent::UPD_TICK: {
				float fXVal = pDoc->GetTime()/60.0F;
				m_FGFGraph->SetXVal(fXVal);
				m_DELGraph->SetXVal(fXVal);
				m_IARMGraph->SetXVal(fXVal);
			}
			break;
		case GasUpdateEvent::UPD_STATE: {
				AState state = pDoc->GetState();
				switch ( state )
				{
				case STOPPED_STATE:
				case INITIAL_STATE:
				case END_STATE:
					EnableScroll();
					break;
				case RUNNING_STATE:
					DisableScroll();
					SetGraphOrigin();
					SetTempmark();
					break;
				default:
					Q_ASSERT( false );
				}
			}
			break;
		case GasUpdateEvent::UPD_NGAS:
			m_nGas = 0;
			break;

		case GasUpdateEvent::UPD_NONE: {
				int nMaxPos = (int)(pDoc->GetTime()/6.0F);
				int nMinPos = qMin( m_nScaleMinute*10, nMaxPos );
				hsbGraph->setRange( nMinPos, nMaxPos );
				hsbGraph->setValue( nMaxPos );

				m_strAgent = pDoc->GetAgent(m_nGas)->m_strName;	//Fetch data 
				agent->setText("<b>"+m_strAgent+"</b>");
				m_fMAC = pDoc->GetAgent(m_nGas)->m_fMAC;
				m_strCircuit = pDoc->GetCircuit();

				UpdateData( false );							//Update static controls
			 	DrawGraphs();
			}
			break;
		default:
			Q_ASSERT( false );	
		}
	}

	if ( evnt->type() == (QEvent::Type)(QEvent::User + 2) ) {
		GasGraphEvent *gasGraphEvent = static_cast<GasGraphEvent * >( evnt );
		if ( !gasGraphEvent ) return;
		
		float *pf = gasGraphEvent->data();
		GasGraph *receiver = gasGraphEvent->receiver();

		if ( receiver == m_FGFGraph )
			OnYValReqGraphFGF(pf, pf+1, pf+2);
		else if ( receiver == m_DELGraph )
			OnYValReqGraphDEL(pf, pf+1, pf+2);
		else if ( receiver == m_IARMGraph )
			OnYValReqGraphIARM(pf, pf+1, pf+2);
		else {
			*pf = -1.0F;
			Q_ASSERT( false );
		}
	}
}

void GasGraphView::OnYValReqGraphDEL(float * X, float * Y, float *lastX) 
{
	if ( m_bToImage ? m_bVaCo : m_strCircuit[0] == tr( "Open", "open circuit" ).at(0) )
		*Y = gasdoc->GetCO(*X);
	else
	{
		bool b;

		*Y = gasdoc->GetDEL(*X, m_nGas, &b, *lastX);
		if ( b )
			*X = -2.0F;
	}
}

void GasGraphView::OnYValReqGraphFGF(float * X, float * Y, float * /*lastX*/)
{
	if (  m_bToImage ? m_bVaCo : m_strCircuit[0] == tr( "Open", "open circuit" ).at(0) )
		*Y = gasdoc->GetVA(*X);
	else
		*Y = gasdoc->GetFGF(*X, m_nGas);
}

void GasGraphView::OnYValReqGraphIARM(float * X, float * Y, float * /*lastX*/)
{
	 
	switch ((int)*Y)	// which is the ordinal y1, y2, y3, or y4
	{
	case 4:
		*Y= gasdoc->GetCKT(*X, m_nGas);
		break;
	case 3:
		*Y = gasdoc->GetALV(*X, m_nGas);
		break;
	case 2:
		*Y = gasdoc->GetVRG(*X, m_nGas);
		break;
	case 1:
		*Y = gasdoc->GetMUS(*X, m_nGas);
		break;
	case 0:
		*Y = gasdoc->GetFAT(*X, m_nGas);
		break;
	default:
		Q_ASSERT( false );
	}
}


QByteArray GasGraphView::toArray()
{
	QByteArray ret;
	QBuffer buf(&ret);
	buf.open(QIODevice::WriteOnly);
	toImage().save(&buf, "PNG");
	
	return ret.toBase64();
}

class FlagHolder
{
public:
	FlagHolder(bool *ps) : m_pFlag(ps) { *ps = true; }
	~FlagHolder() { *m_pFlag = false; }
private:
	bool *m_pFlag;
};

QImage GasGraphView::toImage()
{
	FlagHolder fh(&m_bToImage);

	QPrinter *prn = gasMainWindow->printer();
	if ( !prn ) return QImage();

	QSize page = prn->pageRect().size();
	page.setWidth( page.width() * logicalDpiX() / prn->logicalDpiX() );
	page.setHeight( page.height() * logicalDpiY() / prn->logicalDpiY() );
	
	QImage image( page.width(), page.height() - 10, QImage::Format_RGB32  );
	image.fill( qRgb( 255, 255, 255 ) );

	m_FGFGraph->SetLoading( true );
	m_DELGraph->SetLoading( true );
	m_IARMGraph->SetLoading( true );

	float fXValOld = m_IARMGraph->GetXVal();
	float fHighTime = gasdoc->GetHighTime()/60.0F;

	if ( fXValOld < (float)m_nScaleMinute && fXValOld < fHighTime ) {
		if ( fHighTime > (float)m_nScaleMinute )
			fHighTime = (float)m_nScaleMinute;
		m_FGFGraph->SetXVal( fHighTime );
		m_DELGraph->SetXVal( fHighTime );
		m_IARMGraph->SetXVal( fHighTime );
	}else
		fHighTime = fXValOld;

	enum { PG_VA=-32768, PG_CO, PG_FGF, PG_DEL };
	QString szLabel[][6] = { { tr( "VA" ), tr( "CO" ), tr( "FGF" ), tr( "DEL" ), tr( "CKT" ), tr( "CKT" ) },
		{ QString(), QString(),  QString(),  QString(), tr( "ALV" ), tr( "ALV" ) },
		{ QString(), QString(),  QString(),  QString(), tr( "VRG" ), tr( "VRG" ) },
		{ QString(), QString(),  QString(),  QString(),  QString(), tr( "MUS " ) },
		{ QString(), QString(),  QString(),  QString(),  QString(), tr( "FAT" ) } };
	GasGraph  *pctl[] = { m_FGFGraph,  m_DELGraph,  m_FGFGraph, m_DELGraph, m_IARMGraph, m_IARMGraph };
	int nVal[] = { PG_VA, PG_CO, PG_FGF, PG_DEL, 0x1C, 0x1F };
	bool bVaCo[] = { true, true, false, false, false, false };
	int nPos[] ={ 13, 26, 39, 52, 76, 99 };
	int nLines[] = {1,1,1,1,3,5};
	float fXTickEvery = m_IARMGraph->GetXTickEvery();
	float fXLabelEvery = m_IARMGraph->GetXLabelEvery();
	float fXValEveryOld;
	int nTop = 0;
	int nBottom;
	int nLeft = ( page.width() * 14 ) / 100;
	int cxWidth = ( ( page.width() - nLeft ) * 9 ) / 10;
	GasPanelView *panel = theChild->panelView();

	QFont logFont;
	logFont.setPixelSize( (int)(page.height()/70) );
	logFont.setFamily( "Times" );
	int nFontOffset = logFont.pixelSize()/2;

	int nLast = int(sizeof(nPos)/sizeof(nPos[0]));
	for( int i = 0; i< nLast; ++i, nTop = nBottom ) {
		GasGraph *p = pctl[i];

		nBottom = (int)( page.height() * nPos[i] /100 );

		int j = nLeft/8;
		int k = (nTop+nBottom)/2-nFontOffset*nLines[i];

		QPainter painter;
		painter.begin(&image);
		painter.setFont(logFont);
		painter.setRenderHint(QPainter::Antialiasing);
		for (int l = 0; l < nLines[i]; l++)
			painter.drawText(j, k + l * 2 * nFontOffset, szLabel[l][i]);
		painter.end();

		int cxPhySize = (int)p->GetXPhySize();
		int cyPhySize = (int)p->GetYPhySize();
		int cLMarginPct = (int)p->GetLMarginPct();
		p->SetLMarginPct(10);
		int cFontHeight = (int)p->GetFontHeight();
		p->SetFontHeight(logFont.pixelSize());
		float fXTickEverySave = 0.0F;
		float fXLabelEverySave = 0.0F;

		if (i < nLast-1)
		{
			fXTickEverySave = p->GetXTickEvery();
			fXLabelEverySave= p->GetXLabelEvery();
			p->SetXLabelEvery(fXLabelEvery);
			p->SetXTickEvery(fXTickEvery);
		}

		p->SetXPhyZero(nLeft);
		p->SetYPhyZero(nTop);
		p->SetXPhySize(cxWidth );
		p->SetYPhySize(nBottom-nTop);

		switch (nVal[i])
		{
		case PG_VA:
			SetGraphMax(m_DELGraph, panel->highVAValue() );
			break;
		case PG_CO:
			SetGraphMax(m_FGFGraph, panel->highCOValue() );
			break;
		case PG_FGF:
			SetGraphMax(m_FGFGraph, panel->highFGFValue() );
			break;
		case PG_DEL:
			SetGraphMax(m_DELGraph, panel->highDELValue() );
			break;
		default:
			p->SetMask((quint16)(-1)-nVal[i]);
		}

		fXValEveryOld = p->GetXValEvery();
		p->SetXValEvery(m_nScaleMinute/151.0F);

		m_bVaCo = bVaCo[i];
		p->renderToImage(&image);

#ifndef Q_OS_WIN
		QCoreApplication::flush();
#endif

		// resetting widgets back to what the graph should show
		p->SetXValEvery(fXValEveryOld);
		p->SetMask(0);

		p->SetYPhySize(cyPhySize);
		p->SetXPhySize(cxPhySize);
		p->SetLMarginPct(cLMarginPct);
		p->SetFontHeight(cFontHeight);

		p->SetYPhyZero(0);
		p->SetXPhyZero(0);

		if (i < nLast-1)
		{
			p->SetXTickEvery(fXTickEverySave);
			p->SetXLabelEvery(fXLabelEverySave);
		}

	}

	if ( fHighTime != fXValOld ) {
		m_FGFGraph->SetXVal( fXValOld );
		m_DELGraph->SetXVal( fXValOld );
		m_IARMGraph->SetXVal( fXValOld );
	}	

	SetDELmax( panel->highDELValue() );
	SetFGFmax( panel->highFGFValue() );
	SetVAmax( panel->highVAValue() );
	SetCOmax( panel->highCOValue() );	

	m_FGFGraph->SetLoading( false );
	m_DELGraph->SetLoading( false );
	m_IARMGraph->SetLoading( false );

	return image;

}

QSize GasGraphView::minimumSizeHint() const
{
 
	return QSize(395,320) ;
 
}

QSize GasGraphView::sizeHint() const
{
	return minimumSizeHint();
}

void GasGraphView::onZoomChanged()
{
	int zoom = m_IARMGraph->getZoom();
	m_DELGraph->setZoom(zoom);
	m_FGFGraph->setZoom(zoom);
	if(zoom == 1)
	{
	 
		CKTscroll->setVisible(false);
		CKTscroll->setValue(0);
	}else
	{
	 
	 
		CKTscroll->setVisible(true);
		
		//TODO: set scrollbar range and tick	
		setYScrollBar();
		

	}
	

	RescaleScroll();
	
}
void GasGraphView::setYScrollBar()
{
	int old = CKTscroll->value();
	float range = m_IARMGraph->GetYSize() * m_IARMGraph->zoom *10/(m_IARMGraph->zoom+1) ;
	CKTscroll->setRange(0,range);
	CKTscroll->setValue(old);
	 
	 
}

void GasGraphView::onDrawFAT(int state)  // mask to hide: 0x0001
{
	short mask = m_IARMGraph->GetMask();
	if(state == Qt::Checked)
		mask = mask & 0xFFFE;
	else
		mask = mask | 0x0001;
	m_IARMGraph->SetMask(mask);
	m_IARMGraph->update();
}
void GasGraphView::onDrawMUS(int state)// 0x0002
{
	short mask = m_IARMGraph->GetMask();
	if(state == Qt::Checked)
		mask = mask & 0xFFFD;
	else
		mask = mask | 0x0002;
	m_IARMGraph->SetMask(mask);
	m_IARMGraph->update();
	
}
void GasGraphView::onDrawVRG(int state) //0x0004
{
	short mask = m_IARMGraph->GetMask();
	if(state == Qt::Checked)
		mask = mask & 0xFFFB;
	else
		mask = mask | 0x0004;
	m_IARMGraph->SetMask(mask);
	m_IARMGraph->update();
}
void GasGraphView::onDrawALV(int state) //0x0008
{
	short mask = m_IARMGraph->GetMask();
	if(state == Qt::Checked)
		mask = mask & 0xFFF7;
	else
		mask = mask | 0x0008;
	m_IARMGraph->SetMask(mask);
	m_IARMGraph->update();
}
void GasGraphView::onDrawCKT(int state)//0x0010
{
	short mask = m_IARMGraph->GetMask();
	if(state == Qt::Checked)
		mask = mask & 0xFFEF;
	else
		mask = mask | 0x0010;
	m_IARMGraph->SetMask(mask);
	m_IARMGraph->update();
}
void GasGraphView::wheelEvent(QWheelEvent * event)
{
	int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
	if(CKTscroll->isVisible() || hsbGraph->isVisible()){
	if(event->orientation() ==Qt::Vertical ){
		if( CKTscroll->isVisible()){
			CKTscroll->setValue(CKTscroll->value()+numSteps);
			
		}else if(hsbGraph->isVisible()){
			numSteps *=10;
			hsbGraph->setValue(hsbGraph->value()+numSteps);
		
		}
	}else if(event->orientation() ==Qt::Horizontal){
		if(hsbGraph->isVisible()){
			hsbGraph->setValue(hsbGraph->value()+numSteps);
		
		}
	}
	}else{
		event->ignore();
	}


}

int GasGraphView::getZoom()
{
	return m_IARMGraph->getZoom();
}
void GasGraphView::setZoom(int z)
{
	m_IARMGraph->setZoom(z);	
}
void GasGraphView::onCollapse()
{	
	 
	// collapse the upper two views if !m_bColl;
	//display if m_bColl;
	lFGF->setVisible(m_bColl);
	m_FGFGraph->setVisible(m_bColl);
	lDEL->setVisible(m_bColl);
	m_DELGraph->setVisible(m_bColl);

	m_bColl = !m_bColl;
	if(m_bColl)
		bt_coll->setIcon(QIcon(":/images/expand.png"));
	else
		bt_coll->setIcon(QIcon(":/images/collapse.png"));

}
void GasGraphView::hideCollapse()
{
	bt_coll->setVisible(!bt_coll->isVisible());
}

void GasGraphView::keyPressEvent(QKeyEvent * e)
{
	if(e->key() == Qt::Key_Escape){
	 	m_IARMGraph->line = false;
		update();
	}
}
