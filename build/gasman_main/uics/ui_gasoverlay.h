/********************************************************************************
** Form generated from reading UI file 'gasoverlay.ui'
**
** Created by: Qt User Interface Compiler version 5.15.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASOVERLAY_H
#define UI_GASOVERLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "gasgraph.h"

QT_BEGIN_NAMESPACE

class Ui_GasOverlay
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    GasGraph *m_Graph;
    QVBoxLayout *vboxLayout1;
    QGridLayout *gridLayout;
    QLabel *lShow;
    QLabel *lOver;
    QRadioButton *rbDEL;
    QLabel *lDEL;
    QRadioButton *rbOverDEL;
    QRadioButton *rbCKT;
    QLabel *lCKT;
    QRadioButton *rbOverCKT;
    QRadioButton *rbALV;
    QLabel *lALV;
    QRadioButton *rbOverALV;
    QRadioButton *rbVRG;
    QLabel *lVRG;
    QRadioButton *rbOverVRG;
    QRadioButton *rbMUS;
    QLabel *lMUS;
    QRadioButton *rbOverMUS;
    QRadioButton *rbFAT;
    QLabel *lFAT;
    QRadioButton *rbOverFAT;
    QRadioButton *rbVEN;
    QLabel *lVEN;
    QRadioButton *rbOverVEN;
    QLabel *lMAC;
    QRadioButton *rbOverMAC;
    QLabel *lOne;
    QRadioButton *rbOverOne;
    QRadioButton *rbCost;
    QLabel *lCost;
    QSpacerItem *spacerItem;
    QLabel *lTime;
    QHBoxLayout *hboxLayout1;
    QLabel *lLegend;
    QVBoxLayout *vboxLayout2;
    QLabel *lGraph_0;
    QLabel *lGraph_1;
    QLabel *lGraph_2;
    QLabel *lGraph_3;
    QLabel *lGraph_4;
    QDialogButtonBox *buttonBox;

    void setupUi(QWidget *GasOverlay)
    {
        if (GasOverlay->objectName().isEmpty())
            GasOverlay->setObjectName(QString::fromUtf8("GasOverlay"));
        GasOverlay->resize(610, 478);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GasOverlay->sizePolicy().hasHeightForWidth());
        GasOverlay->setSizePolicy(sizePolicy);
        vboxLayout = new QVBoxLayout(GasOverlay);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        m_Graph = new GasGraph(GasOverlay);
        m_Graph->setObjectName(QString::fromUtf8("m_Graph"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_Graph->sizePolicy().hasHeightForWidth());
        m_Graph->setSizePolicy(sizePolicy1);
        m_Graph->setProperty("LMarginPct", QVariant(5));
        m_Graph->setProperty("TMarginPct", QVariant(5));
        m_Graph->setProperty("BMarginPct", QVariant(20));
        m_Graph->setProperty("RMarginPct", QVariant(17));

        hboxLayout->addWidget(m_Graph);

        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lShow = new QLabel(GasOverlay);
        lShow->setObjectName(QString::fromUtf8("lShow"));
        lShow->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(lShow, 0, 0, 1, 1);

        lOver = new QLabel(GasOverlay);
        lOver->setObjectName(QString::fromUtf8("lOver"));

        gridLayout->addWidget(lOver, 0, 2, 1, 1);

        rbDEL = new QRadioButton(GasOverlay);
        rbDEL->setObjectName(QString::fromUtf8("rbDEL"));
        rbDEL->setLayoutDirection(Qt::RightToLeft);

        gridLayout->addWidget(rbDEL, 1, 0, 1, 1);

        lDEL = new QLabel(GasOverlay);
        lDEL->setObjectName(QString::fromUtf8("lDEL"));
        lDEL->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lDEL, 1, 1, 1, 1);

        rbOverDEL = new QRadioButton(GasOverlay);
        rbOverDEL->setObjectName(QString::fromUtf8("rbOverDEL"));

        gridLayout->addWidget(rbOverDEL, 1, 2, 1, 1);

        rbCKT = new QRadioButton(GasOverlay);
        rbCKT->setObjectName(QString::fromUtf8("rbCKT"));
        rbCKT->setLayoutDirection(Qt::RightToLeft);

        gridLayout->addWidget(rbCKT, 2, 0, 1, 1);

        lCKT = new QLabel(GasOverlay);
        lCKT->setObjectName(QString::fromUtf8("lCKT"));
        lCKT->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lCKT, 2, 1, 1, 1);

        rbOverCKT = new QRadioButton(GasOverlay);
        rbOverCKT->setObjectName(QString::fromUtf8("rbOverCKT"));

        gridLayout->addWidget(rbOverCKT, 2, 2, 1, 1);

        rbALV = new QRadioButton(GasOverlay);
        rbALV->setObjectName(QString::fromUtf8("rbALV"));
        rbALV->setLayoutDirection(Qt::RightToLeft);

        gridLayout->addWidget(rbALV, 3, 0, 1, 1);

        lALV = new QLabel(GasOverlay);
        lALV->setObjectName(QString::fromUtf8("lALV"));
        lALV->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lALV, 3, 1, 1, 1);

        rbOverALV = new QRadioButton(GasOverlay);
        rbOverALV->setObjectName(QString::fromUtf8("rbOverALV"));

        gridLayout->addWidget(rbOverALV, 3, 2, 1, 1);

        rbVRG = new QRadioButton(GasOverlay);
        rbVRG->setObjectName(QString::fromUtf8("rbVRG"));
        rbVRG->setLayoutDirection(Qt::RightToLeft);

        gridLayout->addWidget(rbVRG, 4, 0, 1, 1);

        lVRG = new QLabel(GasOverlay);
        lVRG->setObjectName(QString::fromUtf8("lVRG"));
        lVRG->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lVRG, 4, 1, 1, 1);

        rbOverVRG = new QRadioButton(GasOverlay);
        rbOverVRG->setObjectName(QString::fromUtf8("rbOverVRG"));

        gridLayout->addWidget(rbOverVRG, 4, 2, 1, 1);

        rbMUS = new QRadioButton(GasOverlay);
        rbMUS->setObjectName(QString::fromUtf8("rbMUS"));
        rbMUS->setLayoutDirection(Qt::RightToLeft);

        gridLayout->addWidget(rbMUS, 5, 0, 1, 1);

        lMUS = new QLabel(GasOverlay);
        lMUS->setObjectName(QString::fromUtf8("lMUS"));
        lMUS->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lMUS, 5, 1, 1, 1);

        rbOverMUS = new QRadioButton(GasOverlay);
        rbOverMUS->setObjectName(QString::fromUtf8("rbOverMUS"));

        gridLayout->addWidget(rbOverMUS, 5, 2, 1, 1);

        rbFAT = new QRadioButton(GasOverlay);
        rbFAT->setObjectName(QString::fromUtf8("rbFAT"));
        rbFAT->setLayoutDirection(Qt::RightToLeft);

        gridLayout->addWidget(rbFAT, 6, 0, 1, 1);

        lFAT = new QLabel(GasOverlay);
        lFAT->setObjectName(QString::fromUtf8("lFAT"));
        lFAT->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lFAT, 6, 1, 1, 1);

        rbOverFAT = new QRadioButton(GasOverlay);
        rbOverFAT->setObjectName(QString::fromUtf8("rbOverFAT"));

        gridLayout->addWidget(rbOverFAT, 6, 2, 1, 1);

        rbVEN = new QRadioButton(GasOverlay);
        rbVEN->setObjectName(QString::fromUtf8("rbVEN"));
        rbVEN->setLayoutDirection(Qt::RightToLeft);

        gridLayout->addWidget(rbVEN, 7, 0, 1, 1);

        lVEN = new QLabel(GasOverlay);
        lVEN->setObjectName(QString::fromUtf8("lVEN"));
        lVEN->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lVEN, 7, 1, 1, 1);

        rbOverVEN = new QRadioButton(GasOverlay);
        rbOverVEN->setObjectName(QString::fromUtf8("rbOverVEN"));

        gridLayout->addWidget(rbOverVEN, 7, 2, 1, 1);

        lMAC = new QLabel(GasOverlay);
        lMAC->setObjectName(QString::fromUtf8("lMAC"));
        lMAC->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lMAC, 8, 1, 1, 1);

        rbOverMAC = new QRadioButton(GasOverlay);
        rbOverMAC->setObjectName(QString::fromUtf8("rbOverMAC"));

        gridLayout->addWidget(rbOverMAC, 8, 2, 1, 1);

        lOne = new QLabel(GasOverlay);
        lOne->setObjectName(QString::fromUtf8("lOne"));
        lOne->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lOne, 9, 1, 1, 1);

        rbOverOne = new QRadioButton(GasOverlay);
        rbOverOne->setObjectName(QString::fromUtf8("rbOverOne"));

        gridLayout->addWidget(rbOverOne, 9, 2, 1, 1);

        rbCost = new QRadioButton(GasOverlay);
        rbCost->setObjectName(QString::fromUtf8("rbCost"));
        rbCost->setLayoutDirection(Qt::RightToLeft);

        gridLayout->addWidget(rbCost, 10, 0, 1, 1);

        lCost = new QLabel(GasOverlay);
        lCost->setObjectName(QString::fromUtf8("lCost"));
        lCost->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lCost, 10, 1, 1, 1);


        vboxLayout1->addLayout(gridLayout);

        spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout1->addItem(spacerItem);


        hboxLayout->addLayout(vboxLayout1);


        vboxLayout->addLayout(hboxLayout);

        lTime = new QLabel(GasOverlay);
        lTime->setObjectName(QString::fromUtf8("lTime"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lTime->sizePolicy().hasHeightForWidth());
        lTime->setSizePolicy(sizePolicy2);
        lTime->setAlignment(Qt::AlignCenter);

        vboxLayout->addWidget(lTime);

        hboxLayout1 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout1->setSpacing(6);
#endif
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        lLegend = new QLabel(GasOverlay);
        lLegend->setObjectName(QString::fromUtf8("lLegend"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(lLegend->sizePolicy().hasHeightForWidth());
        lLegend->setSizePolicy(sizePolicy3);
        lLegend->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        hboxLayout1->addWidget(lLegend);

        vboxLayout2 = new QVBoxLayout();
#ifndef Q_OS_MAC
        vboxLayout2->setSpacing(6);
#endif
        vboxLayout2->setContentsMargins(0, 0, 0, 0);
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        lGraph_0 = new QLabel(GasOverlay);
        lGraph_0->setObjectName(QString::fromUtf8("lGraph_0"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(lGraph_0->sizePolicy().hasHeightForWidth());
        lGraph_0->setSizePolicy(sizePolicy4);

        vboxLayout2->addWidget(lGraph_0);

        lGraph_1 = new QLabel(GasOverlay);
        lGraph_1->setObjectName(QString::fromUtf8("lGraph_1"));

        vboxLayout2->addWidget(lGraph_1);

        lGraph_2 = new QLabel(GasOverlay);
        lGraph_2->setObjectName(QString::fromUtf8("lGraph_2"));

        vboxLayout2->addWidget(lGraph_2);

        lGraph_3 = new QLabel(GasOverlay);
        lGraph_3->setObjectName(QString::fromUtf8("lGraph_3"));

        vboxLayout2->addWidget(lGraph_3);

        lGraph_4 = new QLabel(GasOverlay);
        lGraph_4->setObjectName(QString::fromUtf8("lGraph_4"));

        vboxLayout2->addWidget(lGraph_4);


        hboxLayout1->addLayout(vboxLayout2);


        vboxLayout->addLayout(hboxLayout1);

        buttonBox = new QDialogButtonBox(GasOverlay);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        sizePolicy2.setHeightForWidth(buttonBox->sizePolicy().hasHeightForWidth());
        buttonBox->setSizePolicy(sizePolicy2);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close|QDialogButtonBox::Help);
        buttonBox->setCenterButtons(false);

        vboxLayout->addWidget(buttonBox);


        retranslateUi(GasOverlay);

        QMetaObject::connectSlotsByName(GasOverlay);
    } // setupUi

    void retranslateUi(QWidget *GasOverlay)
    {
        GasOverlay->setWindowTitle(QCoreApplication::translate("GasOverlay", "Overlay Comparison", nullptr));
        lShow->setText(QCoreApplication::translate("GasOverlay", "Show", nullptr));
        lOver->setText(QCoreApplication::translate("GasOverlay", "Over", nullptr));
        lDEL->setText(QCoreApplication::translate("GasOverlay", "DEL", nullptr));
        lCKT->setText(QCoreApplication::translate("GasOverlay", "CKT", nullptr));
        lALV->setText(QCoreApplication::translate("GasOverlay", "ALV", nullptr));
        lVRG->setText(QCoreApplication::translate("GasOverlay", "VRG", nullptr));
        lMUS->setText(QCoreApplication::translate("GasOverlay", "MUS", nullptr));
        lFAT->setText(QCoreApplication::translate("GasOverlay", "FAT", nullptr));
        lVEN->setText(QCoreApplication::translate("GasOverlay", "VEN", nullptr));
        lMAC->setText(QCoreApplication::translate("GasOverlay", "MAC", nullptr));
        lOne->setText(QCoreApplication::translate("GasOverlay", "One", nullptr));
        lCost->setText(QCoreApplication::translate("GasOverlay", "Cost", nullptr));
        lTime->setText(QCoreApplication::translate("GasOverlay", "Time (HH:mm)", nullptr));
        lLegend->setText(QCoreApplication::translate("GasOverlay", "Legend:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GasOverlay: public Ui_GasOverlay {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASOVERLAY_H
