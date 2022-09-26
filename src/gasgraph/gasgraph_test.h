#ifndef GASGRAPHTEST_H
#define GASGRAPHTEST_H

#include "ui_gasgraph_test.h"

class TestForm : public QWidget , private Ui::Form
{
    Q_OBJECT
public:
    TestForm();
public slots:
    void on_hScrollBar_valueChanged ( int value );
protected:
    void customEvent ( QEvent * event );
};
#endif
