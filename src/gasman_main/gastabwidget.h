#include <QTabWidget>
#include <QDebug>
#include "gastabbar.h"

class GasChildWindow;
class QToolButton;

class GasTabWidget : public QTabWidget
{
	Q_OBJECT

public:
	GasTabWidget(QWidget *parent = 0);
	QSize sizeHint() const {
#ifdef Q_OS_MAC	
        return QSize(540,405);
#endif
		return QSize(435,350);
	}

public slots:
	void updateTabTextColor();

protected:
	void closeEvent(QCloseEvent* event);
    void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
	void tabInserted ( int index );
	void tabRemoved(int index);


private slots:
	void handleDrag(int);
    void tryCloseTab();
	void updateTab(QString title);
	void removeTabOnClose();
    void currentTabChanged();

signals:
  void reparentChildDrop(QDropEvent* event, GasTabWidget* tabWidget);

private:
	GasTabBar* gasTab;
	QToolButton *closeTabButton;
	void reorderTabs(const QPoint& pos, GasChildWindow* child);
	void makeCloseConnect(QWidget* page);
	bool canCloseAll();
	void resizeIfNec();
 
};
