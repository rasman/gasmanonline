#include "gascontrol.h"

#include "gaschildwindow.h"
#include "gaspanelview.h"
#include "gasgraphview.h"
#include "gasagentdlg.h"
#include "gasdoc.h"
#include "gasglobal.h"
#include "gasapplication.h"
#include "gasagentdelegate.h"
#include "gaslicensemanager.h"
#include "gasmainwindow.h"
#if QT_VERSION >=0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

GasControl::GasControl( GasDoc* gasDoc, GasChildWindow* childWindow, QWidget *parent )
	: GasViewCommon( childWindow, parent ), doc( gasDoc )
{
	setupUi( this );
	 
	// potential Qt bug workaround
	// disables transparent background
	// keeps other tabs from showing through on MDI resize
	setAutoFillBackground(true);

	setWindowTitle(doc->title()+"[*]");

	GasAgentDelegate* agents = new GasAgentDelegate(GasAgentDelegate::anestheticNameColumnType, this);
	GasAgentDelegate* colors = new GasAgentDelegate(GasAgentDelegate::anestheticColorColumnType, this);

	agentTableWidget->setItemDelegateForColumn(0, agents);
	agentTableWidget->setItemDelegateForColumn(1, colors);

	connect(agentTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateAgentWidget(QTableWidgetItem*))); 

	weightDoubleSpinBox->setRange( MIN_WEIGHT, MAX_WEIGHT );
	weightDoubleSpinBox->setValue( STD_WEIGHT );
	weightDoubleSpinBox->setDecimals( 2 );
	
	circuitComboBox->addItems( listCircuit );
	speedComboBox->addItems( listSpeed );

	newGraphViewAction = new QAction( tr( "New &Graph View" ), agentTableWidget );
	newPanelViewAction = new QAction( tr( "New &Picture View" ), agentTableWidget );
	deleteAgentAction = new QAction(tr("&Delete Agent"), agentTableWidget);
	bringToFrontAction = new QAction(tr("&Bring To Front"), agentTableWidget);
	editCostAction = new QAction(tr("&Set Cost"), agentTableWidget);

	connect( newGraphViewAction, SIGNAL( triggered() ), this, SLOT( newViewRequested() ) );
	connect( newPanelViewAction, SIGNAL( triggered() ), this, SLOT( newViewRequested() ) );
	connect( deleteAgentAction, SIGNAL( triggered() ), this, SLOT( deleteAgentRequested() ) );
	connect( bringToFrontAction, SIGNAL( triggered() ), this, SLOT( bringToFrontRequested() ) );

	agentTableWidget->addAction(newGraphViewAction);
	agentTableWidget->addAction(newPanelViewAction);
	agentTableWidget->addAction(deleteAgentAction);
	agentTableWidget->addAction(bringToFrontAction);
	agentTableWidget->addAction(editCostAction);

	connect( agentTableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(bringToFrontRequested()));
	connect(agentTableWidget,SIGNAL(currentCellChanged(int,int, int ,int)),this,SLOT(onSelectChanged(int,int,int,int)));
	connect(editCostAction, SIGNAL(triggered()),this, SLOT(setCost()));

	enVapRadioButton->setChecked(doc->isEnabledVapor());
	connect(enVapRadioButton, SIGNAL(toggled(bool)), gasMainWindow->enableVaporAction, SIGNAL(toggled(bool)));

	disRetRadioButton->setChecked(doc->isDisabledReturn());
	connect(disRetRadioButton, SIGNAL(toggled(bool)), gasMainWindow->disableReturnAction, SIGNAL(toggled(bool)));

	disUpRadioButton->setChecked(doc->isDisabledUptake());
	connect(disUpRadioButton, SIGNAL(toggled(bool)), gasMainWindow->disableUptakeAction, SIGNAL(toggled(bool)));

	updateControlVals();

	connect(upPushButton, SIGNAL(clicked()), this, SLOT(moveUpRequested()));
	connect(downPushButton, SIGNAL(clicked()), this, SLOT(moveDownRequested()));

	connect(weightDoubleSpinBox, SIGNAL(editingFinished()), 
					this, SLOT(updateWeight()));
	connect(circuitComboBox, SIGNAL(currentIndexChanged(const QString&)), 
					this, SLOT(updateCircuit(const QString&)));

	// the view control here is of no use;
	viewComboBox->setVisible(false);
	label_3->setVisible(false);
 
	connect(speedComboBox, SIGNAL(currentIndexChanged(const QString&)), 
					this, SLOT(updateSpeed(const QString&)));
	 
	connect(runPushButton, SIGNAL(clicked()),
					this, SLOT(runSim()));
	connect(afapPushButton, SIGNAL(clicked()),
					this, SLOT(onAFAPButton()));
	//don't want it, hide first, need furthur clean 
	afapPushButton->setVisible(false);

	connect(doc, SIGNAL(stateChanged(AState)), this, SLOT(setState(AState)));
	connect(doc, SIGNAL(modified(bool)), this, SLOT(setWindowModified(bool)));
	connect(doc, SIGNAL(modified(bool)), this, SLOT(updateControlVals()));
	connect(doc, SIGNAL(updateControl()), this, SLOT(updateControlVals()));
	connect(doc, SIGNAL(allCleared()), this, SLOT(clearAllRequested()));
	connect(doc, SIGNAL(agentChanged(int,const QString&)), this, SLOT(changeAgent(int, const QString&)));
	connect(doc, SIGNAL(agentColorsChanged()), this, SLOT(agentColorsChanged()));
	
#if QT_VERSION >=0x050000
	agentTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
#else
	agentTableWidget->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
#endif
	agentTableWidget->horizontalHeader()->setStretchLastSection(false);

	agentTableWidget->installEventFilter(this);
}
 
GasControl::~GasControl()
{
	if (doc)
		doc->close();
}

 
void GasControl::clearAllRequested()
{
	agentTableWidget->clearContents();
	agentTableWidget->setRowCount(1);
	agentTableWidget->setColumnCount(2);
	for ( int i = 0; i < doc->m_gasArray.size(); i++){
		addAgentToWidget(i);
	}
}

void GasControl::updateWeight()
{
	double weight = weightDoubleSpinBox->value();
	GasChildWindow* child = doc->gasChildren().first();
	child->panelView()->ChangeWeight(weight);
}

void GasControl::updateCircuit(const QString  &circuit)
{
	GasChildWindow* child = doc->gasChildren().first();
	child->panelView()->ChangeCircuit(circuit);
}

void GasControl::updateSpeed(const QString &speed)
{
	
	GasChildWindow* child = doc->gasChildren().first();
	child->panelView()->ChangeSpeed(speed);
}
 

void GasControl::runSim()
{
	// ok to run?
//	changeSpeed = false;
	doc->gasChildren().first()->panelView()->run();

}

//run as fast as possible regardless the speed setting.
void GasControl::onAFAPButton()
{
 
 
}

bool GasControl::tryCloseAll()
{
	if (doc)
		return doc->close();
	else
		return true;
}

void GasControl::updateControlVals()
{
	setWeight(doc->GetWeight());
	disconnect(circuitComboBox, SIGNAL(currentIndexChanged(const QString&)), 
					this, SLOT(updateCircuit(const QString&)));
	setCircuit(doc->GetCircuit());
	connect(circuitComboBox, SIGNAL(currentIndexChanged(const QString&)), 
					this, SLOT(updateCircuit(const QString&)));

	int nSpeed = doc->GetSpeed();
	QString m_strSpeed;
	if ( nSpeed !=AFAP )
		m_strSpeed = QString( "%1x" ).arg( nSpeed );
	else
		m_strSpeed = tr( "AFAP" );
	 
	disconnect(speedComboBox, SIGNAL(currentIndexChanged(const QString&)), 
					this, SLOT(updateSpeed(const QString&)));
	setSpeed(m_strSpeed);
	 
	connect(speedComboBox, SIGNAL(currentIndexChanged(const QString&)), 
					this, SLOT(updateSpeed(const QString&)));
	setState(doc->GetState());

	for(int i = 0; i < doc->colors.size(); i++)
	{
		if(!agentTableWidget->item(i, 1) || agentTableWidget->item(i, 1)->text() != doc->colors[i])
		{
			QTableWidgetItem* colorItem = new QTableWidgetItem(doc->colors[i]);
			agentTableWidget->setItem(i, 1, colorItem);
		}
	}
	tryAddRow();

	disconnect(enVapRadioButton, SIGNAL(toggled(bool)), gasMainWindow->enableVaporAction, SIGNAL(toggled(bool)));
	enVapRadioButton->setChecked(doc->isEnabledVapor());
	connect(enVapRadioButton, SIGNAL(toggled(bool)), gasMainWindow->enableVaporAction, SIGNAL(toggled(bool)));

	disconnect(disRetRadioButton, SIGNAL(toggled(bool)), gasMainWindow->disableReturnAction, SIGNAL(toggled(bool)));
	disRetRadioButton->setChecked(doc->isDisabledReturn());
	connect(disRetRadioButton, SIGNAL(toggled(bool)), gasMainWindow->disableReturnAction, SIGNAL(toggled(bool)));

	disconnect(disUpRadioButton, SIGNAL(toggled(bool)), gasMainWindow->disableUptakeAction, SIGNAL(toggled(bool)));
	disUpRadioButton->setChecked(doc->isDisabledUptake());
	connect(disUpRadioButton, SIGNAL(toggled(bool)), gasMainWindow->disableUptakeAction, SIGNAL(toggled(bool)));
}

void GasControl::setWeight(double weight)
{
	weightDoubleSpinBox->setValue(weight);
}

void GasControl::setCircuit(const QString& circuit)
{
	int index = circuitComboBox->findText( circuit, Qt::MatchFixedString );
	if ( index != -1 )
		circuitComboBox->setCurrentIndex( index );
}

void GasControl::setSpeed(const QString& speed)
{
	int index = speedComboBox->findText( speed, Qt::MatchFixedString );
	if ( index != -1 )
		speedComboBox->setCurrentIndex( index );
}

void GasControl::setUpAddAgentText()
{
	disconnect(agentTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateAgentWidget(QTableWidgetItem*))); 

	QTableWidgetItem* item = new QTableWidgetItem(tr("Click to Add"));
	agentTableWidget->setItem(agentTableWidget->rowCount()-1, 0, item);

	QTableWidgetItem* item2 = new QTableWidgetItem("");
	agentTableWidget->setVerticalHeaderItem(agentTableWidget->rowCount()-1, item2);

	connect(agentTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateAgentWidget(QTableWidgetItem*))); 
}

// if "add agent" row is not in the table, add it
void GasControl::tryAddRow()
{
	if(agentTableWidget->rowCount() <= doc->m_gasArray.size()){
		QTableWidgetItem* headerItem = new QTableWidgetItem(QString::number(agentTableWidget->rowCount()));
		agentTableWidget->setVerticalHeaderItem(agentTableWidget->rowCount()-1, headerItem);
		agentTableWidget->setRowCount(agentTableWidget->rowCount()+1);
		setUpAddAgentText();
	}
}

void GasControl::updateAgentWidgetState(bool enableChanges)
{
	disconnect(agentTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateAgentWidget(QTableWidgetItem*))); 

	Qt::ItemFlags agentFlags = Qt::ItemIsSelectable;
	if(enableChanges)
	{
		agentFlags |= Qt::ItemIsEditable | Qt::ItemIsEnabled;
		agentTableWidget->showRow(agentTableWidget->rowCount()-1);
	}else{
		agentTableWidget->closePersistentEditor(agentTableWidget->currentItem());
		// hide "add agent" row from table widget
		if(agentTableWidget->rowCount() > doc->m_gasArray.size())
			agentTableWidget->hideRow(agentTableWidget->rowCount()-1);
	}
	for(int i = 0; i < agentTableWidget->rowCount()-1; i++)
	{
		agentTableWidget->item(i, 0)->setFlags(agentFlags);
	}

	// toggle whether patient weight is changable
	weightDoubleSpinBox->setEnabled(enableChanges);

	// toggle whether re-ordering is allowed
	upPushButton->setEnabled(enableChanges);
	downPushButton->setEnabled(enableChanges);

	// toggle whether delete agent action is available
	if(deleteAgentAction)
		deleteAgentAction->setEnabled(enableChanges);

	// Make sure we have a current row
	if ( agentTableWidget->currentRow() < 0 )
	{
		agentTableWidget->scrollToTop();
		agentTableWidget->setCurrentCell(0,0);
	}

	connect(agentTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateAgentWidget(QTableWidgetItem*))); 
}
	

void GasControl::setState( const AState &theState )
{
	switch ( theState )
	{
	case STOPPED_STATE:
		runPushButton->setText( tr( "Continue" ) );
		updateAgentWidgetState( doc->GetTime() == 0.0F );
		break;
	case RUNNING_STATE:
		runPushButton->setText( tr( "Pause" ) );
		updateAgentWidgetState(false);
		break;
	case INITIAL_STATE:
		runPushButton->setText( tr( "Begin" ) );
		runPushButton->setEnabled( true );
		updateAgentWidgetState(true);
		break;
	case END_STATE:
		runPushButton->setText( tr( "End" ) );
		runPushButton->setEnabled( false );
		updateAgentWidgetState(false);
		break;
	default:
		Q_ASSERT( false );
	}
}

void GasControl::setTitle(const QString& title)
{
	setWindowTitle(title + "[*]");
}


void GasControl::pauseSim()
{
	if ( doc->GetState() == RUNNING_STATE ) {
		if(doc->gasChildren().size())
		{
			doc->gasChildren().first()->panelView()->StopTicker();
		}
		doc->ChangeState( this, STOPPED_STATE );
		setState( STOPPED_STATE );
	}
}


QStringList GasControl::getAllAgents()
{
	if(!glm->instance()->validLicenseExists())
		return AvailableAgents;
	else
		return gasApp->getAgents();
}

QStringList GasControl::getAllColors()
{
	return gasApp->getColors();

}

void GasControl::changeAgent(int nGas, const QString& agent)
{
	if (agentTableWidget->rowCount() > nGas)
	{
		disconnect(agentTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateAgentWidget(QTableWidgetItem*))); 
		QTableWidgetItem* tmp = agentTableWidget->item(nGas, 0);
		tmp->setText(agent);
		connect(agentTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateAgentWidget(QTableWidgetItem*))); 
	}
}

void GasControl::agentColorsChanged()
{
	for(int i = 0; i < doc->colors.size(); i++)
	{
		if(!agentTableWidget->item(i, 1) || agentTableWidget->item(i, 1)->text() != doc->colors[i])
		{
			QTableWidgetItem* colorItem = new QTableWidgetItem(doc->colors[i]);
			agentTableWidget->setItem(i, 1, colorItem);
		}
	}
}

//void GasControl::changeActiveAgent(const QString& agent)
//{
//	GasChildren children = doc->gasChildren();
//	GasChildren tmp;
//
//	foreach(GasChildWindow* child, children)
//	{
//		if(child->graphView()->m_strAgent == agent)
//			tmp.append(child);
//	}
//
//	emit activateChildWindows(tmp);
//}

void GasControl::removeAgent(int gasNum)
{
	 
	if(doc->m_gasArray.size() == 1)
	{
		doc->close();
		return;
	}

	if(!doc->CheckChange(true))
	{
		setUpAddAgentText();
		return;
	}

	foreach(GasChildWindow* child, doc->DeleteAgent(gasNum))
	{
		emit removeChildWindow(child);
	}

	agentTableWidget->removeRow(gasNum);
	 
	// update row header numbers
	for(int i = 0; i < agentTableWidget->rowCount()-1; i++)
	{
		QTableWidgetItem* headerItem = new QTableWidgetItem(QString::number(i+1));
		agentTableWidget->setVerticalHeaderItem(i, headerItem);
	}
}

bool GasControl::tryGetGas(int &row)
{
    row = agentTableWidget->currentRow();
    return row >=0 && row < agentTableWidget->rowCount()-1;
}

bool GasControl::tryGetCurrentRow(int &row)
{
    bool ret = tryGetGas(row);
    if (ret)
        doc->m_nActiveGas = row;
    return ret;
}


// find out which QComboBox had a context menu requested, reset the active 
// gas in the gasdoc, and emit to main window to create a new view
void GasControl::newViewRequested()
{
	int gasNum;
	if ( tryGetCurrentRow(gasNum) )
	{
		if(sender() == newGraphViewAction)
			emit addAgentView(doc, gasNum, GRAPH);
		else
			emit addAgentView(doc, gasNum, PANEL);
	}
}

void GasControl::deleteAgentRequested()
{	
	int gasNum;
	if ( tryGetCurrentRow(gasNum) )
		removeAgent(gasNum);
}

void GasControl::bringToFrontRequested()
{
	int gasNum;
	if ( !tryGetCurrentRow(gasNum) )
		return;

	QWidget* myTab = theChild->parentWidget();
	foreach(GasChildWindow* child, doc->gasChildren()){
		int nGas;
		if(child->getViewType() != CONTROL && child->tryGetGas(nGas) && nGas == gasNum && child->parentWidget() != myTab)
			emit activateChild(child);
	}
}
void GasControl::setCost()
{
	int gasNum;
	if ( tryGetCurrentRow(gasNum) )
		doc->setCost();
}

void GasControl::updateAgentWidget(QTableWidgetItem* item)
{
	int itemRow = item->row();

	if(item->column()==0) // if an agent
	{
		if(getAllAgents().contains(item->text())) // valid agent selected?
		{
			if(itemRow == agentTableWidget->rowCount()-1) // adding an agent
			{
				if(!doc->CheckChange(true))
				{
					setUpAddAgentText();
					return;
				}
				doc->AddAgent(item->text());
			 	emit addAgentView(doc, itemRow, PANEL );
				gasApp->processEvents();
		 		emit addAgentView(doc, itemRow, GRAPH);

			}else{ // changing an existing agent				
				if(!doc->CheckChange(true))
				{
					// revert the change since user refused!
					changeAgent(itemRow, doc->GetAgent(itemRow)->m_strName);
					return;
				}
				doc->SetAgent((QWidget *)-1L, itemRow, item->text());
				doc->setColorToDefault(itemRow);
				doc->updateTitle();
			}
		}else{ // reset text
			setUpAddAgentText();
		}
	}else{ // color change
		int i = itemRow;
		bool inRange = 0<= i && i < doc->colors.size();
		Q_ASSERT( inRange );
		if ( inRange )
			doc->changeColor(i, item->text());
	}
}

void GasControl::initialAgentsUpdate()
{
	for ( int i = 0; i < doc->m_gasArray.size(); i++){
		addAgentToWidget(i);
		emit addAgentView(doc, i, PANEL);
		gasApp->processEvents();
		emit addAgentView(doc, i, GRAPH);
	}
}

void GasControl::swapRows(int old, int notSoOld, QTableWidgetItem* item)
{
	if(!doc->CheckChange(true))
		return;

    doc->m_gasArray.swapItemsAt(old, notSoOld); // was previously  m_gasArray.swap(old, notSoOld)
    doc->colorSwap(old, notSoOld);

	disconnect(agentTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateAgentWidget(QTableWidgetItem*))); 

	QTableWidgetItem* oldAgent = agentTableWidget->takeItem(old, 0);
	QTableWidgetItem* oldColor = agentTableWidget->takeItem(old, 1);

	QTableWidgetItem* newAgent = agentTableWidget->takeItem(notSoOld, 0);
	QTableWidgetItem* newColor = agentTableWidget->takeItem(notSoOld, 1);
	
	agentTableWidget->setItem(old, 0, newAgent);
	agentTableWidget->setItem(old, 1, newColor);

	agentTableWidget->setItem(notSoOld, 0, oldAgent);
	agentTableWidget->setItem(notSoOld, 1, oldColor);
	
	agentTableWidget->setCurrentItem(item);

	doc->UpdateAllViews();
	doc->updateTitle();

	connect(agentTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateAgentWidget(QTableWidgetItem*))); 
}

void GasControl::moveUpRequested()
{
	QTableWidgetItem* item = agentTableWidget->currentItem();
	if(item && (item->row()!=0) && (item->row()!=agentTableWidget->rowCount()-1))
		swapRows(item->row()-1, item->row(), item);
}

void GasControl::moveDownRequested()
{
	QTableWidgetItem* item = agentTableWidget->currentItem();
	if(item && (item->row()<agentTableWidget->rowCount()-2)) // not empty or not last gas
		swapRows(item->row()+1, item->row(), item);
}

void GasControl::addAgentToWidget(const int nGas)
{
	GasAnesthetic *gas = doc->GetAgent(nGas);

	tryAddRow();

	// make sure when setting items that update agent widget does not handle them
	// since we are only initializing
	disconnect(agentTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateAgentWidget(QTableWidgetItem*)));

	QTableWidgetItem* item = new QTableWidgetItem(gas->m_strName);
	agentTableWidget->setItem(nGas, 0, item);

	QTableWidgetItem* item2 = new QTableWidgetItem(doc->colors[nGas]);
	agentTableWidget->setItem(nGas, 1, item2);

	doc->updateTitle();
	connect(agentTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateAgentWidget(QTableWidgetItem*))); 
}

void GasControl::setAgent(int nGas, const QString& agent)
{
	QTableWidgetItem* item = new QTableWidgetItem(agent);
	agentTableWidget->setItem(nGas, 0, item);
}

void GasControl::onSelectChanged(int currentRow, int,int ,int)
{
	// Set the context menu function according to the selected row.
	if( currentRow < 0 || currentRow >= doc->m_gasArray.size() )
		agentTableWidget->setContextMenuPolicy(Qt::PreventContextMenu);
	else
		agentTableWidget->setContextMenuPolicy(Qt::DefaultContextMenu);
}

bool GasControl::eventFilter(QObject *obj, QEvent *evnt)
{
	// If we see a context menu request for the agent table...
	if( obj == agentTableWidget && evnt->type() == QEvent::ContextMenu )
	{
		QContextMenuEvent *qcme = static_cast<QContextMenuEvent *>(evnt);

		// If we clicked the current row...
		int currentRow = agentTableWidget->currentRow();
		int zero = agentTableWidget->rowViewportPosition(1);
		int clickedRow = agentTableWidget->rowAt( qcme->y() - zero );
		if( currentRow >= 0 && currentRow < doc->m_gasArray.size() && currentRow == clickedRow )
		{
			QWidget* myTab = theChild->parentWidget();
			bool enb = false;
			foreach(GasChildWindow* child, doc->gasChildren()){
				int nGas;
				if(child->getViewType() != CONTROL && child->tryGetGas(nGas) && nGas == currentRow && child->parentWidget() != myTab)
				{
					enb=true;
					break;
				}
			}
			bringToFrontAction->setEnabled(enb);
			QMenu::exec(agentTableWidget->actions(), qcme->globalPos(), 0, agentTableWidget);
		}
		return true;	//...eat it
	}
	return QWidget::eventFilter( obj, evnt );
}
