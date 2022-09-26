#include "gasbkpoint.h"

 
gasBkPoint::gasBkPoint( QWidget *parent)
	:  QDialog(parent)
{
	ui.setupUi(this);
	ui.lb_caution->setVisible(false);
	connect(ui.bt_OK,SIGNAL(clicked()), this, SLOT(onOk()));
	connect(ui.bt_Cancel, SIGNAL(clicked()), this, SLOT(onCancel()));
	par = new GasParser(this);
}
void gasBkPoint::onOk()
{
	QString str = ui.textEdit->toPlainText();
	if(str.simplified() == QString()){
		m_doc->cond.clear();
		m_doc->op.clear();
		m_doc->si.clear();
		m_doc->expv=0;
		m_doc->oldval= 0;
		close();
	}else if(par->parse(str)){
		m_doc->cond = str.simplified();
		m_doc->op = par->OpStack;
		m_doc->si = par->SiStack;
		m_doc->expv = par->result;
		m_doc->oldval = m_doc->expv;
		close();
	}else{
		ui.lb_caution->setVisible(true);
	}
}

void gasBkPoint::onCancel()
{
	close();
}

gasBkPoint::~gasBkPoint()
{

}
