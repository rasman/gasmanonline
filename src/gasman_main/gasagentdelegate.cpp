#include "gasagentdelegate.h"
#include "gasglobal.h"
#include "gasapplication.h"
#include "gaslicensemanager.h"
#include "gascontrol.h"
#include "gasdoc.h"

GasAgentDelegate::GasAgentDelegate(
	 anestheticColumnType type
	,QObject *parent
)
	: QStyledItemDelegate(parent), columnType(type)
{
}

/*
void GasAgentDelegate::paint(
	QPainter *painter
	,const QStyleOptionViewItem &option 
	,const QModelIndex &index
) const
{
	QString datum = index.model()->data(index, Qt::DisplayRole).toString();
	drawDisplay(painter, option, option.rect, datum);
	drawFocus(painter, option, option.rect);
}
*/

QStringList GasAgentDelegate::getAgentNames(QWidget *parent) const
{
	GasControl *gc = 0;
	do {
		if (!parent)
			return QStringList();
		if ((gc = qobject_cast<GasControl*>(parent)) == 0)
			parent = parent->parentWidget();
	} while (gc == 0);

	QStringList names;
	GasAnesArray& ar = gc->doc->m_anesArray;
	for( int i=0; i<ar.size(); i++ )
	{
		names<<ar[i]->m_strName;
	}
	return names;
}

QWidget *GasAgentDelegate::createEditor(
	 QWidget *parent
	,const QStyleOptionViewItem &/*option*/ 
	,const QModelIndex &index
) const
{
	QString datum = index.model()->data(index, Qt::DisplayRole).toString();
	if (datum.isEmpty())
		return 0;

	QWidget *ret = NULL;
	QComboBox *combo = NULL;
	QDoubleSpinBox *spin = NULL;
	switch(columnType)
	{
	case anestheticNameColumnType:
		combo = new QComboBox(parent);
		combo->addItems(getAgentNames(parent)); 
		combo->view()->setMinimumWidth(100);
		ret = combo;
		break;
	case anestheticSizeColumnType:
		spin = new QDoubleSpinBox(parent);
		spin->setDecimals(0);
		spin->setRange(0, MAX_BOTTLE_SIZE);
		spin->setSingleStep(1);
		spin->setMinimumWidth(40);
		ret = spin;
		break;
	case anestheticCostColumnType:
		spin = new QDoubleSpinBox(parent);
		spin->setDecimals(2);
		spin->setRange(0, MAX_BOTTLE_COST);
		spin->setSingleStep(1);
		spin->setMinimumWidth(40);
		ret = spin;
		break;
	case anestheticColorColumnType:
		combo = new QComboBox(parent);
		combo->addItems(gasApp->getColors());
		combo->view()->setMinimumWidth(50);
		ret = combo;
		break;
	}
	ret->setContextMenuPolicy(Qt::NoContextMenu);
	ret->installEventFilter(const_cast<GasAgentDelegate*>(this));
	return ret;
}

void GasAgentDelegate::setEditorData(
	 QWidget *editor
	,const QModelIndex &index
) const
{
	QString stringDatum;
	double doubleDatum;
	QComboBox *combobox = NULL;
	QDoubleSpinBox *spin = NULL;
	switch(columnType)
	{
	case anestheticNameColumnType:
	case anestheticColorColumnType:
		stringDatum = index.model()->data(index, Qt::DisplayRole).toString();
		combobox = qobject_cast<QComboBox*>(editor);
		combobox->setCurrentIndex(combobox->findText(stringDatum));
		break;
	case anestheticSizeColumnType:
	case anestheticCostColumnType:
		doubleDatum = index.model()->data(index, Qt::DisplayRole).toDouble();
		spin = qobject_cast<QDoubleSpinBox*>(editor);
		spin->setValue(doubleDatum);
		break;
	}
}


void GasAgentDelegate::setModelData(
	 QWidget *editor
	,QAbstractItemModel *model 
	,const QModelIndex &index
) const
{
	QString stringDatum;
	double doubleDatum;
	QComboBox *combobox = NULL;
	QDoubleSpinBox *spin = NULL;
	switch(columnType)
	{
	case anestheticNameColumnType:
	case anestheticColorColumnType:
		combobox = qobject_cast<QComboBox*>(editor);
		stringDatum = combobox->currentText();
		model->setData(index, stringDatum);
		break;
	case anestheticSizeColumnType:
	case anestheticCostColumnType:
		spin = qobject_cast<QDoubleSpinBox*>(editor);
		doubleDatum = spin->value();
		model->setData(index, doubleDatum);
		break;
	}
}
