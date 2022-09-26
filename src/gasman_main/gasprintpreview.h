
#ifndef GASPRINTPREVIEW_H
#define GASPRINTPREVIEW_H

#include <QMainWindow>
#if QT_VERSION >=0x050000
#include <QtPrintSupport/QPrinter>
#else
#include <QPrinter>
#endif


class GasPreviewView;
class QTextDocument;

class GasPrintPreview : public QMainWindow
{
    Q_OBJECT
public:
    GasPrintPreview( const QTextDocument *document, QPrinter *prn, QWidget *parent );
    virtual ~GasPrintPreview();

private slots:
    void print();			//Print the document
    void pageSetup();		//Draw the preview page

private:
    void setup();			//Setup the page parameters

    QTextDocument *doc;		//Clone of this text document
    GasPreviewView *view;
    QPrinter &printer;		//The printer where this will print
};

#endif //GASPRINTPREVIEW_H

