#ifndef GASCONTROL_H
#define GASCONTROL_H

#include "ui_gascontrol.h"
#include "gasglobal.h"
#include "gasviewcommon.h"
#include <QPointer>
 
class GasDoc;

class GasControl : public GasViewCommon, private Ui::gascontrol
{
	Q_OBJECT
public:
	GasControl( GasDoc* gasDoc, GasChildWindow * childWindow, QWidget *parent = 0 );
	~GasControl();

	bool tryCloseAll();
	QPointer<GasDoc> doc;
	QTableWidget * getTable() {return agentTableWidget;}
	void initialAgentsUpdate();
	bool tryGetGas(int& nGas);

protected:
	virtual bool eventFilter( QObject *obj, QEvent *evnt );

private:
	QAction* newGraphViewAction;
	QAction* newPanelViewAction;
	QAction* deleteAgentAction;
	QAction* bringToFrontAction;
	QAction* editCostAction;

	QStringList getAllAgents();
	QStringList getAllColors();

	void tryAddRow();
	void setUpAddAgentText();
	void addAgentToWidget(const int nGas);

	bool tryGetCurrentRow(int& rowRef);

signals:
	void activateChild(QWidget* widget);
	void addAgentView(GasDoc* doc, int nGas, GasViewType viewType);
	void activateChildWindows(const GasChildren& children);
	void removeChildWindow(QObject* obj);

public slots:
	void updateControlVals();
	void setWeight(double weight);
	void setCircuit(const QString& circuit);
	void setSpeed(const QString& speed);
	void setState( const AState &theState );
	void setTitle(const QString& title);
	void setAgent(int nGas, const QString& agent);
	 
private slots:
	void changeAgent(int nGas, const QString& agent);
	void agentColorsChanged();
	void clearAllRequested();
	void updateAgentWidget(QTableWidgetItem* item);
	void updateWeight();
	void updateCircuit(const QString  &circuit);
	void updateSpeed(const QString &speed);
	void pauseSim();
	void newViewRequested();
	void deleteAgentRequested();
	void bringToFrontRequested();
	void removeAgent(int gasNum);
	void moveUpRequested();
	void moveDownRequested();
	void swapRows(int old, int notSoOld, QTableWidgetItem* item);
	void updateAgentWidgetState(bool enable);
	void setCost();
	void runSim();
	void onAFAPButton();
	void onSelectChanged(int currentRow, int,int ,int);
};

#endif
