/********************************************************************************
** Form generated from reading UI file 'gascostdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASCOSTDLG_H
#define UI_GASCOSTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GasCost
{
public:
    QVBoxLayout *vboxLayout;
    QGridLayout *gridLayout;
    QLabel *lMilliliters;
    QDoubleSpinBox *dsbCost;
    QLabel *lDollars;
    QSpinBox *sbSize;
    QLabel *lSize;
    QLabel *lCost;
    QSpacerItem *spacerItem;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *GasCost)
    {
        if (GasCost->objectName().isEmpty())
            GasCost->setObjectName(QString::fromUtf8("GasCost"));
        GasCost->resize(186, 119);
        GasCost->setModal(true);
        vboxLayout = new QVBoxLayout(GasCost);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        gridLayout = new QGridLayout();
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setContentsMargins(0, 0, 0, 0);
#endif
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lMilliliters = new QLabel(GasCost);
        lMilliliters->setObjectName(QString::fromUtf8("lMilliliters"));

        gridLayout->addWidget(lMilliliters, 0, 2, 1, 1);

        dsbCost = new QDoubleSpinBox(GasCost);
        dsbCost->setObjectName(QString::fromUtf8("dsbCost"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dsbCost->sizePolicy().hasHeightForWidth());
        dsbCost->setSizePolicy(sizePolicy);

        gridLayout->addWidget(dsbCost, 1, 1, 1, 1);

        lDollars = new QLabel(GasCost);
        lDollars->setObjectName(QString::fromUtf8("lDollars"));

        gridLayout->addWidget(lDollars, 1, 2, 1, 1);

        sbSize = new QSpinBox(GasCost);
        sbSize->setObjectName(QString::fromUtf8("sbSize"));
        sizePolicy.setHeightForWidth(sbSize->sizePolicy().hasHeightForWidth());
        sbSize->setSizePolicy(sizePolicy);

        gridLayout->addWidget(sbSize, 0, 1, 1, 1);

        lSize = new QLabel(GasCost);
        lSize->setObjectName(QString::fromUtf8("lSize"));

        gridLayout->addWidget(lSize, 0, 0, 1, 1);

        lCost = new QLabel(GasCost);
        lCost->setObjectName(QString::fromUtf8("lCost"));

        gridLayout->addWidget(lCost, 1, 0, 1, 1);


        vboxLayout->addLayout(gridLayout);

        spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem);

        buttonBox = new QDialogButtonBox(GasCost);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

        vboxLayout->addWidget(buttonBox);


        retranslateUi(GasCost);

        QMetaObject::connectSlotsByName(GasCost);
    } // setupUi

    void retranslateUi(QDialog *GasCost)
    {
        GasCost->setWindowTitle(QCoreApplication::translate("GasCost", "Cost Parameters", nullptr));
        lMilliliters->setText(QCoreApplication::translate("GasCost", "(ml)", nullptr));
        lDollars->setText(QCoreApplication::translate("GasCost", "($)", nullptr));
        lSize->setText(QCoreApplication::translate("GasCost", "Bottle size:", nullptr));
        lCost->setText(QCoreApplication::translate("GasCost", "Bottle cost:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GasCost: public Ui_GasCost {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASCOSTDLG_H
