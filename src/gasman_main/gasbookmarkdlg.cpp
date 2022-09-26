#include "gasbookmarkdlg.h"
#include <QObject>
#if QT_VERSION >=0x050000
#include <QMessageBox>
#else
#include <QtGui>
#endif
#include "gasdoc.h"

/////////////////////////////////////////////////////////////////////////////
// GasBookmarkDlg dialog

GasBookmarkDlg::GasBookmarkDlg( QWidget *parent )
	: QDialog( parent )
{
	setupUi( this );
	//remove a context help button from dialogs
	setWindowFlags( windowFlags() ^ Qt::WindowContextHelpButtonHint );

	m_pDoc = 0;
	m_bFull = false;

	cbTargetCompartment->addItems( listTargetCompart );

	//create connections
	connect( pbAddBookmark, SIGNAL( clicked() ), this, SLOT( addBookmark() ) );
	connect( pbAddTarget, SIGNAL( clicked() ), this, SLOT( addTarget() ) );
	connect( pbDeleteBookmark, SIGNAL(clicked() ), this, SLOT( deleteBookmark() ) );
	connect( pbDeleteTarget, SIGNAL( clicked() ), this, SLOT( deleteTarget() ) );
	connect( pbDeleteAllBookmarks, SIGNAL( clicked() ), this, SLOT( deleteAllBookmarks() ) );
	connect( pbDeleteAllTargets, SIGNAL( clicked() ), this, SLOT( deleteAllTargets() ) );

	connect( pushButtonOK, SIGNAL(clicked()), this, SLOT(accept()) );
	connect( pushButtonOK, SIGNAL(clicked()), this, SLOT(saveBookmarks()) );
	connect( pushButtonCancel, SIGNAL(clicked()), this, SLOT(reject()) );

	sbHours->installEventFilter(this);
	sbMin->installEventFilter(this);
	sbSec->installEventFilter(this);

	cbTargetCompartment->installEventFilter(this);
	sbPercent->installEventFilter(this);
	
	pushButtonOK->installEventFilter(this);

	lwBookmarks->setSortingEnabled(true);
	lwTargets->setSortingEnabled(true);
	//pbAdd->setText(tr("&Add"));
}

bool GasBookmarkDlg::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QEvent::FocusIn)
	{
		if (object == sbHours || object == sbMin || object == sbSec)
			defaultBookmark();
		else if (object == cbTargetCompartment || object == sbPercent)
			defaultTarget();
		else if (object == pushButtonOK)
			defaultOk();
	}
	return QDialog::eventFilter(object, event);
}

void GasBookmarkDlg::setHighTime( quint32 max )
{
	ldiv_t hh = ldiv(max/1000L, 3600L);
	div_t mmss = div((int)hh.rem, 60);
	
	m_nHHMax = (int)hh.quot;
	m_nMMMax = mmss.quot;
	m_nSSMax = mmss.rem;
}

void GasBookmarkDlg::setTime( quint32 time )
{
	ldiv_t  hh = ldiv(qMax(0,(int)(time-1000)/1000), 3600L);
	div_t mmss = div((int)hh.rem, 60);
	
	sbHours->setValue( (int)hh.quot );
	sbMin->setValue( mmss.quot );
	sbSec->setValue( mmss.rem );
}

void GasBookmarkDlg::defaultBookmark()
{
	pushButtonOK->setDefault(false);
	pbAddTarget->setDefault(false);
	pbAddBookmark->setDefault(true);
}

void GasBookmarkDlg::defaultTarget()
{
	pushButtonOK->setDefault(false);
	pbAddBookmark->setDefault(false);
	pbAddTarget->setDefault(true);
}

void GasBookmarkDlg::defaultOk()
{
	pbAddBookmark->setDefault(false);
	pbAddTarget->setDefault(false);
	pushButtonOK->setDefault(true);
}

//initialize and shows the dialog as a modal dialog
int GasBookmarkDlg::exec()
{
	int i;
	for ( i = 0; i < m_pDoc->m_nBookmarks; ++i )
	{//Put all current bookmarks in the list box
		lwBookmarks->addItem( new QListWidgetItem( ( QTime(0,0,0,0).addMSecs( m_pDoc->m_dwBookmarks[i] ) ).toString( "hh:mm:ss" ) ) );
	}

	if ( i == 0 )
	{//if there's nothing to delete, disable delete and clear buttons
		pbDeleteBookmark->setEnabled( false );
		pbDeleteAllBookmarks->setEnabled( false );
	}
	m_bFull = ( i == MAX_BKMK );
	//if we're full, disable add button
	if ( m_bFull )
	{
		pbAddBookmark->setEnabled( false );
		sbHours->setEnabled( false );
		sbMin->setEnabled( false );
		sbSec->setEnabled( false );
	}

	for (i = 0; i < MAX_COMPART; ++i)
	{
		QList<int> list = m_pDoc->m_targetLists[i];
		for (int j = 0; j < list.count(); ++j)
		{
			TargetListWidgetItem* item = new TargetListWidgetItem((AResult)i, list[j]);
			lwTargets->addItem(item);
		}
	}

	if (lwTargets->count() == 0)
	{
		pbDeleteTarget->setEnabled(false);
		pbDeleteAllTargets->setEnabled(false);
	}

	if (m_bFull)
		cbTargetCompartment->setFocus();
	else
		sbHours->setFocus();

	return QDialog::exec();
}

//add new bookmark
void GasBookmarkDlg::addBookmark()
{
	int hh = sbHours->value();
	int mm = sbMin->value();
	int ss = sbSec->value();

    if (    hh > m_nHHMax || (hh == m_nHHMax && mm > m_nMMMax)
             || (hh == m_nHHMax && mm == m_nMMMax && ss > m_nSSMax) ) {
		QMessageBox::critical( this, tr( "Gas Man\xC2\xAE "), tr( "You cannot set a bookmark past the end of the simulation!" ) );
		sbHours->setFocus();
		return;
	}

	if ( !( hh || mm || ss ) ) {
		QApplication::beep();
		sbHours->setFocus();
		return;
	}
	
	Q_ASSERT( !m_bFull );

	QString time = QString( "%1:%2:%3" ).arg( hh, 2, 10, QLatin1Char( '0' ) ).arg( mm, 2, 10, QLatin1Char( '0' ) ).arg( ss, 2, 10,  QLatin1Char( '0' ) );
	
	if ( lwBookmarks->findItems( time, Qt::MatchFixedString ).size() == 0 )
	{
		lwBookmarks->addItem( new QListWidgetItem( time ) );
		
		pbDeleteBookmark->setEnabled( true );
		pbDeleteAllBookmarks->setEnabled( true );
		
		m_bFull = ( lwBookmarks->count() == MAX_BKMK );
		if ( m_bFull ) {
			pbAddBookmark->setEnabled( false );
			sbHours->setEnabled( false );
			sbMin->setEnabled( false );
			sbSec->setEnabled( false );
		}
	}
	pushButtonOK->setFocus();
}

void GasBookmarkDlg::addTarget()
{
	QString compart = cbTargetCompartment->currentText();
	int p = sbPercent->value();

	QString itemText = TargetListWidgetItem::toQString(compart, p);
	QList<QListWidgetItem*> items = lwTargets->findItems(itemText, Qt::MatchFixedString);
	if (items.size() > 0)
		lwTargets->scrollToItem(items[0]);
	else
	{
		AResult c = (AResult)listCompart.indexOf(compart, 0);
		lwTargets->addItem(new TargetListWidgetItem(c, p));
	}

	pbDeleteTarget->setEnabled( true );
	pbDeleteAllTargets->setEnabled( true );
	pushButtonOK->setFocus();
}

//remove existing bookmark
void GasBookmarkDlg::deleteBookmark()
{
	delete lwBookmarks->takeItem( lwBookmarks->currentRow() );
	
	if ( lwBookmarks->count() == 0 ) {
		pbDeleteBookmark->setEnabled( false );
		pbDeleteAllBookmarks->setEnabled( false );
	}
 
	if ( m_bFull ) {
		m_bFull = false;
		pbAddBookmark->setEnabled( true );
		sbHours->setEnabled( true );
		sbMin->setEnabled( true );
		sbSec->setEnabled( true );
	}
	pushButtonOK->setFocus();
}

void GasBookmarkDlg::deleteTarget()
{
	delete lwTargets->takeItem(lwTargets->currentRow());

	if (lwTargets->count() == 0) {
		pbDeleteTarget->setEnabled( false );
		pbDeleteAllTargets->setEnabled( false );
	}
	pushButtonOK->setFocus();
}

//remove all bookmark
void GasBookmarkDlg::deleteAllBookmarks()
{
	lwBookmarks->clear();

	pbDeleteBookmark->setEnabled( false );
	pbDeleteAllBookmarks->setEnabled( false );
	if ( m_bFull ) {
		m_bFull = false;
		pbAddBookmark->setEnabled( true );
		sbHours->setEnabled( true );
		sbMin->setEnabled( true );
		sbSec->setEnabled( true );
	}
	pushButtonOK->setFocus();
}

void GasBookmarkDlg::deleteAllTargets()
{
	lwTargets->clear();

	pbDeleteTarget->setEnabled( false );
	pbDeleteAllTargets->setEnabled( false );
	pushButtonOK->setFocus();
}

void GasBookmarkDlg::saveBookmarks()
{
	m_pDoc->m_nBookmarks = lwBookmarks->count();

	for ( int i = 0; i < m_pDoc->m_nBookmarks; ++i ) {
		QTime t = QTime::fromString( lwBookmarks->item( i )->text(), "hh:mm:ss" );
		m_pDoc->m_dwBookmarks[i] = ( ( t.hour() * 60 + t.minute() ) * 60 + t.second() ) * 1000;
	}

	for ( int i = 0; i < MAX_COMPART; ++i )
		m_pDoc->m_targetLists[i].clear();
	for (int i = 0; i < lwTargets->count(); ++i)
	{
		TargetListWidgetItem* item = dynamic_cast<TargetListWidgetItem*>(lwTargets->item(i));
		m_pDoc->m_targetLists[item->getCompart()].append(item->getPercent());
	}
}
