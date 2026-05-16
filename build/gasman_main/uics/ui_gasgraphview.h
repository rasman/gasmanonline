/********************************************************************************
** Form generated from reading UI file 'gasgraphview.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASGRAPHVIEW_H
#define UI_GASGRAPHVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "gasgraph.h"

QT_BEGIN_NAMESPACE

class Ui_GasGraphView
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *vboxLayout;
    QSpacerItem *spacerItem;
    QLabel *lFGF;
    QSpacerItem *spacerItem1;
    GasGraph *m_FGFGraph;
    QVBoxLayout *vboxLayout1;
    QSpacerItem *spacerItem2;
    QLabel *lDEL;
    QSpacerItem *spacerItem3;
    GasGraph *m_DELGraph;
    QVBoxLayout *vboxLayout2;
    QLabel *agent;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout3;
    QCheckBox *checkBoxCKT;
    QCheckBox *checkBoxALV;
    QCheckBox *checkBoxVRG;
    QCheckBox *checkBoxMUS;
    QCheckBox *checkBoxFAT;
    GasGraph *m_IARMGraph;
    QSpacerItem *spacerItem4;
    QScrollBar *CKTscroll;
    QComboBox *viewComboBox;
    QScrollBar *hsbGraph;
    QSpacerItem *spacerItem5;
    QToolButton *bt_coll;

    void setupUi(QWidget *GasGraphView)
    {
        if (GasGraphView->objectName().isEmpty())
            GasGraphView->setObjectName("GasGraphView");
        GasGraphView->resize(712, 400);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GasGraphView->sizePolicy().hasHeightForWidth());
        GasGraphView->setSizePolicy(sizePolicy);
        GasGraphView->setMinimumSize(QSize(0, 0));
        gridLayout = new QGridLayout(GasGraphView);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(8, 8, 8, 8);
        gridLayout->setObjectName("gridLayout");
        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(0);
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(0, 0, 0, 0);
#endif
        vboxLayout->setObjectName("vboxLayout");
        spacerItem = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        vboxLayout->addItem(spacerItem);

        lFGF = new QLabel(GasGraphView);
        lFGF->setObjectName("lFGF");
        lFGF->setAlignment(Qt::AlignCenter);

        vboxLayout->addWidget(lFGF);

        spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        vboxLayout->addItem(spacerItem1);


        gridLayout->addLayout(vboxLayout, 1, 0, 1, 1);

        m_FGFGraph = new GasGraph(GasGraphView);
        m_FGFGraph->setObjectName("m_FGFGraph");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(m_FGFGraph->sizePolicy().hasHeightForWidth());
        m_FGFGraph->setSizePolicy(sizePolicy1);
        m_FGFGraph->setProperty("RMarginPct", QVariant(3));
        m_FGFGraph->setProperty("TMarginPct", QVariant(5));
        m_FGFGraph->setProperty("BMarginPct", QVariant(30));
        m_FGFGraph->setProperty("LMarginPct", QVariant(17));

        gridLayout->addWidget(m_FGFGraph, 1, 1, 1, 1);

        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setSpacing(0);
        vboxLayout1->setContentsMargins(0, 0, 0, 0);
        vboxLayout1->setObjectName("vboxLayout1");
        spacerItem2 = new QSpacerItem(67, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        vboxLayout1->addItem(spacerItem2);

        lDEL = new QLabel(GasGraphView);
        lDEL->setObjectName("lDEL");
        lDEL->setAlignment(Qt::AlignCenter);

        vboxLayout1->addWidget(lDEL);

        spacerItem3 = new QSpacerItem(67, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        vboxLayout1->addItem(spacerItem3);


        gridLayout->addLayout(vboxLayout1, 2, 0, 1, 1);

        m_DELGraph = new GasGraph(GasGraphView);
        m_DELGraph->setObjectName("m_DELGraph");
        sizePolicy1.setHeightForWidth(m_DELGraph->sizePolicy().hasHeightForWidth());
        m_DELGraph->setSizePolicy(sizePolicy1);
        m_DELGraph->setProperty("RMarginPct", QVariant(3));
        m_DELGraph->setProperty("TMarginPct", QVariant(5));
        m_DELGraph->setProperty("BMarginPct", QVariant(30));
        m_DELGraph->setProperty("LMarginPct", QVariant(17));

        gridLayout->addWidget(m_DELGraph, 2, 1, 1, 1);

        vboxLayout2 = new QVBoxLayout();
        vboxLayout2->setSpacing(0);
        vboxLayout2->setContentsMargins(0, 0, 0, 0);
        vboxLayout2->setObjectName("vboxLayout2");
        agent = new QLabel(GasGraphView);
        agent->setObjectName("agent");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(agent->sizePolicy().hasHeightForWidth());
        agent->setSizePolicy(sizePolicy2);
        agent->setAlignment(Qt::AlignCenter|Qt::AlignHCenter|Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        vboxLayout2->addWidget(agent);

        groupBox = new QGroupBox(GasGraphView);
        groupBox->setObjectName("groupBox");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy3);
        vboxLayout3 = new QVBoxLayout(groupBox);
        vboxLayout3->setObjectName("vboxLayout3");
        checkBoxCKT = new QCheckBox(groupBox);
        checkBoxCKT->setObjectName("checkBoxCKT");
        checkBoxCKT->setMinimumSize(QSize(0, 0));
        checkBoxCKT->setChecked(true);

        vboxLayout3->addWidget(checkBoxCKT);

        checkBoxALV = new QCheckBox(groupBox);
        checkBoxALV->setObjectName("checkBoxALV");
        checkBoxALV->setChecked(true);

        vboxLayout3->addWidget(checkBoxALV);

        checkBoxVRG = new QCheckBox(groupBox);
        checkBoxVRG->setObjectName("checkBoxVRG");
        checkBoxVRG->setChecked(true);

        vboxLayout3->addWidget(checkBoxVRG);

        checkBoxMUS = new QCheckBox(groupBox);
        checkBoxMUS->setObjectName("checkBoxMUS");
        checkBoxMUS->setChecked(true);

        vboxLayout3->addWidget(checkBoxMUS);

        checkBoxFAT = new QCheckBox(groupBox);
        checkBoxFAT->setObjectName("checkBoxFAT");
        checkBoxFAT->setChecked(true);

        vboxLayout3->addWidget(checkBoxFAT);


        vboxLayout2->addWidget(groupBox);


        gridLayout->addLayout(vboxLayout2, 3, 0, 1, 1);

        m_IARMGraph = new GasGraph(GasGraphView);
        m_IARMGraph->setObjectName("m_IARMGraph");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(2);
        sizePolicy4.setHeightForWidth(m_IARMGraph->sizePolicy().hasHeightForWidth());
        m_IARMGraph->setSizePolicy(sizePolicy4);
        m_IARMGraph->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        m_IARMGraph->setProperty("RMarginPct", QVariant(3));
        m_IARMGraph->setProperty("TMarginPct", QVariant(5));
        m_IARMGraph->setProperty("BMarginPct", QVariant(20));
        m_IARMGraph->setProperty("LabelChar0", QVariant(70));
        m_IARMGraph->setProperty("LabelChar1", QVariant(77));
        m_IARMGraph->setProperty("LabelChar2", QVariant(82));
        m_IARMGraph->setProperty("LabelChar3", QVariant(65));
        m_IARMGraph->setProperty("LabelChar4", QVariant(73));
        m_IARMGraph->setProperty("Vals", QVariant(5));
        m_IARMGraph->setProperty("LMarginPct", QVariant(17));

        gridLayout->addWidget(m_IARMGraph, 3, 1, 1, 1);

        spacerItem4 = new QSpacerItem(16, 182, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(spacerItem4, 3, 2, 1, 1);

        CKTscroll = new QScrollBar(GasGraphView);
        CKTscroll->setObjectName("CKTscroll");
        QSizePolicy sizePolicy5(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(CKTscroll->sizePolicy().hasHeightForWidth());
        CKTscroll->setSizePolicy(sizePolicy5);
        CKTscroll->setOrientation(Qt::Vertical);
        CKTscroll->setInvertedAppearance(true);

        gridLayout->addWidget(CKTscroll, 3, 4, 1, 1);

        viewComboBox = new QComboBox(GasGraphView);
        viewComboBox->setObjectName("viewComboBox");

        gridLayout->addWidget(viewComboBox, 4, 0, 1, 1);

        hsbGraph = new QScrollBar(GasGraphView);
        hsbGraph->setObjectName("hsbGraph");
        hsbGraph->setFocusPolicy(Qt::StrongFocus);
        hsbGraph->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(hsbGraph, 4, 1, 1, 1);

        spacerItem5 = new QSpacerItem(20, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(spacerItem5, 4, 4, 1, 1);

        bt_coll = new QToolButton(GasGraphView);
        bt_coll->setObjectName("bt_coll");
        QSizePolicy sizePolicy6(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(bt_coll->sizePolicy().hasHeightForWidth());
        bt_coll->setSizePolicy(sizePolicy6);
        bt_coll->setMinimumSize(QSize(66, 0));
        bt_coll->setMaximumSize(QSize(16777215, 15));

        gridLayout->addWidget(bt_coll, 0, 0, 1, 2);


        retranslateUi(GasGraphView);

        QMetaObject::connectSlotsByName(GasGraphView);
    } // setupUi

    void retranslateUi(QWidget *GasGraphView)
    {
        GasGraphView->setWindowTitle(QCoreApplication::translate("GasGraphView", "Graphs", nullptr));
        lFGF->setText(QCoreApplication::translate("GasGraphView", "FGF\n"
"L/M", nullptr));
        lDEL->setText(QCoreApplication::translate("GasGraphView", "DEL\n"
"%atm", nullptr));
        groupBox->setTitle(QCoreApplication::translate("GasGraphView", "Show %atm", nullptr));
        checkBoxCKT->setText(QCoreApplication::translate("GasGraphView", "CKT(I)", nullptr));
        checkBoxALV->setText(QCoreApplication::translate("GasGraphView", "ALV(A)", nullptr));
        checkBoxVRG->setText(QCoreApplication::translate("GasGraphView", "VRG(R)", nullptr));
        checkBoxMUS->setText(QCoreApplication::translate("GasGraphView", "MUS(M)", nullptr));
        checkBoxFAT->setText(QCoreApplication::translate("GasGraphView", "FAT(F)", nullptr));
        bt_coll->setText(QCoreApplication::translate("GasGraphView", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GasGraphView: public Ui_GasGraphView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASGRAPHVIEW_H
