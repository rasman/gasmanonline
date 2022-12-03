/********************************************************************************
** Form generated from reading UI file 'gassdefaults.ui'
**
** Created by: Qt User Interface Compiler version 5.15.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASSDEFAULTS_H
#define UI_GASSDEFAULTS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GasSDefaults
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QGridLayout *gridLayout;
    QDoubleSpinBox *dsbFGF;
    QLabel *lSemiClosed;
    QLabel *lCircuitVolume;
    QDoubleSpinBox *dsbCKT;
    QSpacerItem *spacerItem;
    QGridLayout *gridLayout1;
    QLabel *lSpeed;
    QComboBox *cbSpeed;
    QLabel *lAgent;
    QComboBox *cbAgent;
    QLabel *lCircuit;
    QComboBox *cbCircuit;
    QSpacerItem *spacerItem1;

    void setupUi(QWidget *GasSDefaults)
    {
        if (GasSDefaults->objectName().isEmpty())
            GasSDefaults->setObjectName(QString::fromUtf8("GasSDefaults"));
        GasSDefaults->resize(270, 257);
        vboxLayout = new QVBoxLayout(GasSDefaults);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(9, 9, 9, 9);
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
        gridLayout = new QGridLayout();
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setContentsMargins(0, 0, 0, 0);
#endif
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        dsbFGF = new QDoubleSpinBox(GasSDefaults);
        dsbFGF->setObjectName(QString::fromUtf8("dsbFGF"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dsbFGF->sizePolicy().hasHeightForWidth());
        dsbFGF->setSizePolicy(sizePolicy);

        gridLayout->addWidget(dsbFGF, 0, 1, 1, 1);

        lSemiClosed = new QLabel(GasSDefaults);
        lSemiClosed->setObjectName(QString::fromUtf8("lSemiClosed"));

        gridLayout->addWidget(lSemiClosed, 0, 0, 1, 1);

        lCircuitVolume = new QLabel(GasSDefaults);
        lCircuitVolume->setObjectName(QString::fromUtf8("lCircuitVolume"));

        gridLayout->addWidget(lCircuitVolume, 1, 0, 1, 1);

        dsbCKT = new QDoubleSpinBox(GasSDefaults);
        dsbCKT->setObjectName(QString::fromUtf8("dsbCKT"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(dsbCKT->sizePolicy().hasHeightForWidth());
        dsbCKT->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(dsbCKT, 1, 1, 1, 1);


        hboxLayout->addLayout(gridLayout);

        spacerItem = new QSpacerItem(91, 60, QSizePolicy::Fixed, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);


        vboxLayout->addLayout(hboxLayout);

        gridLayout1 = new QGridLayout();
#ifndef Q_OS_MAC
        gridLayout1->setSpacing(6);
#endif
        gridLayout1->setContentsMargins(0, 0, 0, 0);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        lSpeed = new QLabel(GasSDefaults);
        lSpeed->setObjectName(QString::fromUtf8("lSpeed"));

        gridLayout1->addWidget(lSpeed, 0, 0, 1, 1);

        cbSpeed = new QComboBox(GasSDefaults);
        cbSpeed->setObjectName(QString::fromUtf8("cbSpeed"));

        gridLayout1->addWidget(cbSpeed, 1, 0, 1, 1);

        lAgent = new QLabel(GasSDefaults);
        lAgent->setObjectName(QString::fromUtf8("lAgent"));

        gridLayout1->addWidget(lAgent, 0, 2, 1, 1);

        cbAgent = new QComboBox(GasSDefaults);
        cbAgent->setObjectName(QString::fromUtf8("cbAgent"));

        gridLayout1->addWidget(cbAgent, 1, 2, 1, 1);

        lCircuit = new QLabel(GasSDefaults);
        lCircuit->setObjectName(QString::fromUtf8("lCircuit"));

        gridLayout1->addWidget(lCircuit, 0, 1, 1, 1);

        cbCircuit = new QComboBox(GasSDefaults);
        cbCircuit->setObjectName(QString::fromUtf8("cbCircuit"));

        gridLayout1->addWidget(cbCircuit, 1, 1, 1, 1);


        vboxLayout->addLayout(gridLayout1);

        spacerItem1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem1);


        retranslateUi(GasSDefaults);

        QMetaObject::connectSlotsByName(GasSDefaults);
    } // setupUi

    void retranslateUi(QWidget *GasSDefaults)
    {
        GasSDefaults->setWindowTitle(QCoreApplication::translate("GasSDefaults", "Form", nullptr));
        lSemiClosed->setText(QCoreApplication::translate("GasSDefaults", "FGF (Semi-Closed):", nullptr));
        lCircuitVolume->setText(QCoreApplication::translate("GasSDefaults", "Circuit Volume:", nullptr));
        lSpeed->setText(QCoreApplication::translate("GasSDefaults", "Speed:", nullptr));
        lAgent->setText(QCoreApplication::translate("GasSDefaults", "Agent:", nullptr));
        lCircuit->setText(QCoreApplication::translate("GasSDefaults", "Circuit:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GasSDefaults: public Ui_GasSDefaults {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASSDEFAULTS_H
