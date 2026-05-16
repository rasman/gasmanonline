/********************************************************************************
** Form generated from reading UI file 'gasdeldlg.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASDELDLG_H
#define UI_GASDELDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GasDel
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QGridLayout *gridLayout;
    QLabel *lVA;
    QLabel *lDEL;
    QDoubleSpinBox *dsbVA;
    QDoubleSpinBox *dsbDEL;
    QSpacerItem *spacerItem;
    QGridLayout *gridLayout1;
    QLabel *lCO;
    QLabel *lFGF;
    QDoubleSpinBox *dsbCO;
    QDoubleSpinBox *dsbFGF;
    QSpacerItem *spacerItem1;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *GasDel)
    {
        if (GasDel->objectName().isEmpty())
            GasDel->setObjectName("GasDel");
        GasDel->resize(218, 117);
        GasDel->setModal(true);
        vboxLayout = new QVBoxLayout(GasDel);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout->setObjectName("vboxLayout");
        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout->setContentsMargins(0, 0, 0, 0);
#endif
        hboxLayout->setObjectName("hboxLayout");
        gridLayout = new QGridLayout();
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setContentsMargins(0, 0, 0, 0);
#endif
        gridLayout->setObjectName("gridLayout");
        lVA = new QLabel(GasDel);
        lVA->setObjectName("lVA");

        gridLayout->addWidget(lVA, 1, 0, 1, 1);

        lDEL = new QLabel(GasDel);
        lDEL->setObjectName("lDEL");

        gridLayout->addWidget(lDEL, 0, 0, 1, 1);

        dsbVA = new QDoubleSpinBox(GasDel);
        dsbVA->setObjectName("dsbVA");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dsbVA->sizePolicy().hasHeightForWidth());
        dsbVA->setSizePolicy(sizePolicy);

        gridLayout->addWidget(dsbVA, 1, 1, 1, 1);

        dsbDEL = new QDoubleSpinBox(GasDel);
        dsbDEL->setObjectName("dsbDEL");
        sizePolicy.setHeightForWidth(dsbDEL->sizePolicy().hasHeightForWidth());
        dsbDEL->setSizePolicy(sizePolicy);

        gridLayout->addWidget(dsbDEL, 0, 1, 1, 1);


        hboxLayout->addLayout(gridLayout);

        spacerItem = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout->addItem(spacerItem);

        gridLayout1 = new QGridLayout();
#ifndef Q_OS_MAC
        gridLayout1->setSpacing(6);
#endif
        gridLayout1->setContentsMargins(0, 0, 0, 0);
        gridLayout1->setObjectName("gridLayout1");
        lCO = new QLabel(GasDel);
        lCO->setObjectName("lCO");

        gridLayout1->addWidget(lCO, 1, 0, 1, 1);

        lFGF = new QLabel(GasDel);
        lFGF->setObjectName("lFGF");

        gridLayout1->addWidget(lFGF, 0, 0, 1, 1);

        dsbCO = new QDoubleSpinBox(GasDel);
        dsbCO->setObjectName("dsbCO");
        sizePolicy.setHeightForWidth(dsbCO->sizePolicy().hasHeightForWidth());
        dsbCO->setSizePolicy(sizePolicy);

        gridLayout1->addWidget(dsbCO, 1, 1, 1, 1);

        dsbFGF = new QDoubleSpinBox(GasDel);
        dsbFGF->setObjectName("dsbFGF");
        sizePolicy.setHeightForWidth(dsbFGF->sizePolicy().hasHeightForWidth());
        dsbFGF->setSizePolicy(sizePolicy);

        gridLayout1->addWidget(dsbFGF, 0, 1, 1, 1);


        hboxLayout->addLayout(gridLayout1);


        vboxLayout->addLayout(hboxLayout);

        spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        vboxLayout->addItem(spacerItem1);

        buttonBox = new QDialogButtonBox(GasDel);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

        vboxLayout->addWidget(buttonBox);


        retranslateUi(GasDel);

        QMetaObject::connectSlotsByName(GasDel);
    } // setupUi

    void retranslateUi(QDialog *GasDel)
    {
        GasDel->setWindowTitle(QCoreApplication::translate("GasDel", "Adjust DEL, FGF, VA and CO", nullptr));
        lVA->setText(QCoreApplication::translate("GasDel", "VA:", nullptr));
        lDEL->setText(QCoreApplication::translate("GasDel", "DEL:", nullptr));
        lCO->setText(QCoreApplication::translate("GasDel", "CO:", nullptr));
        lFGF->setText(QCoreApplication::translate("GasDel", "FGF:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GasDel: public Ui_GasDel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASDELDLG_H
