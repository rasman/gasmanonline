#include <QTabBar>
#include <QDebug>
class GasTabBar : public QTabBar
{
	Q_OBJECT

public:
	GasTabBar(QWidget *parent = 0);

protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
#ifdef Q_OS_MAC
	QSize tabSizeHint(int index) const;
#endif //Q_OS_MACX

signals:
  void drag(int);

private:
  QPoint dragStartPosition;
  bool dragged;
protected:
	 
};
