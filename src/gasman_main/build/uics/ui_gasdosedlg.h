/********************************************************************************
** Form generated from reading UI file 'gasdosedlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASDOSEDLG_H
#define UI_GASDOSEDLG_H

#include <QtCore/QVariant>
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

class Ui_GasDose
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *lUnitDose;
    QDoubleSpinBox *dsbUnitDose;
    QSpacerItem *spacerItem;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *GasDose)
    {
        if (GasDose->objectName().isEmpty())
            GasDose->setObjectName(QString::fromUtf8("GasDose"));
        GasDose->resize(176, 86);
        GasDose->setModal(true);
        gridLayout = new QGridLayout(GasDose);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setContentsMargins(9, 9, 9, 9);
#endif
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        vboxLayout = new QVBoxLayout();
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(0, 0, 0, 0);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout->setContentsMargins(0, 0, 0, 0);
#endif
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        lUnitDose = new QLabel(GasDose);
        lUnitDose->setObjectName(QString::fromUtf8("lUnitDose"));

        hboxLayout->addWidget(lUnitDose);

        dsbUnitDose = new QDoubleSpinBox(GasDose);
        dsbUnitDose->setObjectName(QString::fromUtf8("dsbUnitDose"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dsbUnitDose->sizePolicy().hasHeightForWidth());
        dsbUnitDose->setSizePolicy(sizePolicy);

        hboxLayout->addWidget(dsbUnitDose);


        vboxLayout->addLayout(hboxLayout);

        spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem);

        buttonBox = new QDialogButtonBox(GasDose);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

        vboxLayout->addWidget(buttonBox);


        gridLayout->addLayout(vboxLayout, 0, 0, 1, 1);


        retranslateUi(GasDose);

        QMetaObject::connectSlotsByName(GasDose);
    } // setupUi

    void retranslateUi(QDialog *GasDose)
    {
        GasDose->setWindowTitle(QCoreApplication::translate("GasDose", "Change Unit Dose", nullptr));
        lUnitDose->setText(QCoreApplication::translate("GasDose", "Unit Dose:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GasDose: public Ui_GasDose {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASDOSEDLG_H
