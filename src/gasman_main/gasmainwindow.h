#ifndef GASMAINWINDOW_H
#define GASMAINWINDOW_H

#include <QObject>
#if QT_VERSION >=0x050000
#include <QMainWindow>
#include <QtWidgets>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPageSetupDialog>
#else
#include <QtGui>
#endif
#include <QTextStream>
#include "gasglobal.h"

//Forward declaration
class QDomDocument;
class QSignalMapper;
class Assistant;
class QTextDocument;
class QMdiArea;
class GasTabWidget;
class GasChildWindow;
class GasGraphView;
class QBuffer;
class QTemporaryFile;

class GasMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	GasMainWindow();
	~GasMainWindow();

	GasChildWindow *activeChild();
	GasChildWindow *activeChildForced(); // use if you KNOW child should exist
	GasChildWindow *gasChildOfMdiSubWindow(QMdiSubWindow* sw);
	GasChildWindow* largestVisibleGraphic( );
	void updateRecentFileActions();  

	inline QPrinter *printer() const { return prn; }
	QMenu* createPopupMenu();
	QAction *disableReturnAction;
	QAction *disableUptakeAction;
	QAction *enableVaporAction;
	QList<QMdiSubWindow *> getSubWins();

	inline QString OpenDir() const { return !openDir.isEmpty() ? openDir : (saveDir.isEmpty() ? QDir::homePath() : saveDir); }
	inline QString SaveDir() const { return !saveDir.isEmpty() ? saveDir : (openDir.isEmpty() ? QDir::homePath() : openDir); }
	inline QString TransDir() const { return !transDir.isEmpty() ? transDir : QDir::homePath(); }
	void setOpenDir(const QString& dir);
	void setSaveDir(const QString& dir);
	void setTransDir(const QString& dir);

public slots:
	bool newFile();
	void subWindowActivated(QMdiSubWindow* sw);
	void updateAllActions();
	void updateGasDocs();
	void removeChild( QObject *obj );
	void showHelpTopic();

private slots:
	void newViewOrAdd();
	void groupSubWindows();
    void pictureOnTop();
    void graphOnTop();
    void controlOnTop();
    void onTop( const GasViewType viewType );
	bool editDescription();
	void reparentChildDrop(QDropEvent* event, GasTabWidget* dest = 0);
	void reparentChild(GasChildWindow *child, GasTabWidget *source, GasTabWidget* dest = 0, const QPoint& point=QPoint());
	void tabifyChild(GasChildWindow* child);
	//void giveFocusToMain();
	void updateActiveGas();
	void openFile();
	void saveFile();

	void saveFileAs();
	void closeFile();
    void closeAllFiles();
	void sendMail();	//Send a message with the document as an attachment
	void aboutGasMan();
	void print();		// Print current simulation
	void pageSetup();	//Show the configuration dialog for the 
						//page-related printer options
	void newView(GasDoc* doc = 0, int nGas = 0, GasViewType viewType=PANEL);

	void registerProduct();
	void showHelp();
	void showWorkbook();
	void setShowToolbar( bool set );
	void selectPrintOutput();
	void setDefaultOptions();

	void openRecentFile();
	void removeDoc( QObject *obj );
	void createViewMenu();
	void createWindowMenu();
	void updateFileMenu();
	void updateEditMenu();
	void updateAnesthesiaMenu();
	void updateSpecialMenu();
	void updateTitle();
	void updateViewMenu();
	void updateWindowMenu();
	void setActiveWindow( QWidget *child );
	void setActiveWindowIfNec( QWidget *child );
	void runSimulation();
	void runAllSimulation();
	void liquidInject();
	void rewind();
	void rewindAllSimulations();
	void fastFwd();
	void zeroTimer();
	void clearAll();
	void copyData();		// Copy data to clipboard
	void selectAll();		// Create a pixmap constructed by grabbing the 
							// contents of the current child window
	void copySelection();	//and copying them to the clipboard
	void defaultAgentColors();
	void showCost( bool set );
	void showMl( bool set );
	void hideNumerics( bool set );
	void disableUptake( bool set );
	void disableReturn( bool set );
	void enableVapor( bool set );
	void enableScrolling( bool set );
	void setBookmark();
	void setBreakPoint();
	void changePatient();
	void overlay();
	void zoomin();
	void zoomout();
	void unitDose();
	void setCost();
	void adjustDel();		//Change FGF, VA, CO etc.
	void changeAgent();
	void windowStateChanged();

	void printPreview();	//Show print preview dialog
	void exportToPdf();

	void exportToXml();
	void exportToHtml();
	void transform();
	//void switchLanguage( QAction *action );	

protected:
    virtual void showEvent(QShowEvent *evnt);
	virtual void closeEvent( QCloseEvent *evnt );
    virtual void resizeEvent( QResizeEvent *evnt );
    virtual void moveEvent( QMoveEvent *evnt );
	virtual bool eventFilter( QObject *obj, QEvent *evnt );
	virtual bool event( QEvent *evnt );

private:
	GasChildWindow *createChild( GasDoc * gasdoc, int nGas, GasViewType type );
	GasChildren allChildren();
	GasDoc* newDoc();
	GasTabWidget* newTabWidget(GasDoc* doc);
	GasGraphView* FirstGraphView(GasDoc* pdoc);
	GasChildWindow* FirstGraphWindow(GasDoc *pdoc);

	void newControlDock(GasDoc* gasdoc);
	void updateGasPrintDoc(const QString& family, int fontSize);
	void updateLocale();	
	bool printHelper(const QPrinter::OutputFormat fmt, const QString& fname);
	void writeHtmlToFile(GasDoc* doc, const QString& fileName, bool tmpImage);
	bool saveTransform(QDomDocument& dom, const QString& transformFileName, const QString& outputFileName);
	QString createOutputImageFile(QFile& imageFile, const QString& htmlFileName, const QString& imageName);
	void writeHtmlToGasPrintDoc(GasChildWindow *child);
	void createRecentMenu();
	void createActions();
	void createMenus();
	//void createLanguageMenu();
	void createToolBar();
	void open( const QString &fileName );	//Open a *.gas file by this name

	int CountRunners();						//Count of the started simulations
	int CountRunable();						//Count of the runable simulations
	int CountRewindable();					//Count of the rewindable simulations

	bool dropPosAcceptable(QDropEvent* event);
	bool checkCountSimulations();
	bool m_bPrintout;
	bool m_bPrintGraphs;

	//Assistant *assistant;
	QMdiArea *mdi;
    bool m_bPositioned;
    QRect m_geometry;
	QSignalMapper *windowMapper;
	QPixmap shootscreen;
	QPrinter *prn;
	QTextDocument *gasPrintDocument;		//Text document for printing
	QString openDir;
	QString saveDir;
	QString transDir;
	QString getExportFileName(const QString& title, const QString& fileTypeDesc, const QString& fileExtension);

	QAction *recentFileActs[MaxRecentFiles];
	QAction *separatorAct;
	QAction *aboutGasManAction;
	QAction *editDescriptionAction;
	QAction *agentAction;
	QAction *changePatientAction;
	QAction *clearAllAction;
	QAction *closeDocumentAction;
    QAction *closeAllDocumentsAction;
	QAction *copyDataAction;
	QAction *copySelectionAction;
	QAction *defaultAgentColorsAction;
	QAction *delAction;
	QAction *enableScrollingAction;
	QAction *exitAction;
	QAction *fastFwdAction;
	QAction *helpAction;
	QAction *helpTopicAction;
	QAction *helpWorkbookAction;
	QAction *hideNumericsAction;
	QAction *liquidInjectAction;
	QAction *newDocumentAction;
	QAction *newGraphViewAction;
	QAction *newPanelViewAction;
	QAction *openDocumentAction;
	QAction *optionsAction;
	QAction *overlayAction;
	QAction *zoominAction;
	QAction *zoomoutAction;
	QAction *pageSetupAction;
	QAction *printAction;
	QAction *printSelectAction;
	QAction *rewindAction;
	QAction *rewindAllAction;
	QAction *runAction;
	QAction *runAllAction;
	QAction *saveDocumentAction;
	QAction *saveDocumentAsAction;
	QAction *selectAllAction;
#ifndef Q_OS_MACX
	QAction *sendAction;
#endif //Q_OS_MACX 
	QAction *printPreviewAction;
	QAction *setBookmarkAction;
	QAction *setBreakPointAction;
	QAction *setCostAction;
	QAction *showCostAction;
	QAction *showMlAction;
	QAction *showToolbarAction;
	QAction *tileSubWindowsAction;
	QAction *groupSubWindowsAction;
    QAction *pictureOnTopAction;
    QAction *graphOnTopAction;
    QAction *controlOnTopAction;
	QAction *cascadeSubWindowsAction;
	QAction *unitDoseAction;
	QAction *xmlAction;
	QAction *htmlAction;
	QAction *transformAction;
	QAction *registerAction;
	QAction *pdfAction;
	QAction *zeroTimerAction;

	QMenu *newGraphViewMenu;
	QMenu *newPanelViewMenu;
	QMenu *anesthesiaMenu;
	QMenu *editMenu;
	QMenu *openRecentMenu;
	//QMenu *languageMenu;
	QMenu *specialMenu;
	QMenu *viewMenu;
	QMenu *windowMenu;
	QMenu *menuExport_to;

	QToolBar *toolBar;

	friend class GasApplication;
	friend class GasChildWindow;
	friend class GasControl;
};

extern GasMainWindow *gasMainWindow;

#endif

