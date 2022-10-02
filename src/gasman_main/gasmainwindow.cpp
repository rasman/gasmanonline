#include "gasmainwindow.h"
#if QT_VERSION >=0x050000
#include <QMessageBox>
#endif
#include <QtDebug>
#include <QWidget>
#include <math.h>
//#include <assistant.h>
#include "gasapplication.h"
#include "gaschildwindow.h"
#include "gasdoc.h"
#include "gasoverlay.h"
#include "gaspanelview.h"
#include "gasgraphview.h"
#include "gasgraph.h"
#include "gasoptionsdlg.h"
#include "gasprintseldlg.h"
#include "gasaboutdlg.h"
#include "gascontrol.h"
#include "gasregisterdlg.h"
#include "gaslicensemanager.h"
#include "gastabwidget.h"
#include "gasviewcommon.h"

#include "gasprintpreview.h" 

GasMainWindow *gasMainWindow = 0;

int linesCount = 0;


GasMainWindow::GasMainWindow()
	: QMainWindow(0), m_bPrintout(true), m_bPrintGraphs(true),
	mdi(new QMdiArea(this)), m_bPositioned(false)
{
	mdi->installEventFilter(this);
	mdi->setAcceptDrops(true);
	setWindowTitle(tr("Gas Man\xC2\xAE "));
#ifdef Q_OS_MACX 
	setIconSize(QSize(32, 32)); //Toolbar icons are larger on the Mac
#else
	setIconSize(QSize(22, 22));
#endif

	gasPrintDocument = 0;
	//assistant = new Assistant();
	gasMainWindow = this;

	setCentralWidget(mdi);

	createActions();		//Create all actions
	createMenus();			//Create a Main Menu
	createToolBar();		//Create a Tool Bar

	//Create a printer with high resolution and set the page size
	prn = new QPrinter(QPrinter::HighResolution);

	connect(mdi, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(subWindowActivated(QMdiSubWindow *)));
	connect(glm->instance(), SIGNAL(validLicenseRegistered()), this, SLOT(updateGasDocs()));
	connect(mdi, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(windowStateChanged()));
	mdi->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdi->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	openDir = gasApp->ReadProfile("Defaults", "OpenDir", "").toString();
	if (!openDir.isEmpty() && !QFile::exists(openDir))
		openDir.clear();

	saveDir = gasApp->ReadProfile("Defaults", "SaveDir", "").toString();
	if (!saveDir.isEmpty() && !QFile::exists(saveDir))
		saveDir.clear();

	transDir = gasApp->ReadProfile("Defaults", "TransDir", "").toString();
	if (!transDir.isEmpty() && !QFile::exists(transDir))
		transDir.clear();
}

GasMainWindow::~GasMainWindow()
{
	//Delete printer
	delete prn;
}

void GasMainWindow::showEvent(QShowEvent *evnt)
{
	QMainWindow::showEvent(evnt);

	if (m_bPositioned)
		return;

	m_bPositioned = true;

	QSettings settings("Med Man", "Gas Man");
	QRect geometry = settings.value("MainWindow/geometry").toRect();
	QRect screen = gasApp->primaryScreen()->availableGeometry();

	if (screen.contains(geometry.topLeft()) && screen.contains(geometry.bottomRight()))
		setGeometry(geometry);

	bool max = settings.value("MainWindow/max").toBool();
	if (max)
		this->showMaximized();
}

void GasMainWindow::closeEvent(QCloseEvent *evnt)
{
	foreach(GasDoc *doc, docs) {
		if (!doc->close()) {
			evnt->ignore();
			return;
		}
	}

	//delete assistant;
	//assistant = NULL;

	QSettings settings("Med Man", "Gas Man");
	settings.setValue("MainWindow/max", isMaximized());
	if (!isMaximized() && !isFullScreen() && !isMinimized())
		settings.setValue("MainWindow/geometry", m_geometry);

	evnt->accept();
}

void GasMainWindow::resizeEvent(QResizeEvent *evnt)
{
	QMainWindow::resizeEvent(evnt);

	if (!isMaximized() && !isFullScreen() && !isMinimized())
		m_geometry = geometry();
}

void GasMainWindow::moveEvent(QMoveEvent *evnt)
{
	QMainWindow::moveEvent(evnt);

	if (!isMaximized() && !isFullScreen() && !isMinimized())
		m_geometry = geometry();
}

bool GasMainWindow::event(QEvent *evnt)
{
	if (evnt->type() == QEvent::WindowStateChange) {
		bool minimized = windowState() & Qt::WindowMinimized;
		foreach(GasChildWindow* current, allChildren())
		{
			if (minimized) {
				current->saveWindowState();
				current->hide();
			}
			else {
				current->restoreWindowState();
				current->show();
			}
		}
		//return true;
	}
	return QMainWindow::event(evnt);
}

QMenu* GasMainWindow::createPopupMenu()
{
	QMenu* menu = new QMenu;
	menu->addAction(showToolbarAction);

	return menu;
}

bool GasMainWindow::dropPosAcceptable(QDropEvent* event)
{
	bool ok = true;
	foreach(QMdiSubWindow* subWin, mdi->subWindowList())
	{
		if (subWin->frameGeometry().contains(event->pos()))
			ok = false;
	}
	return ok;
}

void GasMainWindow::reparentChildDrop(QDropEvent* event, GasTabWidget* dest)
{
	GasChildWindow* child = qobject_cast<GasChildWindow*>(gasApp->popGasTab());
	GasTabWidget *source = qobject_cast<GasTabWidget*>(event->source());

	reparentChild(child, source, dest, event->pos());
}

void GasMainWindow::reparentChild(GasChildWindow *child, GasTabWidget *source, GasTabWidget *dest, const QPoint& point)
{
	// remove tab
	source->removeTab(source->indexOf(child));

	if (
		   !dest
		|| (dest->widget(0) != NULL && child->doc() != qobject_cast<GasChildWindow*>(dest->widget(0))->doc())
	)
	{/*drop to blank workspace or tab containing different doc => new window */
		dest = newTabWidget(child->doc());
		QMdiSubWindow* subWin = mdi->addSubWindow(dest);
		dest->addTab(child, child->title());
		if (!point.isNull())
			subWin->move(point);
	} else {
		dest->setUpdatesEnabled(false);
		dest->addTab(child, child->title());
		dest->setCurrentWidget(child);
		dest->setUpdatesEnabled(true);
	}

	dest->show();
	setActiveWindow(dest);
}

bool GasMainWindow::eventFilter(QObject *obj, QEvent *evnt)
{
	// drag and drop handling for mdi
	if (obj == mdi)
	{
		// next two if blocks allow the mdi to accept drops of a certain type
		// and only if the drag is *outside* of all mdi sub windows
		if (evnt->type() == QEvent::DragEnter)
		{
			QDragEnterEvent* event = static_cast<QDragEnterEvent*>(evnt);
			if (gasApp->IsGoodDrop(event))
				event->acceptProposedAction();
			return true;
		}
		if (evnt->type() == QEvent::DragMove)
		{
			QDragMoveEvent* event = static_cast<QDragMoveEvent*>(evnt);
			if (gasApp->IsGoodDrop(event) && dropPosAcceptable(event))
				event->acceptProposedAction();
			else
				event->ignore();
			return true;
		}

		// drag object should always be the active child
		if (evnt->type() == QEvent::Drop)
		{
			QDropEvent* event = static_cast<QDropEvent*>(evnt);
			if (gasApp->IsGoodDrop(event) && dropPosAcceptable(event))
			{
				reparentChildDrop(event);
				event->acceptProposedAction();
			}
			return true;
		}
	}
	if (obj == toolBar) {
		if (evnt->type() == QEvent::ToolTip) {
			//Show tool tip for tool bar actions
			QHelpEvent *helpEvent = static_cast<QHelpEvent *>(evnt);
			QAction *act = toolBar->actionAt(helpEvent->pos());

			if (act)
				QToolTip::showText(helpEvent->globalPos(), act->toolTip());
			else
				QToolTip::showText(helpEvent->globalPos(), QString());
			return true;
		}
		return false;
	}
	GasChildWindow *child = qobject_cast<GasChildWindow *>(obj);
	if (child && allChildren().contains(child)) {
		if (evnt->type() == QEvent::WindowActivate) {
			int nGas;
			if (child->tryGetGas(nGas))
				child->doc()->m_nActiveGas = nGas;
			raise();
			updateAllActions();
			return true;
		}
		if (evnt->type() == QEvent::WindowStateChange) {

			overlayAction->setEnabled(largestVisibleGraphic() != 0);
			return true;
		}
		if (evnt->type() == QEvent::ModifiedChange) {
			updateTitle();
			return true;
		}
		return false;
	}
	return QMainWindow::eventFilter(obj, evnt);
}

void GasMainWindow::subWindowActivated(QMdiSubWindow * /*sw*/)
{
	updateAllActions();
}

//Return a pointer to the active child window or 0 if no windows active
GasChildWindow *GasMainWindow::activeChild()
{
	QMdiSubWindow* subWin = mdi->activeSubWindow();
	return gasChildOfMdiSubWindow(subWin);
}

GasChildWindow *GasMainWindow::gasChildOfMdiSubWindow(QMdiSubWindow* sw)
{
	if (!sw)
		return 0;
	GasTabWidget* tabWidget = qobject_cast<GasTabWidget*>(sw->widget());
	if (!tabWidget)
		return 0;
	QWidget* currentWidget = tabWidget->currentWidget();
	if (!currentWidget)
		return 0;
	GasChildWindow* childWindow = qobject_cast<GasChildWindow*>(currentWidget);
	Q_ASSERT(childWindow);
	bool markedForDeath = childWindow->panelView()->markedForDeath();
	Q_ASSERT(!markedForDeath);
	if (markedForDeath)
		return 0;
	return childWindow;
}

// Return a pointer to the active child window (there must be an active
// window or error will occur. This is for functionality that should only
// be available to a user when an active child window exists.
GasChildWindow *GasMainWindow::activeChildForced()
{
	// Should be the current active one
	GasChildWindow* childWindow = activeChild();
	if (childWindow)
		return childWindow;

	// We should never be here, but OK... try to see if we didn't
	// wait long enough
	qApp->processEvents(); // make sure we have an active child
	childWindow = activeChild();
	if (childWindow)
		return childWindow;

	// REALLY should never be here, but OK... return the last one
	// we knew was active
	QMdiSubWindow* last = mdi->subWindowList(QMdiArea::ActivationHistoryOrder).last();
	childWindow = gasChildOfMdiSubWindow(last);
	Q_ASSERT(childWindow);
	return childWindow;
}

GasChildWindow* GasMainWindow::largestVisibleGraphic()
{
	GasChildWindow* ret = activeChild();
	if (ret == 0 || (ret->getViewType() == GRAPH && !mdi->activeSubWindow()->isMinimized()))
		return ret;

	ret = 0;
	int retSize = 0;
	foreach(QMdiSubWindow* w, mdi->subWindowList()) {
		if (w->isMinimized())
			continue;

		GasTabWidget * tab = qobject_cast<GasTabWidget*>(w->widget());
		for (int i = 0; i < tab->count(); i++) {
			GasChildWindow* cw = qobject_cast<GasChildWindow*>(tab->widget(i));
			if (cw->getViewType() == GRAPH) {
				int s = cw->graphView()->IARMGraph()->GetXSize();
				if (s > retSize) {
					retSize = s;
					ret = cw;
				}
			}
		}
	}

	return ret;
}

//Create a new child window
GasChildWindow *GasMainWindow::createChild(GasDoc * gasdoc, int nGas, GasViewType type)
{
	GasChildWindow *child = new GasChildWindow(gasdoc, nGas, type);
	//Create connections
	connect(gasdoc, SIGNAL(modified(bool)), child, SLOT(setWindowModified(bool)));
	connect(child, SIGNAL(destroyed(QObject *)), this, SLOT(removeChild(QObject *)));
	connect(child, SIGNAL(viewsChanged()), this, SLOT(updateViewMenu()));
	connect(child, SIGNAL(viewsChanged()), this, SLOT(updateWindowMenu()));
	child->installEventFilter(this);

	return child;
}

void GasMainWindow::newControlDock(GasDoc* gasdoc)
{
	GasChildWindow* child = new GasChildWindow(gasdoc, 0, CONTROL);
	GasControl* control = child->gasControl();
	control->addAction(newGraphViewAction);
	control->addAction(newPanelViewAction);

	connect(gasdoc, SIGNAL(modified(bool)), child, SLOT(setWindowModified(bool)));
	connect(control, SIGNAL(addAgentView(GasDoc*, int, GasViewType)),
		this, SLOT(newView(GasDoc*, int, GasViewType)));
	connect(control, SIGNAL(removeChildWindow(QObject*)),
		this, SLOT(removeChild(QObject*)));
	connect(gasdoc, SIGNAL(destroyed()), child, SLOT(close()));
	connect(child, SIGNAL(destroyed(QObject *)), this, SLOT(removeChild(QObject *)));
	connect(control, SIGNAL(activateChild(QWidget*)),
		this, SLOT(setActiveWindow(QWidget*)));
	tabifyChild(child);
	control->initialAgentsUpdate();
}

GasChildren GasMainWindow::allChildren()
{
	GasChildren tmp;
	foreach(GasDoc* doc, docs)
	{
		tmp += doc->gasChildren();
	}
	return tmp;
}

//Remove child from the list of childs
void GasMainWindow::removeChild(QObject *obj)
{
	GasChildWindow *child = static_cast<GasChildWindow *>(obj);
	if (!child) return;
	child->close();
	updateAllActions();
	shootscreen = QPixmap();
}

//Remove document from the list of documents
void GasMainWindow::removeDoc(QObject *obj)
{
	GasDoc *doc = static_cast<GasDoc *>(obj);
	if (!doc) return;
	docs.removeAll(doc);
}

//Make the "child" the active window
void GasMainWindow::setActiveWindow(QWidget *window)
{
	if (!window) return;
	// find sub window containing this widget and activate it
	foreach(QMdiSubWindow* subWin, mdi->subWindowList())
	{
		if (subWin->widget() == window)
		{
			mdi->setActiveSubWindow(subWin);
		}
		GasTabWidget* tabWidget = qobject_cast<GasTabWidget*>(subWin->widget());
		if (tabWidget && (tabWidget->indexOf(window) != -1))
		{
			tabWidget->setCurrentWidget(window);
			if (subWin->isMinimized())
				subWin->setWindowState(subWin->windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
			mdi->setActiveSubWindow(subWin);
		}
	}
}

// set active window if necessary: this is for the case when 
// a dock widget is clicked on but does not need to activate 
// a sub window since one of it's sub windows is already active
void GasMainWindow::setActiveWindowIfNec(QWidget* window)
{
	if (!window) return;

	if (!activeChild() || !activeChildForced()->doc()->gasChildren().contains(qobject_cast<GasChildWindow*>(window)))
		setActiveWindow(window);
}

//Save the current simulation to a file
void GasMainWindow::saveFile()
{
	activeChildForced()->doc()->save();
}

//Save the current simulation to a file
void GasMainWindow::saveFileAs()
{
	activeChildForced()->doc()->saveAs();
}

bool GasMainWindow::checkCountSimulations()
{
	if (docs.size() < MaxSimulations || MaxSimulations == -1) return true;

	QMessageBox::information(this, tr("Gas Man\xC2\xAE "),
		tr("Cannot create another simulation.  Preview mode only allows %1 simultaneous simulations.").arg(MaxSimulations));
	return false;
}

//Create a new simulation
bool GasMainWindow::newFile() {
	//	if(!glm->instance()->validLicenseExists())
	//		if ( !checkCountSimulations() ) return false;
	setUpdatesEnabled(false);
	GasDoc *gasdoc = newDoc();						//Create a new document
	if (!gasdoc->m_anesArray.size())
	{
		setUpdatesEnabled(true);
		return false;
	}
	newControlDock(gasdoc);
	updateTitle();
	updateViewMenu();
	updateWindowMenu();
	setUpdatesEnabled(true);

	return true;
}

//Open a *.gas file by this name
void GasMainWindow::open(const QString &fileName)
{
	//if(!glm->instance()->validLicenseExists())
	//	if ( !checkCountSimulations() ) return;

	if (fileName.isEmpty())
		return;

	foreach(GasDoc *doc, docs) {
		if (doc->currentFile() == fileName) {
			//If the simulation is open, show it
			GasChildWindow *existing = doc->gasChildren()[0];
			existing->show();
			existing->raise();
			existing->activateWindow();
			return;
		}
	}

	GasDoc *gasdoc = newDoc();
	if (!gasdoc->loadFile(fileName)) {
		delete gasdoc;
		return;
	}

	QFileInfo fi(fileName);
	setOpenDir(fi.path());

	newControlDock(gasdoc);
	updateTitle();
	updateViewMenu();
	//setUpdatesEnabled(true);
}

//Create a new document
GasDoc* GasMainWindow::newDoc()
{
	GasDoc *gasdoc = new GasDoc(this);
	//Create connections
	connect(gasdoc, SIGNAL(destroyed(QObject *)), this, SLOT(removeDoc(QObject *)));
	docs.append(gasdoc);
	gasdoc->newDocument();
	return gasdoc;
}

//Open an existing simulation
void GasMainWindow::openFile()
{
#ifdef Q_OS_MACX 
	QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a file to open"),
		OpenDir(), tr("Gas Man\xC2\xAE Files (*.gas )"), NULL, QFileDialog::DontUseNativeDialog);
#else
	QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a file to open"),
		OpenDir(), tr("Gas Man\xC2\xAE Files (*.gas )"));
#endif

	open(fileName);
}

//Close the current simulation
void GasMainWindow::closeFile()
{
	activeChildForced()->doc()->close();
}

//Close all simulations
void GasMainWindow::closeAllFiles()
{
	foreach(GasDoc *doc, docs) {
		if (!doc->close()) {
			return;
		}
	}
}

//Sends a message with the document as an attachment
void GasMainWindow::sendMail()
{
	activeChildForced()->doc()->send();
}

//Show the "About GasMan" dialog
void GasMainWindow::aboutGasMan()
{
	GasAboutDlg *dlg = new GasAboutDlg(this);
	dlg->exec();
}

//Start a current simulation
void GasMainWindow::runSimulation()
{
	activeChildForced()->panelView()->run();
}

//Start all simulations
void GasMainWindow::runAllSimulation()
{
	bool haveRunners = CountRunners() != 0;
	foreach(GasChildWindow *child, allChildren()) {
		if (haveRunners && (child->doc()->GetState() == RUNNING_STATE))
			child->panelView()->stop();
		if (!haveRunners && (child->doc()->GetState() == STOPPED_STATE || child->doc()->GetState() == INITIAL_STATE))
			child->panelView()->start();
	}
}

void GasMainWindow::liquidInject()
{
	activeChildForced()->panelView()->Srynge();
}

void GasMainWindow::rewind()
{
	GasDoc *pDoc = activeChildForced()->doc();
	pDoc->rewind();
	updateAllActions();
}

void GasMainWindow::rewindAllSimulations()
{
	if (CountRewindable() == 0)
		return;

	foreach(GasChildWindow *child, allChildren()) {
		GasDoc *pDoc = child->doc();
		if (pDoc->GetState() == RUNNING_STATE)
			child->panelView()->stop();
		if (pDoc->GetState() != INITIAL_STATE)
			pDoc->rewind();
	}
	updateAllActions();
}

void GasMainWindow::fastFwd()
{
	activeChildForced()->doc()->fastFwd();
	updateAllActions();
}

void GasMainWindow::zeroTimer()
{
	activeChildForced()->doc()->zeroTimer();
	updateAllActions();
}

void GasMainWindow::clearAll()
{
	activeChildForced()->doc()->clearAll();
	updateAllActions();
}

//Copy data to clipboard
void GasMainWindow::copyData()
{
	activeChildForced()->doc()->copyData();
}

//Creates a pixmap constructed by grabbing the
//contents of the current child window
void GasMainWindow::selectAll()
{
	QPixmap child(QPixmap::grabWidget(activeChildForced()));

	shootscreen = child;
	updateEditMenu();
}

//Copy a screen shoot of the current child window to the clipboard
void GasMainWindow::copySelection()
{
	if (shootscreen.isNull()) return;
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setPixmap(shootscreen);
}

void GasMainWindow::defaultAgentColors()
{
	activeChildForced()->doc()->defaultAgentColors();
}

//Show $ or L as result
void GasMainWindow::showCost(bool set)
{
	activeChildForced()->panelView()->showCost(set);
}

//Show mL liquid or L gas
void GasMainWindow::showMl(bool set)
{
	activeChildForced()->panelView()->showMl(set);
}

//Show or hide the numerics under the gas machine
void GasMainWindow::hideNumerics(bool set)
{
	activeChildForced()->panelView()->hideNumerics(set);
}

//Disable or enable uptake
void GasMainWindow::disableUptake(bool set)
{
	activeChildForced()->doc()->disableUptake(set);
}

//Disable or enable return
void GasMainWindow::disableReturn(bool set)
{
	activeChildForced()->doc()->disableReturn(set);
}

//Enable or disable vapor
void GasMainWindow::enableVapor(bool set)
{
	activeChildForced()->doc()->enableVapor(set);
}

//Enable or disable scrolling
void GasMainWindow::enableScrolling(bool set)
{
	// only available when child window selected?
	GasChildren  children = activeChildForced()->doc()->gasChildren();
	foreach(GasChildWindow * child, children) {
		child->graphView()->enableScrolling(set);
	}
	//activeChildForced()->graphView()->enableScrolling( set );
}

//Show the bookmark dialog
void GasMainWindow::setBookmark()
{
	activeChildForced()->doc()->setBookmark();
}

void GasMainWindow::setBreakPoint()
{
	activeChildForced()->doc()->setBkPoint();
}

//Show the dialog for changing a patient
void GasMainWindow::changePatient()
{
	activeChildForced()->doc()->changePatient();
}

//Change dose dialog
void GasMainWindow::unitDose()
{
	activeChildForced()->doc()->unitDose();
}

//Set cost parameters
void GasMainWindow::setCost()
{
	activeChildForced()->doc()->setCost();
}

//Change FGF, VA, CO etc.
void GasMainWindow::adjustDel()
{
	activeChildForced()->doc()->adjustDel();
}

//Change agent
void GasMainWindow::changeAgent()
{
	activeChildForced()->panelView()->changeAgent();
}

//Show or hide the Tool Bar
void GasMainWindow::setShowToolbar(bool set)
{
	toolBar->setVisible(set);
}

//Set up the default options for the application
void GasMainWindow::setDefaultOptions()
{
	GasOptionsDialog dlg(this);

	dlg.patientParameters()->setWeight(GasDoc::m_fDfltWeight);
	dlg.patientParameters()->setVolumeALV(GasDoc::m_fDfltVolume[ALV]);
	dlg.patientParameters()->setVolumeVRG(GasDoc::m_fDfltVolume[VRG]);
	dlg.patientParameters()->setVolumeMUS(GasDoc::m_fDfltVolume[MUS]);
	dlg.patientParameters()->setVolumeFAT(GasDoc::m_fDfltVolume[FAT]);
	dlg.patientParameters()->setVolumeVEN(GasDoc::m_fDfltVolume[VEN]);
	dlg.patientParameters()->setRatioVRG(qRound(GasDoc::m_fDfltRatio[VRG] * 100));
	dlg.patientParameters()->setRatioMUS(qRound(GasDoc::m_fDfltRatio[MUS] * 100));
	dlg.patientParameters()->setRatioFAT(qRound(GasDoc::m_fDfltRatio[FAT] * 100));
	dlg.patientParameters()->setCO(GasDoc::m_fDfltCO);
	dlg.patientParameters()->setVA(GasDoc::m_fDfltVA);

	int nOldBeep, nOldLabel, nOldThick, nOldPercent;
	bool bOldGraphpaper;

	nOldBeep = GasDoc::m_nBeep;

	dlg.programDefaults()->setSignal(nOldBeep);
	dlg.programDefaults()->setLineLabel(nOldLabel = GasGraphView::m_nLineLabelType);
	dlg.programDefaults()->setLineThickness(nOldThick = GasGraphView::m_nLineWidth - 1);
	dlg.programDefaults()->setJumpPercent(nOldPercent = GasGraphView::m_nScrollPercent);
	dlg.programDefaults()->setUseGraphpaper(bOldGraphpaper = GasGraphView::m_bGraphpaper);

	quint16 nOldHighCO, nOldHighFGF, nOldHighVA;
	bool bOldEnableScroll, bOldShowCost, bOldShowControl, bOldShowMl;
	QString strOldScale;

	dlg.viewDefaults()->setEnabledScroll(bOldEnableScroll = GasGraphView::m_bDfltScrollEnb);
	dlg.viewDefaults()->setScale(strOldScale = GasGraphView::m_szScale);
	dlg.viewDefaults()->setVisibleCost(bOldShowCost = GasPanelView::m_nDfltShowCost);
	dlg.viewDefaults()->setVisibleMl(bOldShowMl = GasPanelView::m_nDfltShowMl);
	dlg.viewDefaults()->setVisibleControl(bOldShowControl = GasPanelView::m_nDfltShowControl);
	dlg.viewDefaults()->setHighCO(nOldHighCO = GasPanelView::m_nDfltHIGH_CO);
	dlg.viewDefaults()->setHighFGF(nOldHighFGF = GasPanelView::m_nDfltHIGH_FGF);
	dlg.viewDefaults()->setHighVA(nOldHighVA = GasPanelView::m_nDfltHIGH_VA);

	QString strOldCircuit, strOldSpeed;
	QString strOldAgent = gasApp->ReadProfile("Defaults", "Agent", QString()).toString();

	if (GasDoc::m_nDfltSpeed != AFAP)
		strOldSpeed = QString("%1x").arg(GasDoc::m_nDfltSpeed);
	else
		strOldSpeed = tr("AFAP");

	GasAnesArray anesArray;
	int nAnes = anesArray.ReadProfile();

	dlg.simulationDefaults()->setSpeed(strOldSpeed);
	dlg.simulationDefaults()->setCircuit(strOldCircuit = GasDoc::m_szDfltCircuit);
	dlg.simulationDefaults()->setFGF(GasDoc::m_fDfltFGF);
	dlg.simulationDefaults()->setVolumeCKT(GasDoc::m_fDfltVolume[CKT]);
	dlg.simulationDefaults()->setAgent(anesArray[anesArray.LookupSection(strOldAgent)]->m_strName);

	for (int i = 0; i < nAnes; i++)
	{
		GasAnesthetic& anes = *anesArray[i];
		dlg.colorDefaults()->addAgentSizeCostColor(anes.m_strName, anes.m_nBottleSize, anes.m_fBottleCost, anes.m_defaultColor, anes.m_bIsLiquid);
	}

	if (dlg.exec() == QDialog::Rejected)		 //Shows the dialog as a modal dialog
		return;

	GasAttributeMap agentsAndSizes = dlg.colorDefaults()->getSizes();
	GasAttributeMap agentsAndCosts = dlg.colorDefaults()->getCosts();
	GasAttributeMap agentsAndColors = dlg.colorDefaults()->getColors();
	for (int i = 0; i < nAnes; i++)
	{
		GasAnesthetic& anes = *anesArray[i];
		QString name = anes.m_strName;
		if (agentsAndSizes.contains(name) && agentsAndCosts.contains(name) && agentsAndColors.contains(name))
		{
			int size = agentsAndSizes[name].toInt();
			if (size != 0 && anes.m_nBottleSize != size)
				gasApp->WriteProfile(anes.m_strSectionName, "BottleSize", size);

			double cost = agentsAndCosts[name].toDouble();
			if (anes.m_fBottleCost != cost)
			{
				if (cost != 0.0)
					gasApp->WriteProfile(anes.m_strSectionName, "BottleCost", cost);
				else
					gasApp->WriteProfile(anes.m_strSectionName, "BottleCost", 0);
			}

			QString color = agentsAndColors[name].toString();
			if (anes.m_defaultColor != color)
				gasApp->WriteProfile(anes.m_strSectionName, "Color", color);
		}
	}

	if (!FNear(GasDoc::m_fDfltWeight, dlg.patientParameters()->weight())) {
		GasDoc::m_fDfltWeight = dlg.patientParameters()->weight();
		gasApp->WriteProfile("Defaults", "Weight", GasDoc::m_fDfltWeight);
	}

	if (!FNear(GasDoc::m_fDfltVolume[ALV], dlg.patientParameters()->volumeALV())) {
		GasDoc::m_fDfltVolume[ALV] = dlg.patientParameters()->volumeALV();
		gasApp->WriteProfile("Volumes", "ALV", GasDoc::m_fDfltVolume[ALV]);
	}

	if (!FNear(GasDoc::m_fDfltVolume[VRG], dlg.patientParameters()->volumeVRG())) {
		GasDoc::m_fDfltVolume[VRG] = dlg.patientParameters()->volumeVRG();
		gasApp->WriteProfile("Volumes", "VRG", GasDoc::m_fDfltVolume[VRG]);
	}

	if (!FNear(GasDoc::m_fDfltVolume[MUS], dlg.patientParameters()->volumeMUS())) {
		GasDoc::m_fDfltVolume[MUS] = dlg.patientParameters()->volumeMUS();
		gasApp->WriteProfile("Volumes", "MUS", GasDoc::m_fDfltVolume[MUS]);
	}

	if (!FNear(GasDoc::m_fDfltVolume[FAT], dlg.patientParameters()->volumeFAT())) {
		GasDoc::m_fDfltVolume[FAT] = dlg.patientParameters()->volumeFAT();
		gasApp->WriteProfile("Volumes", "FAT", GasDoc::m_fDfltVolume[FAT]);
	}

	if (!FNear(GasDoc::m_fDfltVolume[VEN], dlg.patientParameters()->volumeVEN())) {
		GasDoc::m_fDfltVolume[VEN] = dlg.patientParameters()->volumeVEN();
		gasApp->WriteProfile("Volumes", "VEN", GasDoc::m_fDfltVolume[VEN]);
	}

	if (!FNear(GasDoc::m_fDfltRatio[VRG], dlg.patientParameters()->ratioVRG() / 100.0F)) {
		GasDoc::m_fDfltRatio[VRG] = dlg.patientParameters()->ratioVRG() / 100.0F;
		gasApp->WriteProfile("Ratio", "VRG", GasDoc::m_fDfltRatio[VRG]);
	}

	if (!FNear(GasDoc::m_fDfltRatio[MUS], dlg.patientParameters()->ratioMUS() / 100.0F)) {
		GasDoc::m_fDfltRatio[MUS] = dlg.patientParameters()->ratioMUS() / 100.0F;
		gasApp->WriteProfile("Ratio", "MUS", GasDoc::m_fDfltRatio[MUS]);
	}

	if (!FNear(GasDoc::m_fDfltRatio[FAT], dlg.patientParameters()->ratioFAT() / 100.0F)) {
		GasDoc::m_fDfltRatio[FAT] = dlg.patientParameters()->ratioFAT() / 100.0F;
		gasApp->WriteProfile("Ratio", "FAT", GasDoc::m_fDfltRatio[FAT]);
	}

	if (!FNear(GasDoc::m_fDfltCO, dlg.patientParameters()->NormalCO())) {
		GasDoc::m_fDfltCO = dlg.patientParameters()->NormalCO();
		gasApp->WriteProfile("Defaults", "CO", GasDoc::m_fDfltCO);
	}

	if (!FNear(GasDoc::m_fDfltVA, dlg.patientParameters()->NormalVA())) {
		GasDoc::m_fDfltVA = dlg.patientParameters()->NormalVA();
		gasApp->WriteProfile("Defaults", "VA", GasDoc::m_fDfltVA);
	}

	bool bChangeGraphs = false;

	int nNewBeep = dlg.programDefaults()->signal();
	if (nOldBeep != nNewBeep) {
		GasDoc::m_nBeep = nNewBeep;
		gasApp->WriteProfile("Defaults", "Click", GasDoc::m_nBeep);
	}
	if (nOldLabel != dlg.programDefaults()->lineLabel()) {
		GasGraphView::m_nLineLabelType = dlg.programDefaults()->lineLabel();
		gasApp->WriteProfile("Defaults", "LineLabels", GasGraphView::m_nLineLabelType);
		bChangeGraphs = true;
	}
	if (nOldThick != dlg.programDefaults()->lineThickness()) {
		GasGraphView::m_nLineWidth = dlg.programDefaults()->lineThickness() + 1;
		gasApp->WriteProfile("Defaults", "LineWidth", GasGraphView::m_nLineWidth);
		bChangeGraphs = true;
	}
	if (nOldPercent != dlg.programDefaults()->jumpPercent()) {
		GasGraphView::m_nScrollPercent = dlg.programDefaults()->jumpPercent();
		gasApp->WriteProfile("Defaults", "ScrollPercent", GasGraphView::m_nScrollPercent);
	}
	if (bOldGraphpaper != dlg.programDefaults()->isUseGraphpaper()) {
		GasGraphView::m_bGraphpaper = dlg.programDefaults()->isUseGraphpaper();
		gasApp->WriteProfile("Defaults", "GraphPaper", GasGraphView::m_bGraphpaper);
		bChangeGraphs = true;
	}

	if (bChangeGraphs)
		foreach(GasChildWindow *child, allChildren()) {
		child->graphView()->DrawGraphs(true);
	}

	if (bOldEnableScroll != dlg.viewDefaults()->isEnabledScroll()) {
		GasGraphView::m_bDfltScrollEnb = dlg.viewDefaults()->isEnabledScroll();
		gasApp->WriteProfile("Defaults", "Scroll", GasGraphView::m_bDfltScrollEnb);
	}
	QString strNewScale = dlg.viewDefaults()->scale();
	if (strOldScale != strNewScale) {
		int pos = strNewScale.indexOf(' ');
		GasGraphView::m_nDfltScaleMinutes = strNewScale.left(pos).toShort();
		GasGraphView::m_szScale = strNewScale;
		if ((pos = strNewScale.indexOf(QRegExp(QObject::tr("[HM]")), pos)) != -1)
			if (strNewScale.at(pos) == QObject::tr("[HM]").at(1))
				GasGraphView::m_nDfltScaleMinutes *= 60;
		gasApp->WriteProfile("Defaults", "GraphMinutes", GasGraphView::m_nDfltScaleMinutes);
	}
	if (bOldShowCost != dlg.viewDefaults()->isVisibleCost()) {

		GasPanelView::m_nDfltShowCost = dlg.viewDefaults()->isVisibleCost();
		gasApp->WriteProfile("Defaults", "ShowCost", GasPanelView::m_nDfltShowCost);
	}
	if (bOldShowMl != dlg.viewDefaults()->isVisibleMl()) {

		GasPanelView::m_nDfltShowMl = dlg.viewDefaults()->isVisibleMl();
		gasApp->WriteProfile("Defaults", "ShowMl", GasPanelView::m_nDfltShowMl);
	}
	if (bOldShowControl != dlg.viewDefaults()->isVisibleControl()) {
		GasPanelView::m_nDfltShowControl = dlg.viewDefaults()->isVisibleControl();
		gasApp->WriteProfile("Defaults", "ShowControl", GasPanelView::m_nDfltShowControl);
	}
	if (nOldHighCO != dlg.viewDefaults()->highCO()) {
		GasPanelView::m_nDfltHIGH_CO = dlg.viewDefaults()->highCO();
		gasApp->WriteProfile("Defaults", "HighCO", GasPanelView::m_nDfltHIGH_CO);
	}
	if (nOldHighFGF != dlg.viewDefaults()->highFGF()) {
		GasPanelView::m_nDfltHIGH_FGF = dlg.viewDefaults()->highFGF();
		gasApp->WriteProfile("Defaults", "HighFGF", GasPanelView::m_nDfltHIGH_FGF);
	}
	if (nOldHighVA != dlg.viewDefaults()->highVA()) {
		GasPanelView::m_nDfltHIGH_VA = dlg.viewDefaults()->highVA();
		gasApp->WriteProfile("Defaults", "HighVA", GasPanelView::m_nDfltHIGH_VA);
	}
	QString strNewSpeed = dlg.simulationDefaults()->speed();
	if (strOldSpeed != strNewSpeed) {
		int nSpeed = AFAP;

		if (strNewSpeed == tr("AFAP") || (nSpeed = strNewSpeed.left(strNewSpeed.indexOf('x')).toInt()) != 0) {
			GasDoc::m_nDfltSpeed = nSpeed;
			if (nSpeed == AFAP)
				gasApp->WriteProfile("Defaults", "Speed", "AFAP");
			else
				gasApp->WriteProfile("Defaults", "Speed", nSpeed);
		}
	}
	if (strOldCircuit != dlg.simulationDefaults()->circuit()) {
		GasDoc::m_szDfltCircuit = dlg.simulationDefaults()->circuit();
		gasApp->WriteProfile("Defaults", "Circuit", GasDoc::m_szDfltCircuit);
	}
	if (!FNear(GasDoc::m_fDfltFGF, dlg.simulationDefaults()->FGF())) {
		GasDoc::m_fDfltFGF = dlg.simulationDefaults()->FGF();
		gasApp->WriteProfile("Defaults", "FGF", GasDoc::m_fDfltFGF);
	}
	if (!FNear(GasDoc::m_fDfltVolume[CKT], dlg.simulationDefaults()->volumeCKT())) {
		GasDoc::m_fDfltVolume[CKT] = dlg.simulationDefaults()->volumeCKT();
		gasApp->WriteProfile("Volumes", "CKT", GasDoc::m_fDfltVolume[CKT]);
	}
	if (strOldAgent != dlg.simulationDefaults()->agent())
		gasApp->WriteProfile("Defaults", "Agent", anesArray[anesArray.Lookup(dlg.simulationDefaults()->agent())]->m_strSectionName);

	// save gascdefaults into .ini
}

bool GasMainWindow::printHelper(const QPrinter::OutputFormat fmt, const QString& fname)
{
	GasChildWindow* child = activeChildForced();
	if (child->doc()->GetDescription() == "" && !editDescription())
		return false;

	prn->setOutputFormat(fmt);
	prn->setOutputFileName(fname);
    prn->setPageOrientation(QPageLayout::Portrait);
	updateLocale();
	qDebug() << "pageRect: " << prn->pageRect(QPrinter::Inch) << '\n';
	qDebug() << "resolution: " << prn->resolution() << '\n';

	if (!gasPrintDocument)
		gasPrintDocument = new QTextDocument(this);
	updateGasPrintDoc("Courier", 10);

	writeHtmlToGasPrintDoc(child);
	return true;
}

//Print the current document
void GasMainWindow::print()
{
	if (glm->instance()->validLicenseExists())
	{
		if (!printHelper(QPrinter::NativeFormat, QString()))
			return;
		QPrintDialog *dlg = new QPrintDialog(prn, this);		//Show the print dialog
		if (dlg->exec() == QDialog::Accepted)
			gasPrintDocument->print(prn);
		delete dlg;
	} else {
		QMessageBox::information(activeChildForced(), tr("Gas Man\xC2\xAE "), tr("Print not available in student mode"));
	}
}

// update printer page size depending on locale
void GasMainWindow::updateLocale()
{
	Q_ASSERT(prn != 0);
	switch (QLocale::system().country()) {
	case QLocale::AnyCountry:
	case QLocale::Canada:
	case QLocale::UnitedStates:
	case QLocale::UnitedStatesMinorOutlyingIslands:
		prn->setPaperSize(QPrinter::Letter); break;
	default:
        prn->setPaperSize(QPrinter::A4); break;
	}
}

void GasMainWindow::updateGasPrintDoc(const QString& family, int fontSize)
{
	Q_ASSERT(prn != 0);
	Q_ASSERT(gasPrintDocument != 0);
    QSizeF pageSize = prn->pageRect().size(); // //pageLayout().paintRectPixels(resolution());
	pageSize.setWidth((pageSize.width() / prn->logicalDpiX()) * logicalDpiX());
	pageSize.setHeight((pageSize.height() / prn->logicalDpiY()) * logicalDpiY());
	gasPrintDocument->setPageSize(pageSize);

	QFont f;
	f.setFamily(family);
	f.setPointSize(fontSize);
	f.setFixedPitch(true);
	gasPrintDocument->setDefaultFont(f);
}

QString GasMainWindow::getExportFileName(const QString& title, const QString& fileTypeDesc, const QString& fileExtension)
{
#ifdef Q_OS_MACX 
	QString fileName = QFileDialog::getSaveFileName(this, title, SaveDir(),
		fileTypeDesc + " (*" + fileExtension + ")", NULL, QFileDialog::DontUseNativeDialog);
#else
	QString fileName = QFileDialog::getSaveFileName(this, title, SaveDir(), fileTypeDesc + " (*" + fileExtension + ")");
#endif

	if (!fileName.isEmpty()) {
		if (QFileInfo(fileName).suffix().isEmpty())
			fileName.append(fileExtension);
	}
	return fileName;
}

// helper to save html output to gasPrintDocument
void GasMainWindow::writeHtmlToGasPrintDoc(GasChildWindow* child)
{
	Q_ASSERT(gasPrintDocument != 0);
	QTemporaryFile tmp;
	tmp.open();

	writeHtmlToFile(child->doc(), tmp.fileName(), true);
	QString tmpString(QString::fromUtf8(tmp.readAll().constData()));
	QTextStream out(&tmpString);

	if (out.string())
		gasPrintDocument->setHtml(*out.string());
}


//Show the print preview dialog
void GasMainWindow::printPreview()
{
	if (glm->instance()->validLicenseExists())
	{
		if (!printHelper(QPrinter::NativeFormat, QString()))
			return;
		GasPrintPreview *preview = new GasPrintPreview(gasPrintDocument, prn, this);
		preview->show();
	} else {
		QMessageBox::information(activeChildForced(), tr("Gas Man\xC2\xAE"), tr("Print Preview not available in student mode"));
	}
}

//Export data to pdf format
void GasMainWindow::exportToPdf()
{
	if (glm->instance()->validLicenseExists())
	{
		QString fileName = getExportFileName(tr("Export to PDF"), tr("PDF Files"), tr(".pdf"));
		if (fileName.isEmpty() || !printHelper(QPrinter::PdfFormat, fileName))
			return;
		gasPrintDocument->print(prn);
		QFileInfo fi(fileName);
		setSaveDir(fi.path());
	} else {
		QMessageBox::information(activeChildForced(), tr("Gas Man\xC2\xAE"), tr("Export to PDF not available in student mode"));
	}
}


bool GasMainWindow::editDescription()
{
	GasDoc* doc = activeChildForced()->doc();

	QDialog descDialog;
	descDialog.setWindowTitle(tr("Edit simulation description"));

	QTextEdit textEdit;
	QDialogButtonBox bb(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	textEdit.setText(doc->GetDescription());

	QVBoxLayout vLayout;
	vLayout.addWidget(&textEdit);
	vLayout.addWidget(&bb);

	descDialog.setLayout(&vLayout);
	connect(&bb, SIGNAL(accepted()), &descDialog, SLOT(accept()));
	connect(&bb, SIGNAL(rejected()), &descDialog, SLOT(reject()));

	if (descDialog.exec() != QDialog::Accepted)
		return false;

	doc->SetDescription(textEdit.toPlainText());
	return true;
}

//Export data to xml format
void GasMainWindow::exportToXml()
{
	if (glm->instance()->validLicenseExists())
	{
		GasChildWindow *child = activeChildForced();
		if (child->doc()->GetDescription() == "" && !editDescription())
			return;

		QString saveFileName = getExportFileName(tr("Export to XML"), tr("XML Files"), tr(".xml"));

		if (!saveFileName.isEmpty())
		{
			QFile file(saveFileName);
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
				return;
			QTextStream stream(&file);
			stream.setCodec(QTextCodec::codecForName("utf-8"));

			GasDoc *document = child->doc();

			QByteArray ba = FirstGraphView(document)->toArray();

			document->toXml(ba, "", fileName(gasApp->getIconFile()), m_bPrintGraphs, m_bPrintout, true).save(stream, 0);

			file.close();
			QFileInfo fi(saveFileName);
			setSaveDir(fi.path());
		}
	} else {
		QMessageBox::information(activeChildForced(), tr("Gas Man\xC2\xAE"), tr("Export to XML not available in student mode"));
	}
}

GasGraphView* GasMainWindow::FirstGraphView(GasDoc* pdoc)
{
	return FirstGraphWindow(pdoc)->graphView();
}

GasChildWindow* GasMainWindow::FirstGraphWindow(GasDoc *pdoc)
{
	const GasChildren& children = pdoc->gasChildren();
	foreach(GasChildWindow* pchild, children)
	{
		if (pchild->getViewType() == GRAPH)
			return pchild;
	}
	return children.first();
}

void GasMainWindow::transform()
{
}

//Export data to html format
void GasMainWindow::exportToHtml()
{
	if (glm->instance()->validLicenseExists())
	{
		GasChildWindow* child = activeChildForced();
		if (child->doc()->GetDescription() == "" && !editDescription())
			return;

		QString fileName = getExportFileName(tr("Export to HTML"), tr("Web Page"), tr(".html"));
		if (!fileName.isEmpty())
			writeHtmlToFile(child->doc(), fileName, false);
	} else {
		QMessageBox::information(activeChildForced(), tr("Gas Man\xC2\xAE "), tr("Export to HTML not available in student mode"));
	}

}

void GasMainWindow::writeHtmlToFile(GasDoc* doc, const QString& htmlFileName, bool tmpImage)
{
	// get png back, save to image
	QByteArray ba = FirstGraphView(doc)->toArray();
	QImage image = QImage::fromData(QByteArray::fromBase64(ba));

	// create a temporary .png file for image
	QFile imageFile;
	QString base = completeBaseName(htmlFileName);
	QString imageName = tmpImage ? QString() : base + "_img.png";
	QString imageFileName = createOutputImageFile(imageFile, htmlFileName, imageName);
	if (imageFileName.isNull())
		return;
	image.save(&imageFile, "PNG", 0);
	imageName = imageFile.fileName();

	// and one for icon
	QFile iconFile;
	QString iconName = "missing";
	QString iconFileName = createOutputImageFile(iconFile, htmlFileName, tmpImage ? QString() : base + "_icon.png");
	if (!iconFileName.isNull())
	{
		QImage iconImage = QImage(gasApp->checkFile("gasman_lg.png"), "PNG");
		if (!iconImage.isNull())
		{
			iconImage.save(&iconFile, "PNG", 0);
			iconName = iconFile.fileName();
		}
	}


	QDomDocument dom = doc->toXml(ba, imageName, iconName, m_bPrintGraphs, m_bPrintout, false);
	saveTransform(dom, gasApp->getXslFile(), htmlFileName);
}

bool GasMainWindow::saveTransform(QDomDocument& dom, const QString& transformFileName, const QString& outputFileName)
{
	return true;
}

// Create an output .png file and return its name. If imageName is empty, the file will be a temp
QString GasMainWindow::createOutputImageFile(QFile& imageFile, const QString& htmlFileName, const QString& imageName)
{
	if (imageFile.openMode() != QIODevice::NotOpen)
		imageFile.close();

	QString newFileName;

	if (imageName.isEmpty())
	{
		QTemporaryFile tmpFile;
		if (!tmpFile.open())
			return QString();
		tmpFile.setAutoRemove(false);
		QFileInfo fi(tmpFile);
		newFileName = fi.absoluteFilePath();
		tmpFile.close();
		gasApp->addTemporaryFile(newFileName);
	} else {
		QFileInfo fi(htmlFileName);
		newFileName = fi.absoluteDir().path() + "/" + imageName;
		if (QFile::exists(newFileName))
			QFile::remove(newFileName);
	}

	imageFile.setFileName(newFileName);
	if (!imageFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
		return QString();
	return newFileName;
}

//Show configuration dialog for the page-related options on a printer
void GasMainWindow::pageSetup()
{
	QPageSetupDialog dlg(prn, this);
	dlg.exec();
}

//Create a new view for the current simulation
void GasMainWindow::newView(GasDoc* doc, int nGas, GasViewType viewType)
{
	if (doc)
		doc->m_nActiveGas = nGas;
	else
	{
		doc = activeChildForced()->doc();
		nGas = doc->m_nActiveGas;
	}

	GasChildWindow *child = 0;
	if ((sender() == newGraphViewAction) || (viewType == GRAPH))
		child = createChild(doc, nGas, GRAPH);
	else
		child = createChild(doc, nGas, PANEL);

	tabifyChild(child);

	// quick fix for new views not repainting graph results when shown
	//doc->UpdateAllViews( 0, 0, GasUpdateEvent::UPD_NONE );
	updateViewMenu();
}

//Show help topics
void GasMainWindow::showHelpTopic()
{
	//assistant->showDocumentation("index.html");
	QDesktopServices::openUrl(tr("http://gasmanhelp.medmansimulations.org"));
}

//Show context help
void GasMainWindow::showHelp()
{
	QMessageBox::information(this, tr("Gas Man\xC2\xAE "), tr("Feature not implemented"));

	//QWhatsThis::enterWhatsThisMode();
}

//Show workbook
void GasMainWindow::showWorkbook()
{
	QDesktopServices::openUrl(tr("http://gasmanweb.com/Workbook.pdf"));
}

//Select print output dialog
void GasMainWindow::selectPrintOutput()
{
	GasPrintSelDlg dlg(this);

	dlg.setGraphs(m_bPrintGraphs);
	dlg.setPrintout(m_bPrintout);

	if (dlg.exec() == QDialog::Rejected)
		return;

	m_bPrintGraphs = dlg.isGraphs();
	m_bPrintout = dlg.isPrintout();
}

//Open the last simulation
void GasMainWindow::openRecentFile()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		open(action->data().toString());
}

//Show the overlay dialaog
void GasMainWindow::overlay()
{
	// handle gas control
	activeChildForced()->panelView()->StopForChange();
	GasChildWindow *w = largestVisibleGraphic();
	if (w == 0)
		return;
	GasOverlay *over = new GasOverlay(this);

	over->init(w->graphView());//activeChildForced()->graphView() );
	over->show();
	over->UpdateData(false);
}

//Create menu for recent files
void GasMainWindow::createRecentMenu()
{
	for (int i = 0; i < MaxRecentFiles; ++i)
		openRecentMenu->addAction(recentFileActs[i]);
	updateRecentFileActions();
}

//Update all File menu items
void GasMainWindow::updateFileMenu()
{
	// edit
	bool enable = activeChild() != 0;

	closeDocumentAction->setEnabled(enable);
	closeAllDocumentsAction->setEnabled(enable);
	saveDocumentAction->setEnabled(enable);
	saveDocumentAsAction->setEnabled(enable);
	runAction->setEnabled(enable);
	runAllAction->setEnabled(enable && allChildren().size() > 1);
	rewindAllAction->setEnabled(enable && CountRewindable() > 0);
#ifndef Q_OS_UNIX
	sendAction->setEnabled(enable);
#endif
	printPreviewAction->setEnabled(enable);

	menuExport_to->setEnabled(enable);
	printAction->setEnabled(enable);

	if (!activeChild() || activeChild()->doc()->m_bClosed) return;

	AState state = activeChildForced()->doc()->GetState();		//Get the state of the current simulation

	bool bOkToGo = state == STOPPED_STATE || state == INITIAL_STATE;
	if (bOkToGo) {
		runAction->setText(tr("Run"));
		runAction->setIcon(QIcon(":/images/run.png"));
		//runAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right));
	}
	else {
		runAction->setText(tr("Stop"));
		runAction->setIcon(QIcon(":/images/stop.png"));
		//runAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));
	}
	runAction->setEnabled(state != END_STATE);


	if (CountRunners()) {
		runAllAction->setText(tr("Stop All"));
		runAllAction->setIcon(QIcon(":/images/stopall.png"));
	}
	else if (CountRunable()) {
		runAllAction->setText(tr("Run All"));
		runAllAction->setIcon(QIcon(":/images/runall.png"));
	}
	else
		runAllAction->setEnabled(false);
}

//Update all Edit menu items
void GasMainWindow::updateEditMenu()
{

	if (!activeChild() || activeChild()->doc()->m_bClosed) {
		foreach(QAction* act, editMenu->actions())
			act->setEnabled(false);
	} else {
		foreach(QAction* act, editMenu->actions())
			act->setEnabled(true);

		GasDoc *doc = activeChildForced()->doc();

		rewindAction->setEnabled(doc->GetTime() > 0L);
		fastFwdAction->setEnabled(!doc->IsOver());

		zeroTimerAction->setEnabled(doc->GetHighCalcTime() > 0L);
		copySelectionAction->setEnabled(!shootscreen.isNull());
	}
}

//Update all Anesthesia menu items
void GasMainWindow::updateAnesthesiaMenu()
{
	GasChildWindow* child = activeChild();
	int activeGas;
	bool ok = child && !child->doc()->m_bClosed && child->tryGetGas(activeGas);

	foreach(QAction* act, anesthesiaMenu->actions())
		act->setEnabled(ok);

	if (!ok)
		return;

	GasDoc *doc = child->doc();

	unitDoseAction->setEnabled(doc->GetUnitDose(activeGas) > 0.0F);
	liquidInjectAction->setEnabled(doc->CanInject(activeGas));
	setCostAction->setEnabled(doc->GetAgent(activeGas)->m_wVolatility);
}

//void GasMainWindow::giveFocusToMain()
//{
//	qApp->setActiveWindow(this);
//}

//Create a View menu for the Main Menu
void GasMainWindow::createViewMenu()
{
	viewMenu->clear();
	viewMenu->addMenu(newGraphViewMenu);
	viewMenu->addMenu(newPanelViewMenu);
	//viewMenu->addAction( newGraphViewAction );
	//viewMenu->addAction( newPanelViewAction );
	viewMenu->addAction(overlayAction);
	viewMenu->addSeparator();

	//	viewMenu->addAction(zoominAction);
	//	viewMenu->addAction(zoomoutAction);
	//	viewMenu->addSeparator();
	viewMenu->addAction(showToolbarAction);
}

void GasMainWindow::createWindowMenu()
{
	windowMenu->clear();
	windowMenu->addAction(tileSubWindowsAction);
	windowMenu->addAction(cascadeSubWindowsAction);
	windowMenu->addAction(groupSubWindowsAction);

	if (!allChildren().isEmpty())
	{
		windowMenu->addSeparator();
		windowMenu->addAction(pictureOnTopAction);
		windowMenu->addAction(graphOnTopAction);
		windowMenu->addAction(controlOnTopAction);
		windowMenu->addSeparator();
	}

	bool haveActive = activeChild() != 0 && !activeChild()->doc()->m_bClosed;
	foreach(QAction* act, windowMenu->actions())
		act->setEnabled(haveActive);

	foreach(GasDoc* doc, docs) {
		QMenu * menu = new QMenu(doc->title());
		GasChildren children = doc->gasChildren();
		foreach(GasChildWindow * child, children) {
			QString text;

			text = tr("%1").arg(child->title());
			QAction *action = menu->addAction(text);
			action->setCheckable(true);
			action->setChecked(child == activeChild());
			connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
			windowMapper->setMapping(action, child);
		}


		windowMenu->addMenu(menu);
		//		tmp += doc->gasChildren();
	}
	//doc->gasChildren()

	/*
	for ( int i = 0; i < childs().size(); ++i ) {
	GasChildWindow *child = childs().at(i);
	QString text;
	if ( i < 9 ) {
	text = tr("&%1 %2").arg(i + 1)
	.arg( child->title() );
	} else {
	text = tr("%1 %2").arg(i + 1)
	.arg( child->title() );
	}
	QAction *action  = windowMenu->addAction( text );
	action->setCheckable( true );
	action->setChecked( child == activeChild() );
	connect( action, SIGNAL( triggered() ), windowMapper, SLOT( map() ) );
	windowMapper->setMapping( action, child );
	}*/

}

////Create a language menu
//void GasMainWindow::createLanguageMenu()
//{
//	QString qmPath = qApp->applicationDirPath() + "/language";		//Folder in which the language files are located
//	QActionGroup *languageActionGroup = new QActionGroup( this );
//	//Create connections
//	connect( languageActionGroup, SIGNAL( triggered( QAction *) ), this, SLOT( switchLanguage( QAction * ) ) );
//	QDir dir( qmPath );
//	//Fet all *.qm files from "language" dir
//	QStringList fileNames =	dir.entryList( QStringList( "gasman_*.qm" ) );
//	for ( int i = 0; i < fileNames.size(); ++i ) {
//		QString locale = fileNames[i];
//		locale.remove( 0, locale.indexOf('_') + 1 );
//		locale.truncate( locale.lastIndexOf('.') );
//		QTranslator translator;
//		translator.load( fileNames[i], qmPath );
//		QString language = translator.translate( "GasMainWindow", "English" );
//		//Create menu item
//		QAction *action = new QAction( tr("&%1 %2").arg(i + 1).arg(language), this);
//		action->setCheckable(true);
//		action->setData(locale);
//		languageMenu->addAction(action);
//		languageActionGroup->addAction(action);
//		if ( language == tr( "English" ) )
//			action->setChecked(true);
//	}
//}
//
////Change GUI language
//void GasMainWindow::switchLanguage( QAction *action )
//{
//	QString locale = action->data().toString();
//	gasApp->WriteProfile( "Application", "Locale", locale );
//	QMessageBox::information( this, tr( "Gas Man\xC2\xAE ), tr( "Changes will have effect after restart of the application." ) );
//}

//Update all View menu items
void GasMainWindow::updateViewMenu()
{
	if (!activeChild() || activeChild()->doc()->m_bClosed) {
		foreach(QAction* act, viewMenu->actions())
			act->setEnabled(false);
		//add here just for convenience
		editDescriptionAction->setEnabled(false);

	} else {
		foreach(QAction* act, viewMenu->actions())
			act->setEnabled(true);
		// zoom actions are only enabled when the activechild is graph
		if (activeChild()->objectName() != "Graph") {
			zoominAction->setEnabled(false);
			zoomoutAction->setEnabled(false);
		}
		// if the zoom is 1x zoomout is disabled;
		// if the zoom is 3x zoomin is disabled;
		else {
			if (activeChild()->graphView()->getZoom() == 1) {
				zoomoutAction->setEnabled(false);
			} else if (activeChild()->graphView()->getZoom() == 3) {
				zoominAction->setEnabled(false);
			}
		}
		//add here just for convenience
		editDescriptionAction->setEnabled(true);



		overlayAction->setEnabled(largestVisibleGraphic() != 0);//!activeChildForced()->isMinimized() );

		newGraphViewMenu->clear();
		newPanelViewMenu->clear();
		GasDoc* doc = activeChildForced()->doc();
		QList<QString> agents;
		for (int i = 0; i < doc->m_gasArray.size(); i++) {
			agents << doc->GetAgent(i)->m_strName;
		}

		foreach(QString agent, agents)
		{
			QAction* action1 = newGraphViewMenu->addAction(agent);
			QAction* action2 = newPanelViewMenu->addAction(agent);
			connect(action1, SIGNAL(triggered()), this, SLOT(newViewOrAdd()));
			connect(action2, SIGNAL(triggered()), this, SLOT(newViewOrAdd()));
		}

		QStringList newAgents;
		GasAnesArray& ar = doc->m_anesArray;
		for (int i = 0; i < ar.size(); i++)
		{
			newAgents << ar[i]->m_strName;
		}

		foreach(QString agent, newAgents)
		{
			if (!agents.contains(agent))
			{
				QAction* action1 = newGraphViewMenu->addAction(tr("(Add) ") + agent);
				QAction* action2 = newPanelViewMenu->addAction(tr("(Add) ") + agent);
				connect(action1, SIGNAL(triggered()), this, SLOT(newViewOrAdd()));
				connect(action2, SIGNAL(triggered()), this, SLOT(newViewOrAdd()));
				if (doc->GetState() != INITIAL_STATE)
				{
					action1->setEnabled(false);
					action2->setEnabled(false);
				}
			}
		}

		if (activeChildForced()->gasControl())
		{
			newGraphViewAction->setEnabled(false);
			newPanelViewAction->setEnabled(false);
		}
	}
	showToolbarAction->setEnabled(true);
}

void GasMainWindow::updateWindowMenu()
{
	// Context is important to content... recreate
	createWindowMenu();
}

//Update all Special menu items
void GasMainWindow::updateSpecialMenu()
{
	GasChildWindow* child = activeChild();
	if (!child || child->doc()->m_bClosed) {
		foreach(QAction* act, specialMenu->actions())
			act->setEnabled(false);
	} else {
		foreach(QAction* act, specialMenu->actions())
			act->setEnabled(true);

		int activeGas;
		GasDoc *doc = child->doc();
		GasGraphView *graph = child->graphView();
		GasPanelView *panel = child->panelView();
		bool isPanel = activeChildForced()->getViewType() == PANEL;
		bool bIsLiquid = child->tryGetGas(activeGas) && doc->GetUnitDose(activeGas) > 0.0F;

		bool blockSignal = disableUptakeAction->blockSignals(true);
		disableUptakeAction->setChecked(doc->isDisabledUptake());
		disableUptakeAction->blockSignals(blockSignal);

		blockSignal = disableReturnAction->blockSignals(true);
		disableReturnAction->setChecked(doc->isDisabledReturn());
		disableReturnAction->blockSignals(blockSignal);

		blockSignal = enableVaporAction->blockSignals(true);
		if (!bIsLiquid)
			enableVaporAction->setChecked(false);
		else
			enableVaporAction->setChecked(doc->isEnabledVapor());
		enableVaporAction->blockSignals(blockSignal);

		blockSignal = enableScrollingAction->blockSignals(true);
		enableScrollingAction->setChecked(graph->isEnabledScrolling());
		enableScrollingAction->blockSignals(blockSignal);

		blockSignal = showCostAction->blockSignals(true);
		showCostAction->setChecked(isPanel && panel->isShownCost());
		showCostAction->blockSignals(blockSignal);

		blockSignal = showMlAction->blockSignals(true);
		showMlAction->setChecked(isPanel && panel->isShownMl());
		showMlAction->blockSignals(blockSignal);

		blockSignal = hideNumericsAction->blockSignals(true);
		hideNumericsAction->setChecked(isPanel && panel->isHiddenNumerics());
		hideNumericsAction->blockSignals(blockSignal);

		if (!isPanel) {
			showCostAction->setEnabled(false);
			showMlAction->setEnabled(false);
			hideNumericsAction->setEnabled(false);
		} else {
			showMlAction->setEnabled(bIsLiquid);
		}
	}
}

//Update main window title
void GasMainWindow::updateTitle()
{
	QString childTitle = activeChild() ? " - " + activeChildForced()->title() : QString();
	if (activeChild() && activeChildForced()->isWindowModified())
		childTitle += "*";
	setWindowTitle(tr("Gas Man\xC2\xAE Anesthesia Simulator") + childTitle);
}

//Update items in the recent menu
void GasMainWindow::updateRecentFileActions()
{
	//Get recent files from gasman.ini
	QStringList files = gasApp->ReadProfile("Settings", "RecentFileList", QStringList()).toStringList();

	//Check if a recent file exists
	QStringListIterator it(files);
	while (it.hasNext()) {
		QString file = it.next();
		if (!QFile::exists(file))	//If it does not exist, remove this name the from list
			files.removeAll(file);
	}
	//Save recent files to gasman.ini
	gasApp->WriteProfile("Settings", "RecentFileList", files);

	int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

	for (int i = 0; i < numRecentFiles; ++i) {
		QString text = tr("&%1 %2").arg(i + 1).arg(QFileInfo(files[i]).fileName());
		recentFileActs[i]->setText(text);
		recentFileActs[i]->setData(files[i]);
		recentFileActs[i]->setVisible(true);
	}
	for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
		recentFileActs[j]->setVisible(false);
}

//Update all main menu items
void GasMainWindow::updateAllActions()
{
	updateActiveGas();
	updateTitle();
	updateEditMenu();
	updateSpecialMenu();
	updateViewMenu();
	updateWindowMenu();
	updateAnesthesiaMenu();
	updateFileMenu();
}

//Count of the started simulations
int GasMainWindow::CountRunners()
{
	int nRun = 0;

	GasChildIterator it(allChildren());
	while (it.hasNext()) {
		GasChildWindow *pChild = it.next();
		GasDoc *p = pChild->doc();
		switch (p->GetState())
		{
		case RUNNING_STATE:
			++nRun;
			break;
		case INITIAL_STATE:
		case STOPPED_STATE:
		case END_STATE:
			continue;
		default:
			Q_ASSERT(false);
		}
	}
	return nRun;
}

int GasMainWindow::CountRunable()
{
	int nRun = 0;

	GasChildIterator it(allChildren());
	while (it.hasNext()) {
		GasChildWindow *pChild = it.next();
		GasDoc *p = pChild->doc();
		switch (p->GetState())
		{
		case INITIAL_STATE:
		case STOPPED_STATE:
			++nRun;
			break;
		case RUNNING_STATE:
		case END_STATE:
			continue;
		default:
			Q_ASSERT(false);
		}
	}
	return nRun;
}

int GasMainWindow::CountRewindable()
{
	int nRun = 0;

	GasChildIterator it(allChildren());
	while (it.hasNext()) {
		GasChildWindow *pChild = it.next();
		GasDoc *p = pChild->doc();

		if (p->GetTime() > 0L)
			++nRun;
	}
	return nRun;
}

//Create a Tool Bar for the main window
void GasMainWindow::createToolBar()
{
	toolBar = new QToolBar(this);
	toolBar->setMovable(false);
	toolBar->setOrientation(Qt::Horizontal);
	addToolBar(Qt::TopToolBarArea, toolBar);

	toolBar->addAction(newDocumentAction);
	toolBar->addAction(openDocumentAction);
	toolBar->addAction(saveDocumentAction);
	toolBar->addAction(printAction);
	toolBar->addSeparator();
	toolBar->addAction(copyDataAction);
	toolBar->addAction(copySelectionAction);
	toolBar->addSeparator();
	toolBar->addAction(showCostAction);
	toolBar->addAction(showMlAction);
	toolBar->addAction(overlayAction);
	toolBar->addAction(setBookmarkAction);
	toolBar->addSeparator();
	toolBar->addAction(rewindAllAction);
	toolBar->addAction(rewindAction);
	toolBar->addAction(runAction);
	toolBar->addAction(fastFwdAction);
	toolBar->addAction(runAllAction);
	toolBar->addSeparator();
	//toolBar->addAction(helpAction);

	toolBar->installEventFilter(this);
}

//Create all actions
void GasMainWindow::createActions()
{
	newDocumentAction = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
	newDocumentAction->setShortcutContext(Qt::ApplicationShortcut);
	newDocumentAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	connect(newDocumentAction, SIGNAL(triggered()), this, SLOT(newFile()));

	openDocumentAction = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
	openDocumentAction->setShortcutContext(Qt::ApplicationShortcut);
	openDocumentAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
	connect(openDocumentAction, SIGNAL(triggered()), this, SLOT(openFile()));

	closeDocumentAction = new QAction(tr("&Close Simulation"), this);
	closeDocumentAction->setShortcutContext(Qt::ApplicationShortcut);
	closeDocumentAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_W));
	connect(closeDocumentAction, SIGNAL(triggered()), this, SLOT(closeFile()));

	closeAllDocumentsAction = new QAction(tr("Close All"), this);
	closeAllDocumentsAction->setShortcutContext(Qt::ApplicationShortcut);
	closeAllDocumentsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_W));
	connect(closeAllDocumentsAction, SIGNAL(triggered()), this, SLOT(closeAllFiles()));

	saveDocumentAction = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
	saveDocumentAction->setShortcutContext(Qt::ApplicationShortcut);
	saveDocumentAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
	connect(saveDocumentAction, SIGNAL(triggered()), this, SLOT(saveFile()));

	saveDocumentAsAction = new QAction(tr("Save &As..."), this);
	connect(saveDocumentAsAction, SIGNAL(triggered()), this, SLOT(saveFileAs()));

	runAction = new QAction(QIcon(":/images/run.png"), tr("Run"), this);
	runAction->setShortcutContext(Qt::ApplicationShortcut);
	//runAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right));
	connect(runAction, SIGNAL(triggered()), this, SLOT(runSimulation()));

	runAllAction = new QAction(QIcon(":/images/runall.png"), tr("Run All"), this);
	runAllAction->setShortcutContext(Qt::ApplicationShortcut);
	connect(runAllAction, SIGNAL(triggered()), this, SLOT(runAllSimulation()));

	printAction = new QAction(QIcon(":/images/print.png"), tr("&Print..."), this);
	connect(printAction, SIGNAL(triggered()), this, SLOT(print()));

#ifndef Q_OS_UNIX
	sendAction = new QAction(QIcon(":/images/send.png"), tr("Sen&d..."), this);
	connect(sendAction, SIGNAL(triggered()), this, SLOT(sendMail()));
#endif	
	printPreviewAction = new QAction(QIcon(":/images/printpreview.png"), tr("Print Pre&view..."), this);
	connect(printPreviewAction, SIGNAL(triggered()), this, SLOT(printPreview()));


	xmlAction = new QAction(tr("XML..."), this);
	connect(xmlAction, SIGNAL(triggered()), this, SLOT(exportToXml()));

	htmlAction = new QAction(tr("HTML..."), this);
	connect(htmlAction, SIGNAL(triggered()), this, SLOT(exportToHtml()));

	transformAction = new QAction(tr("Transform..."), this);
	connect(transformAction, SIGNAL(triggered()), this, SLOT(transform()));

	pdfAction = new QAction(QIcon(":/images/exportpdf.png"), tr("&PDF..."), this);
	connect(pdfAction, SIGNAL(triggered()), this, SLOT(exportToPdf()));


	pageSetupAction = new QAction(tr("Page Setup..."), this);
	connect(pageSetupAction, SIGNAL(triggered()), this, SLOT(pageSetup()));

	printSelectAction = new QAction(tr("Print Se&lect..."), this);
	connect(printSelectAction, SIGNAL(triggered()), this, SLOT(selectPrintOutput()));

	exitAction = new QAction(tr("E&xit"), this);
	connect(exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

	rewindAction = new QAction(QIcon(":/images/rewind.png"), tr("Re&wind"), this);
	rewindAction->setShortcutContext(Qt::ApplicationShortcut);
	//rewindAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left));
	connect(rewindAction, SIGNAL(triggered()), this, SLOT(rewind()));

	rewindAllAction = new QAction(QIcon(":/images/rewall.png"), tr("Rewind All"), this);
	rewindAllAction->setShortcutContext(Qt::ApplicationShortcut);
	connect(rewindAllAction, SIGNAL(triggered()), this, SLOT(rewindAllSimulations()));

	fastFwdAction = new QAction(QIcon(":/images/ffwd.png"), tr("&Fast Fwd"), this);
	fastFwdAction->setShortcutContext(Qt::ApplicationShortcut);
	//fastFwdAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Right));
	connect(fastFwdAction, SIGNAL(triggered()), this, SLOT(fastFwd()));

	zeroTimerAction = new QAction(QIcon(":/images/timer.png"), tr("&Zero Timer"), this);
	zeroTimerAction->setShortcutContext(Qt::ApplicationShortcut);
	zeroTimerAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
	connect(zeroTimerAction, SIGNAL(triggered()), this, SLOT(zeroTimer()));

	clearAllAction = new QAction(tr("Clear &All"), this);
	connect(clearAllAction, SIGNAL(triggered()), this, SLOT(clearAll()));

	selectAllAction = new QAction(tr("&Select All"), this);
	selectAllAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
	connect(selectAllAction, SIGNAL(triggered()), this, SLOT(selectAll()));

	copySelectionAction = new QAction(QIcon(":/images/cut.png"), tr("Copy Selec&tion"), this);
	copySelectionAction->setShortcutContext(Qt::ApplicationShortcut);
	copySelectionAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
	connect(copySelectionAction, SIGNAL(triggered()), this, SLOT(copySelection()));

	copyDataAction = new QAction(QIcon(":/images/copy.png"), tr("&Copy Data"), this);
	copyDataAction->setShortcutContext(Qt::ApplicationShortcut);
	copyDataAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
	connect(copyDataAction, SIGNAL(triggered()), this, SLOT(copyData()));

	liquidInjectAction = new QAction(QIcon(":/images/srynge_up.png"), tr("&Liquid Inject"), this);
	liquidInjectAction->setShortcutContext(Qt::ApplicationShortcut);
	liquidInjectAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
	connect(liquidInjectAction, SIGNAL(triggered()), this, SLOT(liquidInject()));

	unitDoseAction = new QAction(tr("Unit &Dose..."), this);
	unitDoseAction->setShortcutContext(Qt::ApplicationShortcut);
	unitDoseAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));
	connect(unitDoseAction, SIGNAL(triggered()), this, SLOT(unitDose()));

	setCostAction = new QAction(tr("Set &Cost..."), this);
	connect(setCostAction, SIGNAL(triggered()), this, SLOT(setCost()));

	delAction = new QAction(tr("&FGF, DEL, VA, CO..."), this);
	connect(delAction, SIGNAL(triggered()), this, SLOT(adjustDel()));

	agentAction = new QAction(tr("&Agent and Circuit..."), this);
	connect(agentAction, SIGNAL(triggered()), this, SLOT(changeAgent()));

	defaultAgentColorsAction = new QAction(tr("Default &Colors"), this);
	connect(defaultAgentColorsAction, SIGNAL(triggered()), this, SLOT(defaultAgentColors()));

	showToolbarAction = new QAction(tr("&Toolbar"), this);
	showToolbarAction->setCheckable(true);
	showToolbarAction->setChecked(true);
	connect(showToolbarAction, SIGNAL(toggled(bool)), this, SLOT(setShowToolbar(bool)));

	tileSubWindowsAction = new QAction(tr("&Tile Windows"), this);
	connect(tileSubWindowsAction, SIGNAL(triggered()), mdi, SLOT(tileSubWindows()));

	groupSubWindowsAction = new QAction(tr("&Group Windows"), this);
	connect(groupSubWindowsAction, SIGNAL(triggered()), this, SLOT(groupSubWindows()));

	cascadeSubWindowsAction = new QAction(tr("&Cascade Windows"), this);
	connect(cascadeSubWindowsAction, SIGNAL(triggered()), mdi, SLOT(cascadeSubWindows()));

	pictureOnTopAction = new QAction(tr("Pictures On Top"), this);
	pictureOnTopAction->setShortcutContext(Qt::ApplicationShortcut);
	pictureOnTopAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_P));
	connect(pictureOnTopAction, SIGNAL(triggered()), this, SLOT(pictureOnTop()));

	graphOnTopAction = new QAction(tr("Graphs On Top"), this);
	graphOnTopAction->setShortcutContext(Qt::ApplicationShortcut);
	graphOnTopAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_G));
	connect(graphOnTopAction, SIGNAL(triggered()), this, SLOT(graphOnTop()));

	controlOnTopAction = new QAction(tr("Controls On Top"), this);
	controlOnTopAction->setShortcutContext(Qt::ApplicationShortcut);
	controlOnTopAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_C));
	connect(controlOnTopAction, SIGNAL(triggered()), this, SLOT(controlOnTop()));

	newPanelViewMenu = new QMenu(tr("New &Picture"), this);
	newGraphViewMenu = new QMenu(tr("New &Graph"), this);

	newGraphViewAction = new QAction(tr("New &Graph View"), this);
	connect(newGraphViewAction, SIGNAL(triggered()), this, SLOT(newView()));

	newPanelViewAction = new QAction(tr("New &Picture View"), this);
	connect(newPanelViewAction, SIGNAL(triggered()), this, SLOT(newView()));

	overlayAction = new QAction(QIcon(":/images/overlay.png"), tr("&Overlay"), this);
	connect(overlayAction, SIGNAL(triggered()), this, SLOT(overlay()));

	zoominAction = new QAction(tr("Zoom &in"), this);
	connect(zoominAction, SIGNAL(triggered()), this, SLOT(zoomin()));

	zoomoutAction = new QAction(tr("Zoom o&ut"), this);
	connect(zoomoutAction, SIGNAL(triggered()), this, SLOT(zoomout()));


	disableUptakeAction = new QAction(tr("Disable &Uptake"), this);
	disableUptakeAction->setCheckable(true);
	disableUptakeAction->setShortcutContext(Qt::ApplicationShortcut);
	disableUptakeAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
	connect(disableUptakeAction, SIGNAL(toggled(bool)), this, SLOT(disableUptake(bool)));

	disableReturnAction = new QAction(tr("Disable &Return"), this);
	disableReturnAction->setCheckable(true);
	disableReturnAction->setShortcutContext(Qt::ApplicationShortcut);
	disableReturnAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
	connect(disableReturnAction, SIGNAL(toggled(bool)), this, SLOT(disableReturn(bool)));

	enableVaporAction = new QAction(tr("Enable &Vapor"), this);
	enableVaporAction->setCheckable(true);
	enableVaporAction->setShortcutContext(Qt::ApplicationShortcut);
	enableVaporAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
	connect(enableVaporAction, SIGNAL(toggled(bool)), this, SLOT(enableVapor(bool)));

	changePatientAction = new QAction(QIcon(":/images/patient.png"), tr("Change &Patient..."), this);
	connect(changePatientAction, SIGNAL(triggered()), this, SLOT(changePatient()));

	setBookmarkAction = new QAction(QIcon(":/images/bookmark.png"), tr("Set &Bookmark..."), this);
	connect(setBookmarkAction, SIGNAL(triggered()), this, SLOT(setBookmark()));

	setBreakPointAction = new QAction(tr("Set Brea&kPoint"), this);
	//setBreakPointAction->setShortcut(tr("Ctrl+B"));
	setBreakPointAction->setVisible(false);
	connect(setBreakPointAction, SIGNAL(triggered()), this, SLOT(setBreakPoint()));

	enableScrollingAction = new QAction(tr("Enable &Scrolling"), this);
	enableScrollingAction->setCheckable(true);
	connect(enableScrollingAction, SIGNAL(toggled(bool)), this, SLOT(enableScrolling(bool)));

	hideNumericsAction = new QAction(tr("&Hide Numerics"), this);
	hideNumericsAction->setCheckable(true);
	connect(hideNumericsAction, SIGNAL(toggled(bool)), this, SLOT(hideNumerics(bool)));

	showCostAction = new QAction(QIcon(":/images/cost.png"), tr("Show &Cost"), this);
	showCostAction->setCheckable(true);
	connect(showCostAction, SIGNAL(toggled(bool)), this, SLOT(showCost(bool)));

	showMlAction = new QAction(QIcon(":/images/bottle.png"), tr("Show mL"), this);
	showMlAction->setCheckable(true);
	connect(showMlAction, SIGNAL(toggled(bool)), this, SLOT(showMl(bool)));

	helpTopicAction = new QAction(QIcon(":/images/help.png"), tr("&Help Topic"), this);
	helpTopicAction->setShortcutContext(Qt::ApplicationShortcut);
	helpTopicAction->setShortcut(QKeySequence(Qt::Key_F1));
	connect(helpTopicAction, SIGNAL(triggered()), this, SLOT(showHelpTopic()));

	helpWorkbookAction = new QAction(QIcon(":/images/help.png"), tr("Online Workbook"), this);
	helpWorkbookAction->setShortcutContext(Qt::ApplicationShortcut);
	connect(helpWorkbookAction, SIGNAL(triggered()), this, SLOT(showWorkbook()));

	registerAction = new QAction(tr("&Register product..."), this);
	connect(registerAction, SIGNAL(triggered()), this, SLOT(registerProduct()));

	aboutGasManAction = new QAction(QIcon(":/images/gasman.png"), tr("&About Gas Man\xC2\xAE..."), this);
	connect(aboutGasManAction, SIGNAL(triggered()), this, SLOT(aboutGasMan()));

	editDescriptionAction = new QAction(tr("Simulation Description"), this);
	connect(editDescriptionAction, SIGNAL(triggered()), this, SLOT(editDescription()));
	//editDescriptionAction->setShortcut( tr("Ctrl+D") );

	optionsAction = new QAction(QIcon(":/images/options.png"), tr("&Options..."), this);
	connect(optionsAction, SIGNAL(triggered()), this, SLOT(setDefaultOptions()));

	helpAction = new QAction(QIcon(":/images/whatsthis.png"), tr("Help"), this);
	helpAction->setShortcutContext(Qt::ApplicationShortcut);
	helpAction->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F1));
	//connect( helpAction, SIGNAL( triggered() ), this, SLOT( showHelp() ) );

	QShortcut * sc = new QShortcut(this);
	sc->setAutoRepeat(false);
	sc->setKey(QKeySequence(Qt::CTRL + Qt::Key_B));
	connect(sc, SIGNAL(activated()), this, SLOT(setBreakPoint()));

}

//Create all menus
void GasMainWindow::createMenus()
{
	QMenuBar *menubar = 0;
#if defined(Q_OS_MACX)
	/*
	* Parenting the menubar to 0 upon creation on the Mac will make this
	* menu bar serve as the system-wide menu bar for Gas Man, regardless of
	* which Gas Man window has focus.
	*/
	menubar = new QMenuBar(0);
#else
	menubar = new QMenuBar(this);
#endif
	QMenu *fileMenu = new QMenu(tr("&File"), menubar);
	menuExport_to = new QMenu(tr("Export to..."), fileMenu);
	openRecentMenu = new QMenu(tr("Open Recent"), fileMenu);
	openRecentMenu->setIcon(QIcon(":/images/open.png"));

	fileMenu->addAction(newDocumentAction);
	fileMenu->addAction(openDocumentAction);
	fileMenu->addAction(openRecentMenu->menuAction());
	fileMenu->addAction(saveDocumentAction);
	fileMenu->addAction(saveDocumentAsAction);
	fileMenu->addAction(closeDocumentAction);
	fileMenu->addAction(closeAllDocumentsAction);
	fileMenu->addSeparator();
	fileMenu->addAction(runAction);
	fileMenu->addAction(runAllAction);
	fileMenu->addSeparator();
	fileMenu->addAction(printAction);

	fileMenu->addAction(printPreviewAction);

	fileMenu->addAction(pageSetupAction);
	fileMenu->addAction(printSelectAction);
	fileMenu->addAction(menuExport_to->menuAction());
	fileMenu->addSeparator();
	fileMenu->addAction(editDescriptionAction);
	fileMenu->addSeparator();
#ifndef Q_OS_MACX
	fileMenu->addAction(sendAction);
#endif //Q_OS_MACX
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	menuExport_to->addAction(pdfAction);
	menuExport_to->addAction(xmlAction);
	menuExport_to->addAction(htmlAction);
	menuExport_to->addAction(transformAction);

	for (int i = 0; i < MaxRecentFiles; ++i) {
		recentFileActs[i] = new QAction(this);
		recentFileActs[i]->setVisible(false);
		connect(recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
	}

	createRecentMenu();

	connect(fileMenu, SIGNAL(aboutToShow()), this, SLOT(updateFileMenu()));

	editMenu = new QMenu(tr("&Edit"), menubar);
	editMenu->addAction(rewindAction);
	editMenu->addAction(fastFwdAction);
	editMenu->addAction(zeroTimerAction);
	editMenu->addAction(clearAllAction);
	editMenu->addSeparator();
	editMenu->addAction(selectAllAction);
	editMenu->addAction(copySelectionAction);
	editMenu->addAction(copyDataAction);

	connect(editMenu, SIGNAL(aboutToShow()), this, SLOT(updateEditMenu()));

	viewMenu = new QMenu(tr("&View"), menubar);
	windowMenu = new QMenu(tr("&Window"), menubar);
	windowMapper = new QSignalMapper(this);
	createViewMenu();
	createWindowMenu();
	connect(windowMapper, SIGNAL(mapped(QWidget *)), this, SLOT(setActiveWindow(QWidget *)));
	connect(viewMenu, SIGNAL(aboutToShow()), this, SLOT(updateViewMenu()));
	connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(createWindowMenu()));

	specialMenu = new QMenu(tr("&Special"), menubar);
	specialMenu->addAction(disableUptakeAction);
	specialMenu->addAction(disableReturnAction);
	specialMenu->addAction(enableVaporAction);
	specialMenu->addAction(changePatientAction);
	specialMenu->addAction(setBookmarkAction);
	specialMenu->addAction(setBreakPointAction);
	specialMenu->addAction(enableScrollingAction);
	specialMenu->addSeparator();
	specialMenu->addAction(hideNumericsAction);
	specialMenu->addAction(showCostAction);
	specialMenu->addAction(showMlAction);
	connect(specialMenu, SIGNAL(aboutToShow()), this, SLOT(updateSpecialMenu()));

	QMenu *helpMenu = new QMenu(tr("&Help"), menubar);
	helpMenu->addAction(helpTopicAction);
	helpMenu->addAction(helpWorkbookAction);
	helpMenu->addAction(registerAction);
	helpMenu->addAction(aboutGasManAction);

	QMenu *toolsMenu = new QMenu(tr("&Tools"), menubar);
	toolsMenu->addAction(optionsAction);
	//languageMenu = new QMenu( tr( "Language" ), toolsMenu );
	//createLanguageMenu();
	//toolsMenu->addAction( languageMenu->menuAction() );

	anesthesiaMenu = new QMenu(tr("&Anesthesia"), menubar);
	anesthesiaMenu->addAction(liquidInjectAction);
	anesthesiaMenu->addAction(unitDoseAction);
	anesthesiaMenu->addSeparator();
	anesthesiaMenu->addAction(setCostAction);
	anesthesiaMenu->addAction(delAction);
	anesthesiaMenu->addAction(agentAction);
	anesthesiaMenu->addAction(defaultAgentColorsAction);
	connect(anesthesiaMenu, SIGNAL(aboutToShow()), this, SLOT(updateAnesthesiaMenu()));

	//menubar->addMenu(fileMenu);
	menubar->addAction(fileMenu->menuAction());


	menubar->addAction(editMenu->menuAction());
	menubar->addAction(anesthesiaMenu->menuAction());
	menubar->addAction(viewMenu->menuAction());
	menubar->addAction(specialMenu->menuAction());
	menubar->addAction(toolsMenu->menuAction());
	menubar->addAction(windowMenu->menuAction());
	menubar->addAction(helpMenu->menuAction());

	setMenuBar(menubar);
}

void GasMainWindow::updateActiveGas()
{
	// handle gas control
	int nGas;
	GasChildWindow* actvChild = activeChild();
	if (actvChild && !actvChild->doc()->m_bClosed && actvChild->tryGetGas(nGas))
		actvChild->doc()->m_nActiveGas = nGas;
}

void GasMainWindow::registerProduct()
{
	GasRegisterDlg dlg;
	dlg.exec();
}

// update GasDoc's m_anesArray when valid license registered
// THIS IS A BAD IDEA. THESE DOCS POINT INTO m_anesArray
void GasMainWindow::updateGasDocs()
{/*
 foreach(GasDoc* doc, docs)
 {
 doc->m_anesArray.ReadProfile();
 }
 */}

void GasMainWindow::groupSubWindows()
{
	setUpdatesEnabled(false);

	foreach(QMdiSubWindow* subWin, mdi->subWindowList())
		subWin->hide();

	foreach(GasDoc* doc, docs)
	{

		GasTabWidget *dest = newTabWidget(doc);

		foreach(GasChildWindow* child, doc->gasChildren())
			reparentChild(child, qobject_cast<GasTabWidget*>(child->parentWidget()->parentWidget()), dest);
		mdi->addSubWindow(dest);
		dest->show();
		setActiveWindow(doc->gasChildren().first());

	}
	setUpdatesEnabled(true);
}

void GasMainWindow::pictureOnTop() { onTop(PANEL); }
void GasMainWindow::graphOnTop() { onTop(GRAPH); }
void GasMainWindow::controlOnTop() { onTop(CONTROL); }
void GasMainWindow::onTop(const GasViewType viewType)
{
	foreach(GasDoc* doc, docs) {
		GasChildren children = doc->gasChildren();
		bool found = false;
		foreach(GasChildWindow * child, children) {
			if (!found && child->getViewType() == viewType) {
				GasTabWidget* tabWidget = qobject_cast<GasTabWidget*>(child->parentWidget()->parentWidget());
				if (tabWidget && (tabWidget->indexOf(child) != -1)) {
					found = true;
					tabWidget->setCurrentWidget(child);
				}
			}
		}
	}
}

void GasMainWindow::tabifyChild(GasChildWindow* child)
{
	GasTabWidget* tw = 0;

	// Find the tab widget to add this child to if it's not the control tab
	if (child->getViewType() != CONTROL)
	{
		// Should be the currently active child
		if (activeChildForced()) // may have side effect of message pumping
		{
			tw = qobject_cast<GasTabWidget*>(mdi->activeSubWindow()->widget());
			// ...as long as it refers to the correct simulation
			if (qobject_cast<GasChildWindow*>(tw->widget(0))->doc() != child->doc())
				tw = 0;
		}
		if (!tw)
		{
			// New view without active window? Weird (like surface pro?). Find most recently active
			QList<QMdiSubWindow *> sws = mdi->subWindowList(QMdiArea::ActivationHistoryOrder);
			while (!tw && !sws.isEmpty())
			{
				tw = qobject_cast<GasTabWidget*>(sws.takeLast()->widget());
				if (qobject_cast<GasChildWindow*>(tw->widget(0))->doc() != child->doc())
					tw = 0;
			}
		}
	}

	if (tw)
	{
		tw->setUpdatesEnabled(false);
		tw->addTab(child, child->title());
	} else {
		tw = newTabWidget(child->doc());
		mdi->addSubWindow(tw);
		tw->addTab(child, child->title());
		tw->show();
	}
	tw->setUpdatesEnabled(true);
}

GasTabWidget* GasMainWindow::newTabWidget(GasDoc* doc)
{
	GasTabWidget* tabWidget = new GasTabWidget(this);
	//tabWidget->setToolTip("Click/drag this tab away to view content separately or to move it to another window.");
	tabWidget->setAttribute(Qt::WA_DeleteOnClose);
	tabWidget->setWindowTitle(doc->title());
	connect(doc, SIGNAL(titleChanged(QString)),
		tabWidget, SLOT(setWindowTitle(QString)));
	connect(doc, SIGNAL(agentColorsChanged()),
		tabWidget, SLOT(updateTabTextColor()));
	connect(tabWidget, SIGNAL(currentChanged(int)),
		this, SLOT(updateAllActions()));
	connect(tabWidget, SIGNAL(reparentChildDrop(QDropEvent*, GasTabWidget*)),
		this, SLOT(reparentChildDrop(QDropEvent*, GasTabWidget*)));
	return tabWidget;
}

void GasMainWindow::newViewOrAdd()
{
	int i;
	QAction* action = qobject_cast<QAction*>(sender());
	QMenu *actionParent = qobject_cast<QMenu*>(action->parent());
	QString actionText = action->text();
	GasDoc* doc = activeChildForced()->doc();
	QString t = actionText;
	if (actionText.contains(tr("(Add) "))) // adding an agent
	{
		if (!doc->CheckChange(true))
			return;
		QString agent = actionText.remove(tr("(Add) ")); // extract agent name
		doc->AddAgent(agent);
	}
	else // just update the active gas
		for (i = 0; i < doc->m_gasArray.size(); i++)
			if (actionText == doc->GetAgent(i)->m_strName) {
				doc->m_nActiveGas = i;
				break;
			}

	// create the new view
	if (actionParent == newGraphViewMenu)
		newView(doc, doc->m_nActiveGas, GRAPH);
	if (actionParent == newPanelViewMenu)
		newView(doc, doc->m_nActiveGas, PANEL);
}

void GasMainWindow::zoomin()
{
	int i = activeChild()->graphView()->getZoom();
	i++;
	activeChild()->graphView()->setZoom(i);
}
void GasMainWindow::zoomout()
{
	int i = activeChild()->graphView()->getZoom();
	i--;
	activeChild()->graphView()->setZoom(i);
}

void GasMainWindow::windowStateChanged()
{

	QList<QMdiSubWindow *> sws = mdi->subWindowList();
	foreach(QMdiSubWindow * w, sws) {
		disconnect(w, SIGNAL(windowStateChanged(Qt::WindowStates, Qt::WindowStates)), this, SLOT(updateViewMenu()));
		connect(w, SIGNAL(windowStateChanged(Qt::WindowStates, Qt::WindowStates)), this, SLOT(updateViewMenu()));

	}
}

QList<QMdiSubWindow *> GasMainWindow::getSubWins()
{
	return mdi->subWindowList();
}

void GasMainWindow::setOpenDir(const QString& dir)
{
	openDir = dir;
	gasApp->WriteProfile("Defaults", "OpenDir", dir);
}

void GasMainWindow::setSaveDir(const QString& dir)
{
	saveDir = dir;
	gasApp->WriteProfile("Defaults", "SaveDir", dir);
}

void GasMainWindow::setTransDir(const QString& dir)
{
	transDir = dir;
	gasApp->WriteProfile("Defaults", "TransDir", dir);
}
