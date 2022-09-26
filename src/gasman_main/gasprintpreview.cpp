
#include "gasprintpreview.h"

#include <QObject>
#if QT_VERSION >=0x050000
#include <QtWidgets>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPageSetupDialog>
#else
#include <QtGui>
#endif

/*
	Convert inches to pixels
*/
static inline int inchesToPixels( float inches, QPaintDevice *device )
{
	return qRound( inches * device->logicalDpiY() );
}

/*
	Convert millimeters to inches
*/
static inline qreal mmToInches( double mm )
{
	return mm*0.039370147;
}

class GasPreviewView : public QAbstractScrollArea
{
    Q_OBJECT
public:
    GasPreviewView( QTextDocument *document );
	~GasPreviewView(){};

    inline void updateLayout() { resizeEvent( 0 ); viewport()->update(); }

	inline void setTopMargin( int top ) { topMargin = top; }
	inline void setBottomMargin( int bottom ) { bottomMargin = bottom; }
	inline void setLeftMargin( int left ) { leftMargin = left; }
	inline void setRightMargin( int right ) { rightMargin = right; }

public slots:
    void zoomIn();			//Increase scale
    void zoomOut();			//Decrease scale

protected:
    virtual void paintEvent( QPaintEvent *e );
    virtual void resizeEvent( QResizeEvent * );
    virtual void mousePressEvent( QMouseEvent *e );
    virtual void mouseMoveEvent( QMouseEvent *e );
    virtual void mouseReleaseEvent( QMouseEvent *e );

private:
    void paintPage( QPainter *painter, int page );		//Draw the preview page
    QTextDocument *doc;									//Clone of this text document
    qreal scale;
    int interPageSpacing;
    QPoint mousePressPos;
    QPoint scrollBarValuesOnMousePress;
	int leftMargin;
	int rightMargin;
	int topMargin;
	int bottomMargin;
};

/*
	GasPreviewView constructor
*/
GasPreviewView::GasPreviewView( QTextDocument *document )
{
    verticalScrollBar()->setSingleStep( 20 );
    horizontalScrollBar()->setSingleStep( 20 );

    viewport()->setBackgroundRole( QPalette::Dark );

	leftMargin = rightMargin = topMargin = bottomMargin = 0;

    doc = document;
    scale = 1.0;
    interPageSpacing = 30;
}

/*
	This routine increases the scale
*/
void GasPreviewView::zoomIn()
{
    scale += 0.2;
    resizeEvent( 0 );
    viewport()->update();
}

/*
	This routine decreases the scale
*/
void GasPreviewView::zoomOut()
{
    scale -= 0.2;
    resizeEvent( 0 );
    viewport()->update();
}

void GasPreviewView::paintEvent( QPaintEvent * )
{
    //The start of drawing the print preview page
	QPainter p( viewport() );

    p.translate( -horizontalScrollBar()->value(), -verticalScrollBar()->value() );
    p.translate( interPageSpacing, interPageSpacing );

    const int pages = doc->pageCount();
    for ( int i = 0; i < pages; ++i ) {
        p.save();
        p.scale( scale, scale );

        paintPage( &p, i );		//Draw the preview page

        p.restore();
        p.translate( 0, interPageSpacing + ( doc->pageSize().height() + topMargin + bottomMargin ) * scale );
    }
}

/*
	Draw the preview page
*/
void GasPreviewView::paintPage( QPainter *painter, int page )
{
    const QSizeF pgSize = doc->pageSize();

    QColor col( Qt::black );

    painter->setPen(col);
    painter->setBrush( Qt::white);
    painter->drawRect( QRectF( QPointF( 0, 0 ), pgSize + QSizeF( leftMargin + rightMargin, topMargin + bottomMargin ) ) );
    painter->setBrush( Qt::NoBrush );

    col = col.light();
    painter->drawLine( QLineF( pgSize.width() + leftMargin + rightMargin, 1, pgSize.width() + leftMargin + rightMargin, pgSize.height() + topMargin + bottomMargin - 1 ) );

    col = col.light();
    painter->drawLine( QLineF( pgSize.width() + leftMargin + rightMargin, 2, pgSize.width() + leftMargin + rightMargin, pgSize.height() + topMargin + bottomMargin - 2 ) );

    
	QRectF docRect( QPointF( 0, page * pgSize.height() ), pgSize );
    QAbstractTextDocumentLayout::PaintContext ctx;
    ctx.clip = docRect;
	//Don't use the system palette text as the default text color. On HP/UX,
	//for example, that's white, and white text on white paper doesn't
	//look right
    painter->translate( leftMargin, - page * pgSize.height() + topMargin );
    painter->setClipRect( docRect );
    doc->documentLayout()->draw( painter, ctx );
}

void GasPreviewView::resizeEvent( QResizeEvent * )
{
    const QSize viewportSize = viewport()->size();

    //Resize the viewport widget
	QSize docSize;
    docSize.setWidth( qRound( ( doc->pageSize().width() + leftMargin + rightMargin ) * scale + 2 * interPageSpacing ) );
    const int pageCount = doc->pageCount();
    docSize.setHeight( qRound( pageCount * ( doc->pageSize().height() + topMargin + bottomMargin ) * scale + ( pageCount + 1 ) * interPageSpacing ) );

    horizontalScrollBar()->setRange( 0, docSize.width() + + leftMargin + rightMargin - viewportSize.width() );
    horizontalScrollBar()->setPageStep( viewportSize.width() );

    verticalScrollBar()->setRange( 0, docSize.height() + topMargin + bottomMargin - viewportSize.height() );
    verticalScrollBar()->setPageStep( viewportSize.height() );
}

void GasPreviewView::mousePressEvent( QMouseEvent *e )
{
    mousePressPos = e->pos();
    scrollBarValuesOnMousePress.rx() = horizontalScrollBar()->value();
    scrollBarValuesOnMousePress.ry() = verticalScrollBar()->value();
    e->accept();
}

void GasPreviewView::mouseMoveEvent( QMouseEvent *e )
{
    if ( mousePressPos.isNull() ) {
        e->ignore();
        return;
    }

    horizontalScrollBar()->setValue( scrollBarValuesOnMousePress.x() - e->pos().x() + mousePressPos.x() );
    verticalScrollBar()->setValue( scrollBarValuesOnMousePress.y() - e->pos().y() + mousePressPos.y() );
    horizontalScrollBar()->update();
    verticalScrollBar()->update();
    e->accept();
}

void GasPreviewView::mouseReleaseEvent( QMouseEvent *e )
{
    mousePressPos = QPoint();
    e->accept();
}

/*
	GasPrintPreview constructor
*/
GasPrintPreview::GasPrintPreview( const QTextDocument *document, QPrinter *prn, QWidget *parent )
    : QMainWindow( parent ), printer( *prn )
{
    setWindowTitle( tr( "Print Preview" ) );
	setAttribute( Qt::WA_DeleteOnClose );
	setWindowModality( Qt::ApplicationModal );

    //Creates a new document that is a copy of this text document
	doc = document->clone();

    view = new GasPreviewView( doc );
    setCentralWidget( view );
    resize( 800, 600 );

    doc->setUseDesignMetrics( true );
    doc->documentLayout()->setPaintDevice( view->viewport() );

    setup();

    QFont f( doc->defaultFont() );
    f.setPointSize( 10 );
    doc->setDefaultFont( f );

    QToolBar *tb = addToolBar( tr( "Print" ) );
    tb->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );

    QAction *a = new QAction( QIcon( ":/images/print.png" ), tr( "&Print..." ), this );
    a->setShortcut( Qt::CTRL + Qt::Key_P );
    connect( a, SIGNAL( triggered() ), this, SLOT( print() ) );
    tb->addAction(a);

    a = new QAction( this );
    a->setText( tr( "Page Setup..." ) );
    connect( a, SIGNAL( triggered() ), this, SLOT( pageSetup() ) );
    tb->addAction( a );

    tb->addSeparator();

    a = new QAction( QIcon( ":/images/zoomin.png" ), tr( "Zoom In" ), this );
    connect( a, SIGNAL( triggered() ), view, SLOT( zoomIn() ) );
    tb->addAction( a );

    a = new QAction(QIcon( ":/images/zoomout.png" ), tr( "Zoom Out" ), this );
    connect( a, SIGNAL( triggered() ), view, SLOT( zoomOut() ) );
    tb->addAction( a );

    tb->addSeparator();

    a = new QAction( this );
    a->setText( tr( "&Close" ) );
    connect( a, SIGNAL( triggered() ), this, SLOT( close() ) );
    tb->addAction( a );

    statusBar()->setSizeGripEnabled( true );
}

/*
	Setup page size
*/
void GasPrintPreview::setup()
{
    QSizeF page = printer.pageRect().size();
    page.setWidth( page.width() * view->logicalDpiX() / printer.logicalDpiX() );
    page.setHeight( page.height() * view->logicalDpiY() / printer.logicalDpiY() );
    doc->setPageSize( page );

	QTextFrameFormat fmt = doc->rootFrame()->frameFormat();
	fmt.setMargin( 0 );
	fmt.setPadding( 0 );
	doc->rootFrame()->setFrameFormat( fmt );

	//Margins do not work right in Qt 4.2.x.
	//This is a known issue and should be fixed for the next minor release (Qt 4.3.0)
	//See Task Tracker with ID 136041. http://www.trolltech.com/developer/task-tracker
	int rightMargin = qAbs( printer.paperRect().right() - printer.pageRect().right() );
	int bottomMargin = qAbs( printer.paperRect().bottom() - printer.pageRect().bottom() );
	int leftMargin = qAbs( printer.paperRect().left() - printer.pageRect().left() );
	int topMargin = qAbs( printer.paperRect().top() - printer.pageRect().top() );
	
	view->setLeftMargin( leftMargin );
	view->setTopMargin( topMargin );
	view->setRightMargin( rightMargin );
	view->setBottomMargin( bottomMargin );
}

GasPrintPreview::~GasPrintPreview()
{
    delete doc;
}

/*
	This routine prints the document
*/
void GasPrintPreview::print()
{
    QPrintDialog *dlg = new QPrintDialog( &printer, this );
    if ( dlg->exec() == QDialog::Accepted ) {
        doc->print( &printer );
    }
    delete dlg;
}

/*
	This routine provides a configuration dialog for the
	page-related options on a printer
*/
void GasPrintPreview::pageSetup()
{
    QPageSetupDialog dlg( &printer, this );
    if ( dlg.exec() == QDialog::Accepted ) {
        setup();
        view->updateLayout();
    }
}

#include "gasprintpreview.moc"

