/********************************************************************************
** Form generated from reading UI file 'gasgraph_test.ui'
**
** Created by: Qt User Interface Compiler version 5.15.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASGRAPH_TEST_H
#define UI_GASGRAPH_TEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "gasgraph.h"

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *vboxLayout;
    GasGraph *gasGraph;
    QScrollBar *hScrollBar;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(472, 307);
        Form->setContextMenuPolicy(Qt::NoContextMenu);
        vboxLayout = new QVBoxLayout(Form);
        vboxLayout->setSpacing(1);
        vboxLayout->setContentsMargins(3, 3, 3, 3);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        gasGraph = new GasGraph(Form);
        gasGraph->setObjectName(QString::fromUtf8("gasGraph"));
        gasGraph->setProperty("ScrollEnb", QVariant(true));
        gasGraph->setProperty("MAC", QVariant(3.000000000000000));
        gasGraph->setProperty("LineLabelType", QVariant(1));
        gasGraph->setProperty("Vals", QVariant(3));
        gasGraph->setProperty("XSize", QVariant(15.000000000000000));
        gasGraph->setProperty("XLabelEvery", QVariant(5.000000000000000));
        gasGraph->setProperty("XTickEvery", QVariant(1.000000000000000));
        gasGraph->setProperty("XValEvery", QVariant(0.500000000000000));
        gasGraph->setProperty("YLabelEvery", QVariant(5.000000000000000));
        gasGraph->setProperty("YSize", QVariant(10.000000000000000));
        gasGraph->setProperty("YTickEvery", QVariant(1.000000000000000));

        vboxLayout->addWidget(gasGraph);

        hScrollBar = new QScrollBar(Form);
        hScrollBar->setObjectName(QString::fromUtf8("hScrollBar"));
        hScrollBar->setMinimum(15);
        hScrollBar->setMaximum(30);
        hScrollBar->setPageStep(15);
        hScrollBar->setOrientation(Qt::Horizontal);

        vboxLayout->addWidget(hScrollBar);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASGRAPH_TEST_H
