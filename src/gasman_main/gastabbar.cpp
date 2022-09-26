#include "gastabbar.h"
#include <QtGlobal>
#if QT_VERSION >=0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

GasTabBar::GasTabBar(QWidget *parent) : QTabBar(parent){
	dragged = false;
//    QFont font;
	//font.setStretch(150);
//	font.setPixelSize(9);
//	setFont(font); 
/*	QFontInfo fi = fontInfo();
	qDebug()<<fi.pointSize()<<fi.pixelSize()<<fi.family();
*/	 
	setToolTip(tr( "Click/drag this tab away to view content separately or to move it to another window." ));
	setFocusPolicy ( Qt::ClickFocus);

}

void GasTabBar::mousePressEvent(QMouseEvent *event)
{
  if( (event->button() == Qt::LeftButton) &&
			(tabAt(event->pos()) != -1))
  {
    dragStartPosition = event->pos();
    QTabBar::mousePressEvent(event);
  }
}

void GasTabBar::mouseMoveEvent(QMouseEvent *event)
{
	if( (event->buttons() & Qt::LeftButton) &&
			(tabAt(event->pos()) != -1) &&
			((event->pos() - dragStartPosition).manhattanLength() > QApplication::startDragDistance()))
	{
		emit drag(tabAt(event->pos()));
	}
}

#ifdef Q_OS_MAC
// Awful hack for Mac OS X: in order to get Mac OS X to use tab arrows
// when the combined tab widths are greater than the width of the tab widget,
// you must set the style to the provided "gasmacstyle.h". If you don't, as
// tab widths grow, the widget will resize itself (they can get huge). The
// problem with setting to this Mac style is that we turn off the ability for
// the tab text to suggest the size of the tabs. So what we get instead of
// "This is very long tab text" is "This is ve...". Because of that, we must
// calculate a good tab size from the text it is displaying. The 23 and 8 are
// fudge factors due to frames and whatnot.
QSize GasTabBar::tabSizeHint(int index) const
{
	static QMap<int, QSize> sizes;
	static QFontMetrics fm(font());
	QString text = tabText(index);
	if(!text.contains("..."))
		sizes[index] = QSize(fm.width(text)+23, fm.height()+8);
	return sizes[index];
}
#endif //Q_OS_MAC
