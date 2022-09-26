
#ifndef GASCHILDWINDOW_H
#define GASCHILDWINDOW_H

#include <QMainWindow>
#include "gasglobal.h"

class QScrollArea;
class GasPanelView;
class GasGraphView;
class GasControl;
class GasDoc;
class GasMainWindow;
class QFrame;

class GasChildWindow : public QMainWindow
{
	
	Q_OBJECT
public:
	GasChildWindow( GasDoc * document, int nGas, GasViewType type, QWidget *parent = 0 );
	~GasChildWindow();

	void showPanelView();
	void showGraphView();
	void showGasControl();
	void initShow(int nGas);
	void updateAllView(int nGas);

	GasViewType getViewType();
	GasPanelView *panelView() { Q_ASSERT( gaspanelview != 0 ); return gaspanelview; }
	GasGraphView *graphView() { Q_ASSERT( gasgraphview != 0 ); return gasgraphview; }
	GasControl *gasControl() { return gasControlWidget; }

	GasDoc *doc() { Q_ASSERT( gasdoc != 0 ); return gasdoc; }
	QString title();
	QString tabColor();
	bool tryGetGas(int& nGas);
	void resetGas(const int nGas);

	void saveWindowState();
	void restoreWindowState();
public slots:
	void setTitle( const QString &t, const QString& frameNum, const QString& priority, const QString& agent );

signals:
	void titleUpdated(QString title);
	void viewsChanged();
	void beforeClose();

protected:
	virtual void closeEvent( QCloseEvent * evnt );

private:
	GasPanelView *gaspanelview;				//Pointer to Panel View
	GasControl *gasControlWidget;
	GasGraphView *gasgraphview;				//Pointer to Graph View
	GasDoc *gasdoc;							//Pointer to main document
	QScrollArea *saGraph;
	QScrollArea *saPanel;
	Qt::WindowStates state;
	GasViewType viewType;
	QString m_title;
};

#endif
