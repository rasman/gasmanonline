/********************************************************************************
** Form generated from reading UI file 'gaspanelview.ui'
**
** Created by: Qt User Interface Compiler version 5.15.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASPANELVIEW_H
#define UI_GASPANELVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "gasanibutton.h"
#include "gasgauge.h"
#include "gasmchn.h"

QT_BEGIN_NAMESPACE

class Ui_GasPanelView
{
public:
    QGridLayout *gridLayout;
    QFrame *frVA;
    QGridLayout *gridLayout1;
    QSpacerItem *spacerItem;
    QLineEdit *leHighVA;
    QGridLayout *gridLayout2;
    QToolButton *tbVAup;
    QSpacerItem *spacerItem1;
    QLineEdit *leVA;
    QSpacerItem *spacerItem2;
    QVBoxLayout *vboxLayout;
    QSpacerItem *spacerItem3;
    QLabel *stVA;
    QToolButton *tbVAdown;
    QSpacerItem *spacerItem4;
    QSpacerItem *spacerItem5;
    QFrame *frCO;
    QGridLayout *gridLayout3;
    QSpacerItem *spacerItem6;
    QGridLayout *gridLayout4;
    QToolButton *tbCOdown;
    QLineEdit *leCO;
    QToolButton *tbCOup;
    QSpacerItem *spacerItem7;
    QSpacerItem *spacerItem8;
    QSpacerItem *spacerItem9;
    QSpacerItem *spacerItem10;
    QSpacerItem *spacerItem11;
    QLineEdit *leHighCO;
    QLabel *lbFGF;
    QLabel *lbVA;
    QLabel *lbCO;
    GasGauge *ggCO;
    GasGauge *ggFGF;
    QFrame *frame_t;
    QGridLayout *gridLayout5;
    QLabel *leDelivered;
    QLabel *leUptake;
    QLabel *leTime;
    QLabel *lDelivered;
    QLabel *lUptake;
    QLabel *lTime;
    QFrame *frFlush;
    QGridLayout *gridLayout6;
    QPushButton *tbFlush;
    QSpacerItem *spacerItem12;
    QSpacerItem *spacerItem13;
    QLabel *lFlush;
    GasMchn *m_Gasmchn;
    GasAniButton *m_Srynge;
    GasGauge *ggDEL;
    GasGauge *ggVA;
    QFrame *frDEL;
    QGridLayout *gridLayout7;
    QSpacerItem *spacerItem14;
    QGridLayout *gridLayout8;
    QLineEdit *leDEL;
    QSpacerItem *spacerItem15;
    QSpacerItem *spacerItem16;
    QToolButton *tbDELup;
    QSpacerItem *spacerItem17;
    QToolButton *tbDELdown;
    QSpacerItem *spacerItem18;
    QSpacerItem *spacerItem19;
    QLineEdit *leHighDEL;
    QLabel *lbDEL;
    QFrame *frFGF;
    QGridLayout *gridLayout9;
    QGridLayout *gridLayout10;
    QToolButton *tbFGFdown;
    QSpacerItem *spacerItem20;
    QSpacerItem *spacerItem21;
    QLineEdit *leFGF;
    QSpacerItem *spacerItem22;
    QVBoxLayout *vboxLayout1;
    QSpacerItem *spacerItem23;
    QLabel *stFGF;
    QSpacerItem *spacerItem24;
    QToolButton *tbFGFup;
    QSpacerItem *spacerItem25;
    QLineEdit *leHighFGF;

    void setupUi(QWidget *GasPanelView)
    {
        if (GasPanelView->objectName().isEmpty())
            GasPanelView->setObjectName(QString::fromUtf8("GasPanelView"));
        GasPanelView->resize(643, 421);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GasPanelView->sizePolicy().hasHeightForWidth());
        GasPanelView->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(GasPanelView);
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(9, 8, 9, 8);
        frVA = new QFrame(GasPanelView);
        frVA->setObjectName(QString::fromUtf8("frVA"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frVA->sizePolicy().hasHeightForWidth());
        frVA->setSizePolicy(sizePolicy1);
        frVA->setFrameShape(QFrame::NoFrame);
        frVA->setFrameShadow(QFrame::Raised);
        gridLayout1 = new QGridLayout(frVA);
        gridLayout1->setSpacing(0);
        gridLayout1->setContentsMargins(1, 1, 1, 1);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        spacerItem = new QSpacerItem(20, 71, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout1->addItem(spacerItem, 1, 0, 1, 1);

        leHighVA = new QLineEdit(frVA);
        leHighVA->setObjectName(QString::fromUtf8("leHighVA"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(leHighVA->sizePolicy().hasHeightForWidth());
        leHighVA->setSizePolicy(sizePolicy2);
        leHighVA->setFocusPolicy(Qt::StrongFocus);

        gridLayout1->addWidget(leHighVA, 0, 0, 1, 1);

        gridLayout2 = new QGridLayout();
        gridLayout2->setSpacing(0);
        gridLayout2->setContentsMargins(0, 0, 0, 0);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        tbVAup = new QToolButton(frVA);
        tbVAup->setObjectName(QString::fromUtf8("tbVAup"));
        tbVAup->setMinimumSize(QSize(15, 15));
        tbVAup->setMaximumSize(QSize(15, 15));
        tbVAup->setFocusPolicy(Qt::NoFocus);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/up.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbVAup->setIcon(icon);
        tbVAup->setAutoRepeat(true);

        gridLayout2->addWidget(tbVAup, 0, 1, 1, 1);

        spacerItem1 = new QSpacerItem(1, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout2->addItem(spacerItem1, 0, 2, 1, 1);

        leVA = new QLineEdit(frVA);
        leVA->setObjectName(QString::fromUtf8("leVA"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(leVA->sizePolicy().hasHeightForWidth());
        leVA->setSizePolicy(sizePolicy3);

        gridLayout2->addWidget(leVA, 1, 0, 1, 4);

        spacerItem2 = new QSpacerItem(1, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout2->addItem(spacerItem2, 0, 0, 1, 1);

        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(0);
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        spacerItem3 = new QSpacerItem(15, 3, QSizePolicy::Fixed, QSizePolicy::Minimum);

        vboxLayout->addItem(spacerItem3);

        stVA = new QLabel(frVA);
        stVA->setObjectName(QString::fromUtf8("stVA"));
        sizePolicy2.setHeightForWidth(stVA->sizePolicy().hasHeightForWidth());
        stVA->setSizePolicy(sizePolicy2);
        stVA->setMinimumSize(QSize(15, 15));
        stVA->setMaximumSize(QSize(15, 15));
        stVA->setMouseTracking(true);
        stVA->setFocusPolicy(Qt::NoFocus);
        stVA->setPixmap(QPixmap(QString::fromUtf8(":/images/star.png")));
        stVA->setScaledContents(true);

        vboxLayout->addWidget(stVA);


        gridLayout2->addLayout(vboxLayout, 0, 3, 1, 1);

        tbVAdown = new QToolButton(frVA);
        tbVAdown->setObjectName(QString::fromUtf8("tbVAdown"));
        tbVAdown->setMinimumSize(QSize(15, 15));
        tbVAdown->setMaximumSize(QSize(15, 15));
        tbVAdown->setFocusPolicy(Qt::NoFocus);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/down.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbVAdown->setIcon(icon1);
        tbVAdown->setAutoRepeat(true);

        gridLayout2->addWidget(tbVAdown, 2, 1, 1, 1);

        spacerItem4 = new QSpacerItem(39, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout2->addItem(spacerItem4, 2, 0, 1, 1);

        spacerItem5 = new QSpacerItem(54, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout2->addItem(spacerItem5, 2, 2, 1, 2);


        gridLayout1->addLayout(gridLayout2, 2, 0, 1, 1);


        gridLayout->addWidget(frVA, 2, 4, 1, 1);

        frCO = new QFrame(GasPanelView);
        frCO->setObjectName(QString::fromUtf8("frCO"));
        sizePolicy1.setHeightForWidth(frCO->sizePolicy().hasHeightForWidth());
        frCO->setSizePolicy(sizePolicy1);
        frCO->setFrameShape(QFrame::NoFrame);
        frCO->setFrameShadow(QFrame::Raised);
        gridLayout3 = new QGridLayout(frCO);
        gridLayout3->setSpacing(0);
        gridLayout3->setContentsMargins(1, 1, 1, 1);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        spacerItem6 = new QSpacerItem(16, 100, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout3->addItem(spacerItem6, 1, 0, 1, 1);

        gridLayout4 = new QGridLayout();
        gridLayout4->setSpacing(0);
#ifndef Q_OS_MAC
        gridLayout4->setContentsMargins(0, 0, 0, 0);
#endif
        gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
        tbCOdown = new QToolButton(frCO);
        tbCOdown->setObjectName(QString::fromUtf8("tbCOdown"));
        sizePolicy2.setHeightForWidth(tbCOdown->sizePolicy().hasHeightForWidth());
        tbCOdown->setSizePolicy(sizePolicy2);
        tbCOdown->setMinimumSize(QSize(15, 15));
        tbCOdown->setMaximumSize(QSize(15, 15));
        tbCOdown->setFocusPolicy(Qt::NoFocus);
        tbCOdown->setIcon(icon1);
        tbCOdown->setAutoRepeat(true);

        gridLayout4->addWidget(tbCOdown, 2, 1, 1, 1);

        leCO = new QLineEdit(frCO);
        leCO->setObjectName(QString::fromUtf8("leCO"));
        sizePolicy3.setHeightForWidth(leCO->sizePolicy().hasHeightForWidth());
        leCO->setSizePolicy(sizePolicy3);

        gridLayout4->addWidget(leCO, 1, 0, 1, 4);

        tbCOup = new QToolButton(frCO);
        tbCOup->setObjectName(QString::fromUtf8("tbCOup"));
        sizePolicy2.setHeightForWidth(tbCOup->sizePolicy().hasHeightForWidth());
        tbCOup->setSizePolicy(sizePolicy2);
        tbCOup->setMinimumSize(QSize(15, 15));
        tbCOup->setMaximumSize(QSize(15, 15));
        tbCOup->setFocusPolicy(Qt::NoFocus);
        tbCOup->setIcon(icon);
        tbCOup->setAutoRepeat(true);

        gridLayout4->addWidget(tbCOup, 0, 1, 1, 1);

        spacerItem7 = new QSpacerItem(1, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout4->addItem(spacerItem7, 0, 0, 1, 1);

        spacerItem8 = new QSpacerItem(1, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout4->addItem(spacerItem8, 2, 0, 1, 1);

        spacerItem9 = new QSpacerItem(1, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout4->addItem(spacerItem9, 0, 2, 1, 1);

        spacerItem10 = new QSpacerItem(15, 15, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout4->addItem(spacerItem10, 0, 3, 1, 1);

        spacerItem11 = new QSpacerItem(1, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout4->addItem(spacerItem11, 2, 2, 1, 2);


        gridLayout3->addLayout(gridLayout4, 2, 0, 1, 1);

        leHighCO = new QLineEdit(frCO);
        leHighCO->setObjectName(QString::fromUtf8("leHighCO"));
        sizePolicy2.setHeightForWidth(leHighCO->sizePolicy().hasHeightForWidth());
        leHighCO->setSizePolicy(sizePolicy2);
        leHighCO->setFocusPolicy(Qt::StrongFocus);

        gridLayout3->addWidget(leHighCO, 0, 0, 1, 1);


        gridLayout->addWidget(frCO, 2, 6, 1, 1);

        lbFGF = new QLabel(GasPanelView);
        lbFGF->setObjectName(QString::fromUtf8("lbFGF"));
        sizePolicy2.setHeightForWidth(lbFGF->sizePolicy().hasHeightForWidth());
        lbFGF->setSizePolicy(sizePolicy2);
        lbFGF->setFrameShape(QFrame::NoFrame);
        lbFGF->setTextFormat(Qt::AutoText);
        lbFGF->setWordWrap(false);

        gridLayout->addWidget(lbFGF, 3, 0, 1, 1);

        lbVA = new QLabel(GasPanelView);
        lbVA->setObjectName(QString::fromUtf8("lbVA"));
        sizePolicy2.setHeightForWidth(lbVA->sizePolicy().hasHeightForWidth());
        lbVA->setSizePolicy(sizePolicy2);
        lbVA->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lbVA, 3, 4, 1, 1);

        lbCO = new QLabel(GasPanelView);
        lbCO->setObjectName(QString::fromUtf8("lbCO"));
        sizePolicy2.setHeightForWidth(lbCO->sizePolicy().hasHeightForWidth());
        lbCO->setSizePolicy(sizePolicy2);
        lbCO->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lbCO, 3, 6, 1, 1);

        ggCO = new GasGauge(GasPanelView);
        ggCO->setObjectName(QString::fromUtf8("ggCO"));
        sizePolicy1.setHeightForWidth(ggCO->sizePolicy().hasHeightForWidth());
        ggCO->setSizePolicy(sizePolicy1);
        ggCO->setMinimumSize(QSize(15, 20));
        ggCO->setMaximumSize(QSize(45, 16777215));

        gridLayout->addWidget(ggCO, 2, 7, 1, 1);

        ggFGF = new GasGauge(GasPanelView);
        ggFGF->setObjectName(QString::fromUtf8("ggFGF"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(ggFGF->sizePolicy().hasHeightForWidth());
        ggFGF->setSizePolicy(sizePolicy4);
        ggFGF->setMinimumSize(QSize(15, 20));
        ggFGF->setMaximumSize(QSize(45, 16777215));

        gridLayout->addWidget(ggFGF, 2, 3, 1, 1);

        frame_t = new QFrame(GasPanelView);
        frame_t->setObjectName(QString::fromUtf8("frame_t"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(1);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(frame_t->sizePolicy().hasHeightForWidth());
        frame_t->setSizePolicy(sizePolicy5);
        frame_t->setFrameShape(QFrame::NoFrame);
        frame_t->setFrameShadow(QFrame::Raised);
        gridLayout5 = new QGridLayout(frame_t);
#ifndef Q_OS_MAC
        gridLayout5->setSpacing(6);
#endif
        gridLayout5->setContentsMargins(1, 1, 1, 1);
        gridLayout5->setObjectName(QString::fromUtf8("gridLayout5"));
        leDelivered = new QLabel(frame_t);
        leDelivered->setObjectName(QString::fromUtf8("leDelivered"));
        sizePolicy3.setHeightForWidth(leDelivered->sizePolicy().hasHeightForWidth());
        leDelivered->setSizePolicy(sizePolicy3);
        leDelivered->setFrameShape(QFrame::Panel);
        leDelivered->setFrameShadow(QFrame::Sunken);
        leDelivered->setAlignment(Qt::AlignCenter);

        gridLayout5->addWidget(leDelivered, 2, 0, 1, 1);

        leUptake = new QLabel(frame_t);
        leUptake->setObjectName(QString::fromUtf8("leUptake"));
        sizePolicy3.setHeightForWidth(leUptake->sizePolicy().hasHeightForWidth());
        leUptake->setSizePolicy(sizePolicy3);
        leUptake->setFrameShape(QFrame::Panel);
        leUptake->setFrameShadow(QFrame::Sunken);
        leUptake->setAlignment(Qt::AlignCenter);

        gridLayout5->addWidget(leUptake, 1, 0, 1, 1);

        leTime = new QLabel(frame_t);
        leTime->setObjectName(QString::fromUtf8("leTime"));
        sizePolicy3.setHeightForWidth(leTime->sizePolicy().hasHeightForWidth());
        leTime->setSizePolicy(sizePolicy3);
        leTime->setFrameShape(QFrame::Panel);
        leTime->setFrameShadow(QFrame::Sunken);
        leTime->setLineWidth(1);
        leTime->setMidLineWidth(0);
        leTime->setAlignment(Qt::AlignCenter);

        gridLayout5->addWidget(leTime, 0, 0, 1, 1);

        lDelivered = new QLabel(frame_t);
        lDelivered->setObjectName(QString::fromUtf8("lDelivered"));
        QSizePolicy sizePolicy6(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(lDelivered->sizePolicy().hasHeightForWidth());
        lDelivered->setSizePolicy(sizePolicy6);

        gridLayout5->addWidget(lDelivered, 2, 1, 1, 1);

        lUptake = new QLabel(frame_t);
        lUptake->setObjectName(QString::fromUtf8("lUptake"));
        sizePolicy6.setHeightForWidth(lUptake->sizePolicy().hasHeightForWidth());
        lUptake->setSizePolicy(sizePolicy6);

        gridLayout5->addWidget(lUptake, 1, 1, 1, 1);

        lTime = new QLabel(frame_t);
        lTime->setObjectName(QString::fromUtf8("lTime"));
        sizePolicy1.setHeightForWidth(lTime->sizePolicy().hasHeightForWidth());
        lTime->setSizePolicy(sizePolicy1);

        gridLayout5->addWidget(lTime, 0, 1, 1, 1);


        gridLayout->addWidget(frame_t, 2, 8, 1, 1);

        frFlush = new QFrame(GasPanelView);
        frFlush->setObjectName(QString::fromUtf8("frFlush"));
        sizePolicy2.setHeightForWidth(frFlush->sizePolicy().hasHeightForWidth());
        frFlush->setSizePolicy(sizePolicy2);
        frFlush->setFrameShape(QFrame::NoFrame);
        frFlush->setFrameShadow(QFrame::Raised);
        gridLayout6 = new QGridLayout(frFlush);
        gridLayout6->setSpacing(3);
        gridLayout6->setContentsMargins(1, 1, 1, 1);
        gridLayout6->setObjectName(QString::fromUtf8("gridLayout6"));
        tbFlush = new QPushButton(frFlush);
        tbFlush->setObjectName(QString::fromUtf8("tbFlush"));
        sizePolicy2.setHeightForWidth(tbFlush->sizePolicy().hasHeightForWidth());
        tbFlush->setSizePolicy(sizePolicy2);
        tbFlush->setMinimumSize(QSize(15, 15));
        tbFlush->setMaximumSize(QSize(15, 15));
        tbFlush->setFocusPolicy(Qt::TabFocus);

        gridLayout6->addWidget(tbFlush, 1, 1, 1, 1);

        spacerItem12 = new QSpacerItem(1, 10, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout6->addItem(spacerItem12, 1, 0, 1, 1);

        spacerItem13 = new QSpacerItem(1, 10, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout6->addItem(spacerItem13, 1, 2, 1, 1);

        lFlush = new QLabel(frFlush);
        lFlush->setObjectName(QString::fromUtf8("lFlush"));
        sizePolicy.setHeightForWidth(lFlush->sizePolicy().hasHeightForWidth());
        lFlush->setSizePolicy(sizePolicy);
        lFlush->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);

        gridLayout6->addWidget(lFlush, 0, 0, 1, 3);


        gridLayout->addWidget(frFlush, 0, 2, 1, 1);

        m_Gasmchn = new GasMchn(GasPanelView);
        m_Gasmchn->setObjectName(QString::fromUtf8("m_Gasmchn"));
        QSizePolicy sizePolicy7(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(m_Gasmchn->sizePolicy().hasHeightForWidth());
        m_Gasmchn->setSizePolicy(sizePolicy7);
        m_Gasmchn->setMinimumSize(QSize(310, 150));
        m_Gasmchn->setMaximumSize(QSize(16777215, 300));
        m_Srynge = new GasAniButton(m_Gasmchn);
        m_Srynge->setObjectName(QString::fromUtf8("m_Srynge"));
        m_Srynge->setGeometry(QRect(0, 0, 32, 32));
        QSizePolicy sizePolicy8(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(m_Srynge->sizePolicy().hasHeightForWidth());
        m_Srynge->setSizePolicy(sizePolicy8);
        m_Srynge->setFocusPolicy(Qt::StrongFocus);
        m_Srynge->setProperty("DownPixmap", QVariant(QPixmap(QString::fromUtf8(":/images/srynge_down.png"))));
        m_Srynge->setProperty("UpPixmap", QVariant(QPixmap(QString::fromUtf8(":/images/srynge_up.png"))));

        gridLayout->addWidget(m_Gasmchn, 0, 3, 2, 6);

        ggDEL = new GasGauge(GasPanelView);
        ggDEL->setObjectName(QString::fromUtf8("ggDEL"));
        sizePolicy8.setHeightForWidth(ggDEL->sizePolicy().hasHeightForWidth());
        ggDEL->setSizePolicy(sizePolicy8);
        ggDEL->setMinimumSize(QSize(15, 20));
        ggDEL->setMaximumSize(QSize(45, 16777215));
        ggDEL->setProperty("BarColor", QVariant(QColor(0, 255, 0)));

        gridLayout->addWidget(ggDEL, 0, 1, 1, 1);

        ggVA = new GasGauge(GasPanelView);
        ggVA->setObjectName(QString::fromUtf8("ggVA"));
        sizePolicy1.setHeightForWidth(ggVA->sizePolicy().hasHeightForWidth());
        ggVA->setSizePolicy(sizePolicy1);
        ggVA->setMinimumSize(QSize(15, 20));
        ggVA->setMaximumSize(QSize(45, 16777215));

        gridLayout->addWidget(ggVA, 2, 5, 1, 1);

        frDEL = new QFrame(GasPanelView);
        frDEL->setObjectName(QString::fromUtf8("frDEL"));
        sizePolicy6.setHeightForWidth(frDEL->sizePolicy().hasHeightForWidth());
        frDEL->setSizePolicy(sizePolicy6);
        frDEL->setFrameShape(QFrame::NoFrame);
        frDEL->setFrameShadow(QFrame::Sunken);
        gridLayout7 = new QGridLayout(frDEL);
        gridLayout7->setSpacing(0);
        gridLayout7->setContentsMargins(1, 1, 1, 1);
        gridLayout7->setObjectName(QString::fromUtf8("gridLayout7"));
        spacerItem14 = new QSpacerItem(21, 21, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout7->addItem(spacerItem14, 1, 0, 1, 1);

        gridLayout8 = new QGridLayout();
        gridLayout8->setSpacing(0);
#ifndef Q_OS_MAC
        gridLayout8->setContentsMargins(0, 0, 0, 0);
#endif
        gridLayout8->setObjectName(QString::fromUtf8("gridLayout8"));
        leDEL = new QLineEdit(frDEL);
        leDEL->setObjectName(QString::fromUtf8("leDEL"));
        QSizePolicy sizePolicy9(QSizePolicy::Ignored, QSizePolicy::Fixed);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(leDEL->sizePolicy().hasHeightForWidth());
        leDEL->setSizePolicy(sizePolicy9);

        gridLayout8->addWidget(leDEL, 1, 0, 1, 3);

        spacerItem15 = new QSpacerItem(1, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout8->addItem(spacerItem15, 0, 0, 1, 1);

        spacerItem16 = new QSpacerItem(1, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout8->addItem(spacerItem16, 0, 2, 1, 1);

        tbDELup = new QToolButton(frDEL);
        tbDELup->setObjectName(QString::fromUtf8("tbDELup"));
        tbDELup->setMinimumSize(QSize(15, 15));
        tbDELup->setMaximumSize(QSize(15, 15));
        tbDELup->setFocusPolicy(Qt::NoFocus);
        tbDELup->setIcon(icon);
        tbDELup->setAutoRepeat(true);

        gridLayout8->addWidget(tbDELup, 0, 1, 1, 1);

        spacerItem17 = new QSpacerItem(1, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout8->addItem(spacerItem17, 2, 0, 1, 1);

        tbDELdown = new QToolButton(frDEL);
        tbDELdown->setObjectName(QString::fromUtf8("tbDELdown"));
        tbDELdown->setMinimumSize(QSize(15, 15));
        tbDELdown->setMaximumSize(QSize(15, 15));
        tbDELdown->setFocusPolicy(Qt::NoFocus);
        tbDELdown->setIcon(icon1);
        tbDELdown->setAutoRepeat(true);

        gridLayout8->addWidget(tbDELdown, 2, 1, 1, 1);

        spacerItem18 = new QSpacerItem(1, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout8->addItem(spacerItem18, 2, 2, 1, 2);

        spacerItem19 = new QSpacerItem(15, 15, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout8->addItem(spacerItem19, 0, 3, 1, 1);


        gridLayout7->addLayout(gridLayout8, 2, 0, 1, 1);

        leHighDEL = new QLineEdit(frDEL);
        leHighDEL->setObjectName(QString::fromUtf8("leHighDEL"));
        sizePolicy9.setHeightForWidth(leHighDEL->sizePolicy().hasHeightForWidth());
        leHighDEL->setSizePolicy(sizePolicy9);
        leHighDEL->setFocusPolicy(Qt::StrongFocus);

        gridLayout7->addWidget(leHighDEL, 0, 0, 1, 1);


        gridLayout->addWidget(frDEL, 0, 0, 1, 1);

        lbDEL = new QLabel(GasPanelView);
        lbDEL->setObjectName(QString::fromUtf8("lbDEL"));
        sizePolicy.setHeightForWidth(lbDEL->sizePolicy().hasHeightForWidth());
        lbDEL->setSizePolicy(sizePolicy);
        lbDEL->setMinimumSize(QSize(80, 30));
        lbDEL->setLayoutDirection(Qt::LeftToRight);
        lbDEL->setFrameShape(QFrame::NoFrame);
        lbDEL->setFrameShadow(QFrame::Plain);
        lbDEL->setTextFormat(Qt::AutoText);
        lbDEL->setAlignment(Qt::AlignCenter|Qt::AlignHCenter|Qt::AlignTop|Qt::AlignVCenter);

        gridLayout->addWidget(lbDEL, 1, 0, 1, 3);

        frFGF = new QFrame(GasPanelView);
        frFGF->setObjectName(QString::fromUtf8("frFGF"));
        sizePolicy1.setHeightForWidth(frFGF->sizePolicy().hasHeightForWidth());
        frFGF->setSizePolicy(sizePolicy1);
        frFGF->setFrameShape(QFrame::NoFrame);
        frFGF->setFrameShadow(QFrame::Raised);
        gridLayout9 = new QGridLayout(frFGF);
        gridLayout9->setSpacing(0);
        gridLayout9->setContentsMargins(1, 1, 1, 1);
        gridLayout9->setObjectName(QString::fromUtf8("gridLayout9"));
        gridLayout10 = new QGridLayout();
        gridLayout10->setSpacing(0);
#ifndef Q_OS_MAC
        gridLayout10->setContentsMargins(0, 0, 0, 0);
#endif
        gridLayout10->setObjectName(QString::fromUtf8("gridLayout10"));
        tbFGFdown = new QToolButton(frFGF);
        tbFGFdown->setObjectName(QString::fromUtf8("tbFGFdown"));
        sizePolicy2.setHeightForWidth(tbFGFdown->sizePolicy().hasHeightForWidth());
        tbFGFdown->setSizePolicy(sizePolicy2);
        tbFGFdown->setMinimumSize(QSize(15, 15));
        tbFGFdown->setMaximumSize(QSize(15, 15));
        tbFGFdown->setFocusPolicy(Qt::NoFocus);
        tbFGFdown->setIcon(icon1);
        tbFGFdown->setAutoRepeat(true);

        gridLayout10->addWidget(tbFGFdown, 2, 1, 1, 1);

        spacerItem20 = new QSpacerItem(1, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout10->addItem(spacerItem20, 0, 2, 1, 1);

        spacerItem21 = new QSpacerItem(1, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout10->addItem(spacerItem21, 2, 0, 1, 1);

        leFGF = new QLineEdit(frFGF);
        leFGF->setObjectName(QString::fromUtf8("leFGF"));
        sizePolicy3.setHeightForWidth(leFGF->sizePolicy().hasHeightForWidth());
        leFGF->setSizePolicy(sizePolicy3);

        gridLayout10->addWidget(leFGF, 1, 0, 1, 4);

        spacerItem22 = new QSpacerItem(1, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout10->addItem(spacerItem22, 0, 0, 1, 1);

        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setSpacing(0);
        vboxLayout1->setContentsMargins(0, 0, 0, 0);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        spacerItem23 = new QSpacerItem(15, 3, QSizePolicy::Fixed, QSizePolicy::Minimum);

        vboxLayout1->addItem(spacerItem23);

        stFGF = new QLabel(frFGF);
        stFGF->setObjectName(QString::fromUtf8("stFGF"));
        sizePolicy2.setHeightForWidth(stFGF->sizePolicy().hasHeightForWidth());
        stFGF->setSizePolicy(sizePolicy2);
        stFGF->setMinimumSize(QSize(15, 15));
        stFGF->setMaximumSize(QSize(15, 15));
        stFGF->setPixmap(QPixmap(QString::fromUtf8(":/images/star.png")));
        stFGF->setScaledContents(true);

        vboxLayout1->addWidget(stFGF);


        gridLayout10->addLayout(vboxLayout1, 0, 3, 1, 1);

        spacerItem24 = new QSpacerItem(1, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout10->addItem(spacerItem24, 2, 2, 1, 2);

        tbFGFup = new QToolButton(frFGF);
        tbFGFup->setObjectName(QString::fromUtf8("tbFGFup"));
        sizePolicy2.setHeightForWidth(tbFGFup->sizePolicy().hasHeightForWidth());
        tbFGFup->setSizePolicy(sizePolicy2);
        tbFGFup->setMinimumSize(QSize(15, 15));
        tbFGFup->setMaximumSize(QSize(15, 15));
        tbFGFup->setFocusPolicy(Qt::NoFocus);
        tbFGFup->setIcon(icon);
        tbFGFup->setAutoRepeat(true);

        gridLayout10->addWidget(tbFGFup, 0, 1, 1, 1);


        gridLayout9->addLayout(gridLayout10, 2, 0, 1, 1);

        spacerItem25 = new QSpacerItem(21, 81, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout9->addItem(spacerItem25, 1, 0, 1, 1);

        leHighFGF = new QLineEdit(frFGF);
        leHighFGF->setObjectName(QString::fromUtf8("leHighFGF"));
        sizePolicy2.setHeightForWidth(leHighFGF->sizePolicy().hasHeightForWidth());
        leHighFGF->setSizePolicy(sizePolicy2);
        leHighFGF->setFocusPolicy(Qt::StrongFocus);

        gridLayout9->addWidget(leHighFGF, 0, 0, 1, 1);


        gridLayout->addWidget(frFGF, 2, 0, 1, 3);

        QWidget::setTabOrder(leHighDEL, leDEL);
        QWidget::setTabOrder(leDEL, leHighFGF);
        QWidget::setTabOrder(leHighFGF, leFGF);
        QWidget::setTabOrder(leFGF, leHighVA);
        QWidget::setTabOrder(leHighVA, leVA);
        QWidget::setTabOrder(leVA, leHighCO);
        QWidget::setTabOrder(leHighCO, leCO);
        QWidget::setTabOrder(leCO, m_Srynge);
        QWidget::setTabOrder(m_Srynge, tbFlush);

        retranslateUi(GasPanelView);

        QMetaObject::connectSlotsByName(GasPanelView);
    } // setupUi

    void retranslateUi(QWidget *GasPanelView)
    {
        GasPanelView->setWindowTitle(QCoreApplication::translate("GasPanelView", "Panel", nullptr));
        tbVAup->setText(QCoreApplication::translate("GasPanelView", "...", nullptr));
        stVA->setText(QString());
        tbVAdown->setText(QCoreApplication::translate("GasPanelView", "...", nullptr));
        tbCOdown->setText(QCoreApplication::translate("GasPanelView", "...", nullptr));
        tbCOup->setText(QCoreApplication::translate("GasPanelView", "...", nullptr));
        lbFGF->setText(QCoreApplication::translate("GasPanelView", "FGF  L/m", nullptr));
        lbVA->setText(QCoreApplication::translate("GasPanelView", "VA  L/m", nullptr));
        lbCO->setText(QCoreApplication::translate("GasPanelView", "CO  L/m", nullptr));
        leDelivered->setText(QCoreApplication::translate("GasPanelView", "0.00", nullptr));
        leUptake->setText(QCoreApplication::translate("GasPanelView", "0.00", nullptr));
        leTime->setText(QCoreApplication::translate("GasPanelView", "00:00:00", nullptr));
        lDelivered->setText(QCoreApplication::translate("GasPanelView", "Delivered (L)", nullptr));
        lUptake->setText(QCoreApplication::translate("GasPanelView", "Uptake (L)", nullptr));
        lTime->setText(QCoreApplication::translate("GasPanelView", "Time (hh:mm:ss)", nullptr));
        tbFlush->setText(QString());
        lFlush->setText(QCoreApplication::translate("GasPanelView", "Flush", nullptr));
        leDEL->setText(QString());
        tbDELup->setText(QCoreApplication::translate("GasPanelView", "...", nullptr));
        tbDELdown->setText(QCoreApplication::translate("GasPanelView", "...", nullptr));
        lbDEL->setText(QCoreApplication::translate("GasPanelView", "<b>agent</b><br>DEL %atm", nullptr));
        tbFGFdown->setText(QCoreApplication::translate("GasPanelView", "...", nullptr));
        stFGF->setText(QString());
        tbFGFup->setText(QCoreApplication::translate("GasPanelView", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GasPanelView: public Ui_GasPanelView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASPANELVIEW_H
