/********************************************************************************
** Form generated from reading UI file 'gasgraphview.ui'
**
** Created by: Qt User Interface Compiler version 5.15.6
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
            GasGraphView->setObjectName(QString::fromUtf8("GasGraphView"));
        GasGraphView->resize(712, 400);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GasGraphView->sizePolicy().hasHeightForWidth());
        GasGraphView->setSizePolicy(sizePolicy);
        GasGraphView->setMinimumSize(QSize(0, 0));
        gridLayout = new QGridLayout(GasGraphView);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(8, 8, 8, 8);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(0);
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(0, 0, 0, 0);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        spacerItem = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);

        vboxLayout->addItem(spacerItem);

        lFGF = new QLabel(GasGraphView);
        lFGF->setObjectName(QString::fromUtf8("lFGF"));
        lFGF->setAlignment(Qt::AlignCenter);

        vboxLayout->addWidget(lFGF);

        spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);

        vboxLayout->addItem(spacerItem1);


        gridLayout->addLayout(vboxLayout, 1, 0, 1, 1);

        m_FGFGraph = new GasGraph(GasGraphView);
        m_FGFGraph->setObjectName(QString::fromUtf8("m_FGFGraph"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
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
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        spacerItem2 = new QSpacerItem(67, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);

        vboxLayout1->addItem(spacerItem2);

        lDEL = new QLabel(GasGraphView);
        lDEL->setObjectName(QString::fromUtf8("lDEL"));
        lDEL->setAlignment(Qt::AlignCenter);

        vboxLayout1->addWidget(lDEL);

        spacerItem3 = new QSpacerItem(67, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);

        vboxLayout1->addItem(spacerItem3);


        gridLayout->addLayout(vboxLayout1, 2, 0, 1, 1);

        m_DELGraph = new GasGraph(GasGraphView);
        m_DELGraph->setObjectName(QString::fromUtf8("m_DELGraph"));
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
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        agent = new QLabel(GasGraphView);
        agent->setObjectName(QString::fromUtf8("agent"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(agent->sizePolicy().hasHeightForWidth());
        agent->setSizePolicy(sizePolicy2);
        agent->setAlignment(Qt::AlignCenter|Qt::AlignHCenter|Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        vboxLayout2->addWidget(agent);

        groupBox = new QGroupBox(GasGraphView);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy3);
        vboxLayout3 = new QVBoxLayout(groupBox);
        vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
        checkBoxCKT = new QCheckBox(groupBox);
        checkBoxCKT->setObjectName(QString::fromUtf8("checkBoxCKT"));
        checkBoxCKT->setMinimumSize(QSize(0, 0));
        checkBoxCKT->setChecked(true);

        vboxLayout3->addWidget(checkBoxCKT);

        checkBoxALV = new QCheckBox(groupBox);
        checkBoxALV->setObjectName(QString::fromUtf8("checkBoxALV"));
        checkBoxALV->setChecked(true);

        vboxLayout3->addWidget(checkBoxALV);

        checkBoxVRG = new QCheckBox(groupBox);
        checkBoxVRG->setObjectName(QString::fromUtf8("checkBoxVRG"));
        checkBoxVRG->setChecked(true);

        vboxLayout3->addWidget(checkBoxVRG);

        checkBoxMUS = new QCheckBox(groupBox);
        checkBoxMUS->setObjectName(QString::fromUtf8("checkBoxMUS"));
        checkBoxMUS->setChecked(true);

        vboxLayout3->addWidget(checkBoxMUS);

        checkBoxFAT = new QCheckBox(groupBox);
        checkBoxFAT->setObjectName(QString::fromUtf8("checkBoxFAT"));
        checkBoxFAT->setChecked(true);

        vboxLayout3->addWidget(checkBoxFAT);


        vboxLayout2->addWidget(groupBox);


        gridLayout->addLayout(vboxLayout2, 3, 0, 1, 1);

        m_IARMGraph = new GasGraph(GasGraphView);
        m_IARMGraph->setObjectName(QString::fromUtf8("m_IARMGraph"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(2);
        sizePolicy4.setHeightForWidth(m_IARMGraph->sizePolicy().hasHeightForWidth());
        m_IARMGraph->setSizePolicy(sizePolicy4);
        m_IARMGraph->setCursor(QCursor(Qt::PointingHandCursor));
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

        spacerItem4 = new QSpacerItem(16, 182, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem4, 3, 2, 1, 1);

        CKTscroll = new QScrollBar(GasGraphView);
        CKTscroll->setObjectName(QString::fromUtf8("CKTscroll"));
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(CKTscroll->sizePolicy().hasHeightForWidth());
        CKTscroll->setSizePolicy(sizePolicy5);
        CKTscroll->setOrientation(Qt::Vertical);
        CKTscroll->setInvertedAppearance(true);

        gridLayout->addWidget(CKTscroll, 3, 4, 1, 1);

        viewComboBox = new QComboBox(GasGraphView);
        viewComboBox->setObjectName(QString::fromUtf8("viewComboBox"));

        gridLayout->addWidget(viewComboBox, 4, 0, 1, 1);

        hsbGraph = new QScrollBar(GasGraphView);
        hsbGraph->setObjectName(QString::fromUtf8("hsbGraph"));
        hsbGraph->setFocusPolicy(Qt::StrongFocus);
        hsbGraph->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(hsbGraph, 4, 1, 1, 1);

        spacerItem5 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem5, 4, 4, 1, 1);

        bt_coll = new QToolButton(GasGraphView);
        bt_coll->setObjectName(QString::fromUtf8("bt_coll"));
        QSizePolicy sizePolicy6(QSizePolicy::Fixed, QSizePolicy::Fixed);
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
