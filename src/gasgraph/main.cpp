#include <QApplication>

#include "gasgraph_test.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TestForm * form = new TestForm();
    form->setAttribute( Qt::WA_DeleteOnClose, true );
    //w.resize(600, 600);
    form->show();
    return app.exec();
}
