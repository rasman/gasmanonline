/********************************************************************************
** Form generated from reading UI file 'gascdefaults.ui'
**
** Created by: Qt User Interface Compiler version 5.15.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASCDEFAULTS_H
#define UI_GASCDEFAULTS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GasCDefaults
{
public:
    QHBoxLayout *hboxLayout;
    QGroupBox *groupBox;
    QHBoxLayout *hboxLayout1;
    QTableWidget *tableWidget;

    void setupUi(QWidget *GasCDefaults)
    {
        if (GasCDefaults->objectName().isEmpty())
            GasCDefaults->setObjectName(QString::fromUtf8("GasCDefaults"));
        GasCDefaults->resize(259, 304);
        hboxLayout = new QHBoxLayout(GasCDefaults);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        groupBox = new QGroupBox(GasCDefaults);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        hboxLayout1 = new QHBoxLayout(groupBox);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        tableWidget = new QTableWidget(groupBox);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(3);
        tableWidget->horizontalHeader()->setDefaultSectionSize(75);

        hboxLayout1->addWidget(tableWidget);


        hboxLayout->addWidget(groupBox);


        retranslateUi(GasCDefaults);

        QMetaObject::connectSlotsByName(GasCDefaults);
    } // setupUi

    void retranslateUi(QWidget *GasCDefaults)
    {
        GasCDefaults->setWindowTitle(QCoreApplication::translate("GasCDefaults", "Form", nullptr));
        groupBox->setTitle(QCoreApplication::translate("GasCDefaults", "Agent Defaults", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("GasCDefaults", "ml", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("GasCDefaults", "Cost", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("GasCDefaults", "Color", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GasCDefaults: public Ui_GasCDefaults {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASCDEFAULTS_H
