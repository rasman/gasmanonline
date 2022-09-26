#ifndef GASBKPOINT_H
#define GASBKPOINT_H

#include <QDialog>
#include "ui_gasbkpoint.h"
#include "gasparser.h"
#include "gasdoc.h"
class gasBkPoint : public QDialog
{
	Q_OBJECT

public:
	gasBkPoint( QWidget *parent = 0);
	~gasBkPoint();
public slots:
	void setDoc(GasDoc * doc){m_doc = doc; ui.textEdit->setPlainText(doc->cond);}
	void onOk();
	void onCancel();
private:
	 
	GasDoc *m_doc;
	Ui::gasBkPointClass ui;
	GasParser * par ;
};

#endif // GASBKPOINT_H
