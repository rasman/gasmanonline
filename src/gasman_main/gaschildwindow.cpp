
#include "gaschildwindow.h"

#if QT_VERSION >=0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif
#include "gasapplication.h"
#include "gasdoc.h"
#include "gaspanelview.h"
#include "gasgraphview.h"
#include "gasmainwindow.h"
#include "gascontrol.h"

// GasChildWindow construction
GasChildWindow::GasChildWindow(  GasDoc * document, int nGas, GasViewType type, QWidget *parent )
	: QMainWindow( parent ), gaspanelview( 0 ),
		gasControlWidget(0), gasgraphview( 0 ), gasdoc( document ), state( Qt::WindowNoState ), viewType(type)
{
	setWindowIcon(QIcon( ":/images/gasman_sm.png" ));
	setAttribute( Qt::WA_DeleteOnClose );
	setFocusPolicy(Qt::StrongFocus);
	setObjectName("Child");
	 
	// Create Panel/Graph
	 
	gasgraphview = new GasGraphView( gasdoc, this );
	
	// Scroll area for GasGraphView

	saGraph = new QScrollArea;
	saGraph->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	saGraph->setWidget( gasgraphview );
	saGraph->setWidgetResizable( true );

	gaspanelview = new GasPanelView( gasdoc, this );

	// Scroll area for GasPanelView

	saPanel = new QScrollArea;
	saPanel->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	saPanel->setWidget( gaspanelview );
	saPanel->setWidgetResizable( true );

	initShow(nGas);
	switch(type)
	{
	case GRAPH: showGraphView(); break;
	case PANEL: showPanelView(); break;
	case CONTROL: showGasControl(); break;
	}
 
	gasdoc->addChildWindow(this);
	gasdoc->updateTitle();
}	

GasChildWindow::~GasChildWindow()
{
	gasdoc->removeChildWindow( this ); // Remove this child from the list of childs in the document
}


void GasChildWindow::closeEvent( QCloseEvent * evnt )
{
	if(gasControl() && !gasdoc->close())
		evnt->ignore();
	else{
		gaspanelview->StopTicker();
		emit beforeClose();
		evnt->accept();
	}
}


// Initializes the "all" view
void GasChildWindow::updateAllView(int nGas)
{
	gaspanelview->InitialUpdate(nGas);
	gasgraphview->InitialUpdate(nGas);
}

void GasChildWindow::showPanelView()
{
	setObjectName("Panel");
	setCentralWidget(saPanel);
	updateGeometry();
	 
	 
	emit viewsChanged();
}

void GasChildWindow::showGraphView()
{
	setObjectName("Graph");
	setCentralWidget(saGraph);
	updateGeometry();

	emit viewsChanged();
}

void GasChildWindow::setTitle( const QString &t, const QString& frameNum, const QString& priority, const QString& agent )
{
	m_title = t + ":";

	switch(viewType)
	{
	case GRAPH: m_title+=tr("Graph"); break;
	case PANEL: m_title+=tr("Picture"); break;
	case CONTROL: m_title+=tr("Control"); break;
	}

	m_title += frameNum;

	if((viewType != CONTROL) && (priority != ""))
		m_title += ":(" + priority + ":" + agent + ")";	

	setWindowTitle( m_title + "[*]" );
	emit titleUpdated(m_title);
}

void GasChildWindow::saveWindowState()
{
	state = windowState();
}

void GasChildWindow::restoreWindowState()
{
	setWindowState( state );
}

void GasChildWindow::initShow(int nGas)
{
	updateAllView(nGas);
	gasdoc->save_nActiveGas = NGS;
}

void GasChildWindow::showGasControl()
{


	setObjectName("Control");
	gasControlWidget = new GasControl(gasdoc, this);
	
	QScrollArea * saControl = new QScrollArea;
	saControl->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	saControl->setWidget( gasControlWidget );
 
	saControl->setWidgetResizable( true );


 
	setCentralWidget(saControl);
	updateGeometry();
	emit viewsChanged();
}

QString GasChildWindow::title()
{ 
	return m_title; 
}

GasViewType GasChildWindow::getViewType()
{
	return viewType;
}

// Tricky: by getting the color for the gas in the picture view, we get the
// primary color for the control and the agent color for the picture and graph
QString GasChildWindow::tabColor()
{
	int nGas;
	QList<QString>& colors = gasdoc->colors;
	return panelView()->tryGetGas(nGas) && nGas >=0 && nGas < colors.size() ? ToQColor(colors.at(nGas)) : QColor::colorNames().at(Qt::black);
}

bool GasChildWindow::tryGetGas(int& nGas)
{
	GasViewCommon* view;

	switch(viewType)
	{
	case GRAPH: view = graphView(); break;
	case PANEL: view = panelView(); break;
	case CONTROL: view = gasControlWidget; break;
	}
	return view->tryGetGas(nGas);
}

void GasChildWindow::resetGas(const int nGas)
{
	graphView()->resetGas(nGas);
	panelView()->resetGas(nGas);
}
