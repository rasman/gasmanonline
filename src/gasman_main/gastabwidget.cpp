#include <QtGui>
#if QT_VERSION >=0x050000
#include <QtWidgets>
#endif
#include "gastabwidget.h"
#include "gaschildwindow.h"
#include "gasdoc.h"
#include "gaspanelview.h"
#include "gasapplication.h"

GasTabWidget::GasTabWidget(QWidget *parent) : QTabWidget(parent), gasTab(new GasTabBar){
	connect(gasTab, SIGNAL(drag(int)), this, SLOT(handleDrag(int)));
	setTabBar(gasTab);
	setAcceptDrops(true);
	setAttribute( Qt::WA_DeleteOnClose );

// Qt 4.3.2 Bug: OS X usually places tabs in the top-middle of a widget but
// Qt does not return the correct position of the QTabBar when doing so.
// Qt claims the QTabBar is at the top-left. Setting the widget style to 
// Plastique places the QTabBar in the top-left.
#ifdef Q_OS_MACX
	setStyle(QStyleFactory::create("Fusion"));
#endif
	 
	closeTabButton = new QToolButton(this);
	setCornerWidget(closeTabButton, Qt::TopRightCorner);
	closeTabButton->setToolTip(tr("Close tab"));
	closeTabButton->setAutoRaise(true);
	closeTabButton->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
	connect(closeTabButton, SIGNAL(clicked()), this, SLOT(tryCloseTab()));
	connect(this, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged()));
	closeTabButton->setFocusPolicy ( Qt::ClickFocus);
	setFocusPolicy ( Qt::ClickFocus);
}

void GasTabWidget::tryCloseTab()
{
	GasChildWindow* child = qobject_cast<GasChildWindow*>(currentWidget());
	if(child)
		child->close();
}

void GasTabWidget::handleDrag(int tabNum)
{
	QDrag *drag = new QDrag(this);
	QMimeData* mimeData = new QMimeData;
	mimeData->setData(GasApplication::DragMime, QByteArray::number(gasApp->pushGasTab(widget(tabNum))));
	drag->setMimeData(mimeData);
	drag->setPixmap(QPixmap::grabWidget(widget(tabNum)).scaled(QSize(300,300), Qt::KeepAspectRatio));
	drag->exec();
	if (count() == 0)
		qobject_cast<QWidget*>(parent())->close();
}


// If tab widget contains a control, return whether it is ok to close
// all tab widgets. If there is no control, return true.
bool GasTabWidget::canCloseAll()
{
	QList<GasChildWindow*> children;
	GasChildWindow* tmp = 0;
	bool ret = false;

	// get all children
	for(int i = 0; i < count(); i++)
		children << qobject_cast<GasChildWindow*>(widget(i));

	// see if there is a control tab
	foreach(GasChildWindow* child, children)
	{
		if(child->gasControl())
			tmp = child;
	}

	// if there is a control tab, check if gasdoc can close
	if(tmp)
		ret = tmp->doc()->close();
	else
		ret = true;

	// if ok, close all windows
	if(ret)
		foreach(GasChildWindow* child, children)
			child->close();

	return ret;
}

void GasTabWidget::closeEvent(QCloseEvent* event)
{
	if(canCloseAll())
	{
		event->accept();
		qobject_cast<QWidget*>(parent())->close();
	}else
		event->ignore();
}

void GasTabWidget::dragEnterEvent(QDragEnterEvent *event)
{
	if(gasApp->IsGoodDrop(event))
	{
		// if reordering tabs
		if(event->source() == this) 
		{
			if(gasTab->rect().contains(event->pos()))
				event->acceptProposedAction();
			else
				event->ignore();
		}

		// if reparenting widget
		if(event->source() != this)
				event->acceptProposedAction();
	}
}

void GasTabWidget::dragMoveEvent(QDragMoveEvent *event)
{
	if(gasApp->IsGoodDrop(event))
	{

		// if reordering tabs
		if(event->source() == this) 
		{
			if(gasTab->rect().contains(event->pos()))
				event->acceptProposedAction();
			else
				event->ignore();
		}

		// if reparenting widget
		if(event->source() != this)
				event->acceptProposedAction();
	}
}

void GasTabWidget::dropEvent(QDropEvent *event)
{
	if (!gasApp->IsGoodDrop(event))
		return;

	if(event->source() == this)
	{
		GasChildWindow* child = qobject_cast<GasChildWindow*>(gasApp->popGasTab());
		reorderTabs(event->pos(), child);
	}
	else
		emit reparentChildDrop(event, this);
}

// L33T H4X0R
void GasTabWidget::reorderTabs(const QPoint& pos, GasChildWindow* child)
{
	setUpdatesEnabled(false);

	int tabDrop = gasTab->tabAt(pos);	
	int tabRemove = indexOf(child);

	if(tabDrop == tabRemove)
	{
		setUpdatesEnabled(true);
		return;
	}

	QRect firstHalf = gasTab->tabRect(tabDrop);
	firstHalf.setWidth(firstHalf.width()/2);

	removeTab(tabRemove);

	// if cursor in first half of tab
	if(!firstHalf.contains(pos) && (tabRemove > tabDrop))
		tabDrop++;
	if(firstHalf.contains(pos) && (tabRemove < tabDrop))
		tabDrop--;

	insertTab(tabDrop, child, child->title());
	setCurrentIndex(tabDrop);

	setUpdatesEnabled(true);
}

void GasTabWidget::resizeIfNec()
{
	/*
	QSize parentWidgetSize(parentWidget()->size());
	QSize newSize(parentWidgetSize);
	QSize parentWidgetSizeHint(parentWidget()->sizeHint());

	if(parentWidgetSize.width() < parentWidgetSizeHint.width())
		newSize.setWidth(parentWidgetSizeHint.width());
	if(parentWidgetSize.height() < parentWidgetSizeHint.height())
		newSize.setHeight(parentWidgetSizeHint.height());
			
	if(newSize != parentWidgetSize)
		parentWidget()->resize(newSize);*/
 
}

void GasTabWidget::tabRemoved(int index)
{
	QTabWidget::tabRemoved(index);
}

void GasTabWidget::tabInserted ( int index )
{
	QTabWidget::tabInserted(index);
	GasChildWindow* child = qobject_cast<GasChildWindow*>(widget(index));
	 
	connect(child, SIGNAL(titleUpdated(QString)), this, SLOT(updateTab(QString)));
	connect(child, SIGNAL(beforeClose()), this, SLOT(removeTabOnClose()));
	currentTabChanged();
	gasTab->setTabTextColor(index, qobject_cast<GasChildWindow*>(widget(index))->tabColor());
}

void GasTabWidget::updateTab(QString title)
{
	setTabText(indexOf(qobject_cast<QWidget*>(sender())), title);
}

void GasTabWidget::removeTabOnClose()
{
	GasChildWindow* child = qobject_cast<GasChildWindow*>(sender());
	removeTab(indexOf(child));
	if(!count())
		close();
}

void GasTabWidget::currentTabChanged()
{
	QWidget *current = currentWidget();
	if (!current)
		return;

	if(qobject_cast<GasChildWindow*>(current)->gasControl())
		closeTabButton->hide();
	else
		closeTabButton->show();

	// Makes no sense, but otherwise Qt 5.2 ignores the tabTextColor
	setStyleSheet("QTabBar::tab:selected{background-color:palette(mid);}");
}

void GasTabWidget::updateTabTextColor()
{
	for(int i = 0; i < count(); i++){
		gasTab->setTabTextColor(i, qobject_cast<GasChildWindow*>(widget(i))->tabColor());
	}
}
