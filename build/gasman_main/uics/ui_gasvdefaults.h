/********************************************************************************
** Form generated from reading UI file 'gasvdefaults.ui'
**
** Created by: Qt User Interface Compiler version 5.15.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASVDEFAULTS_H
#define UI_GASVDEFAULTS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GasVDefaults
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *gbScale;
    QGridLayout *gridLayout;
    QLabel *lFGF;
    QSpinBox *sbCO;
    QLabel *lCO;
    QSpinBox *sbVA;
    QLabel *lVA;
    QSpinBox *sbFGF;
    QLabel *lView;
    QGridLayout *gridLayout1;
    QComboBox *cbTime;
    QCheckBox *cbShowCost;
    QCheckBox *cbEnableScrolling;
    QCheckBox *cbShowMl;
    QCheckBox *cbShowControl;
    QSpacerItem *spacerItem;

    void setupUi(QWidget *GasVDefaults)
    {
        if (GasVDefaults->objectName().isEmpty())
            GasVDefaults->setObjectName(QString::fromUtf8("GasVDefaults"));
        GasVDefaults->resize(280, 213);
        gridLayout_2 = new QGridLayout(GasVDefaults);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gbScale = new QGroupBox(GasVDefaults);
        gbScale->setObjectName(QString::fromUtf8("gbScale"));
        gridLayout = new QGridLayout(gbScale);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setContentsMargins(9, 9, 9, 9);
#endif
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lFGF = new QLabel(gbScale);
        lFGF->setObjectName(QString::fromUtf8("lFGF"));

        gridLayout->addWidget(lFGF, 0, 0, 1, 1);

        sbCO = new QSpinBox(gbScale);
        sbCO->setObjectName(QString::fromUtf8("sbCO"));

        gridLayout->addWidget(sbCO, 1, 2, 1, 1);

        lCO = new QLabel(gbScale);
        lCO->setObjectName(QString::fromUtf8("lCO"));

        gridLayout->addWidget(lCO, 0, 2, 1, 1);

        sbVA = new QSpinBox(gbScale);
        sbVA->setObjectName(QString::fromUtf8("sbVA"));

        gridLayout->addWidget(sbVA, 1, 1, 1, 1);

        lVA = new QLabel(gbScale);
        lVA->setObjectName(QString::fromUtf8("lVA"));

        gridLayout->addWidget(lVA, 0, 1, 1, 1);

        sbFGF = new QSpinBox(gbScale);
        sbFGF->setObjectName(QString::fromUtf8("sbFGF"));

        gridLayout->addWidget(sbFGF, 1, 0, 1, 1);


        gridLayout_2->addWidget(gbScale, 0, 0, 1, 1);

        lView = new QLabel(GasVDefaults);
        lView->setObjectName(QString::fromUtf8("lView"));

        gridLayout_2->addWidget(lView, 1, 0, 1, 1);

        gridLayout1 = new QGridLayout();
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        cbTime = new QComboBox(GasVDefaults);
        cbTime->setObjectName(QString::fromUtf8("cbTime"));

        gridLayout1->addWidget(cbTime, 0, 0, 1, 1);

        cbShowCost = new QCheckBox(GasVDefaults);
        cbShowCost->setObjectName(QString::fromUtf8("cbShowCost"));
        cbShowCost->setChecked(true);

        gridLayout1->addWidget(cbShowCost, 0, 1, 1, 1);

        cbEnableScrolling = new QCheckBox(GasVDefaults);
        cbEnableScrolling->setObjectName(QString::fromUtf8("cbEnableScrolling"));
        cbEnableScrolling->setChecked(true);

        gridLayout1->addWidget(cbEnableScrolling, 0, 2, 1, 1);

        cbShowMl = new QCheckBox(GasVDefaults);
        cbShowMl->setObjectName(QString::fromUtf8("cbShowMl"));
        cbShowMl->setChecked(true);

        gridLayout1->addWidget(cbShowMl, 1, 1, 1, 1);

        cbShowControl = new QCheckBox(GasVDefaults);
        cbShowControl->setObjectName(QString::fromUtf8("cbShowControl"));
        cbShowControl->setChecked(true);

        gridLayout1->addWidget(cbShowControl, 1, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout1, 2, 0, 1, 1);

        spacerItem = new QSpacerItem(254, 21, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(spacerItem, 3, 0, 1, 1);


        retranslateUi(GasVDefaults);

        QMetaObject::connectSlotsByName(GasVDefaults);
    } // setupUi

    void retranslateUi(QWidget *GasVDefaults)
    {
        GasVDefaults->setWindowTitle(QCoreApplication::translate("GasVDefaults", "Form", nullptr));
        gbScale->setTitle(QCoreApplication::translate("GasVDefaults", "Scale", nullptr));
        lFGF->setText(QCoreApplication::translate("GasVDefaults", "FGF", nullptr));
        lCO->setText(QCoreApplication::translate("GasVDefaults", "CO", nullptr));
        lVA->setText(QCoreApplication::translate("GasVDefaults", "VA", nullptr));
        lView->setText(QCoreApplication::translate("GasVDefaults", "View:", nullptr));
        cbShowCost->setText(QCoreApplication::translate("GasVDefaults", "Show Cost", nullptr));
        cbEnableScrolling->setText(QCoreApplication::translate("GasVDefaults", "Enable Scrolling", nullptr));
        cbShowMl->setText(QCoreApplication::translate("GasVDefaults", "Show mL", nullptr));
        cbShowControl->setText(QCoreApplication::translate("GasVDefaults", "Show Control Tray", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GasVDefaults: public Ui_GasVDefaults {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASVDEFAULTS_H
