#ifndef GASAGENTDELEGATE_H
#define GASAGENTDELEGATE_H

#include <QObject>
#if QT_VERSION >=0x050000
#include <QtWidgets>
#include <QStyledItemDelegate>
#else
#include <QtGui>
#endif


class GasAgentDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
 
	enum anestheticColumnType		
	{
		anestheticNameColumnType,
		anestheticSizeColumnType,
		anestheticCostColumnType,
		anestheticColorColumnType
	};

	GasAgentDelegate(anestheticColumnType type, QObject *parent=0);
	//void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
	anestheticColumnType columnType;
	QStringList getAgentNames(QWidget *parent) const;
};

#endif
