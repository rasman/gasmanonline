/********************************************************************************
** Form generated from reading UI file 'gaspatient.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASPATIENT_H
#define UI_GASPATIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GasPatient
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *spacerItem;
    QGridLayout *gridLayout1;
    QGroupBox *gbFlowPercentages;
    QGridLayout *gridLayout2;
    QSpinBox *sbRatioFAT;
    QSpinBox *sbRatioMUS;
    QSpinBox *sbRatioVRG;
    QLabel *lFlowVRG;
    QLabel *lFlowFAT;
    QLabel *lFlowMUS;
    QGroupBox *gbWeight;
    QGridLayout *gridLayout3;
    QRadioButton *rbKilograms;
    QSpacerItem *spacerItem1;
    QRadioButton *rbPounds;
    QDoubleSpinBox *dsbWeight;
    QGroupBox *gbMetabolism;
    QGridLayout *gridLayout4;
    QDoubleSpinBox *dsbMetabolismCO;
    QDoubleSpinBox *dsbMetabolismVA;
    QLabel *lMetabolismCO;
    QLabel *lMetabolismVA;
    QSpacerItem *spacerItem2;
    QGroupBox *gbVolumes;
    QGridLayout *gridLayout5;
    QLabel *lVEN;
    QDoubleSpinBox *dsbMUS;
    QLabel *lFAT;
    QDoubleSpinBox *dsbFAT;
    QLabel *lVRG;
    QDoubleSpinBox *dsbVRG;
    QDoubleSpinBox *dsbVEN;
    QLabel *lMUS;
    QLabel *lALV;
    QDoubleSpinBox *dsbALV;

    void setupUi(QWidget *GasPatient)
    {
        if (GasPatient->objectName().isEmpty())
            GasPatient->setObjectName("GasPatient");
        GasPatient->resize(272, 344);
        gridLayout = new QGridLayout(GasPatient);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setContentsMargins(9, 9, 9, 9);
#endif
        gridLayout->setObjectName("gridLayout");
        spacerItem = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(spacerItem, 1, 0, 1, 1);

        gridLayout1 = new QGridLayout();
#ifndef Q_OS_MAC
        gridLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout1->setContentsMargins(0, 0, 0, 0);
#endif
        gridLayout1->setObjectName("gridLayout1");
        gbFlowPercentages = new QGroupBox(GasPatient);
        gbFlowPercentages->setObjectName("gbFlowPercentages");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gbFlowPercentages->sizePolicy().hasHeightForWidth());
        gbFlowPercentages->setSizePolicy(sizePolicy);
        gridLayout2 = new QGridLayout(gbFlowPercentages);
#ifndef Q_OS_MAC
        gridLayout2->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout2->setContentsMargins(9, 9, 9, 9);
#endif
        gridLayout2->setObjectName("gridLayout2");
        sbRatioFAT = new QSpinBox(gbFlowPercentages);
        sbRatioFAT->setObjectName("sbRatioFAT");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sbRatioFAT->sizePolicy().hasHeightForWidth());
        sbRatioFAT->setSizePolicy(sizePolicy1);

        gridLayout2->addWidget(sbRatioFAT, 2, 1, 1, 1);

        sbRatioMUS = new QSpinBox(gbFlowPercentages);
        sbRatioMUS->setObjectName("sbRatioMUS");
        sizePolicy1.setHeightForWidth(sbRatioMUS->sizePolicy().hasHeightForWidth());
        sbRatioMUS->setSizePolicy(sizePolicy1);

        gridLayout2->addWidget(sbRatioMUS, 1, 1, 1, 1);

        sbRatioVRG = new QSpinBox(gbFlowPercentages);
        sbRatioVRG->setObjectName("sbRatioVRG");
        sizePolicy1.setHeightForWidth(sbRatioVRG->sizePolicy().hasHeightForWidth());
        sbRatioVRG->setSizePolicy(sizePolicy1);

        gridLayout2->addWidget(sbRatioVRG, 0, 1, 1, 1);

        lFlowVRG = new QLabel(gbFlowPercentages);
        lFlowVRG->setObjectName("lFlowVRG");

        gridLayout2->addWidget(lFlowVRG, 0, 0, 1, 1);

        lFlowFAT = new QLabel(gbFlowPercentages);
        lFlowFAT->setObjectName("lFlowFAT");

        gridLayout2->addWidget(lFlowFAT, 2, 0, 1, 1);

        lFlowMUS = new QLabel(gbFlowPercentages);
        lFlowMUS->setObjectName("lFlowMUS");

        gridLayout2->addWidget(lFlowMUS, 1, 0, 1, 1);


        gridLayout1->addWidget(gbFlowPercentages, 1, 1, 1, 1);

        gbWeight = new QGroupBox(GasPatient);
        gbWeight->setObjectName("gbWeight");
        sizePolicy.setHeightForWidth(gbWeight->sizePolicy().hasHeightForWidth());
        gbWeight->setSizePolicy(sizePolicy);
        gridLayout3 = new QGridLayout(gbWeight);
#ifndef Q_OS_MAC
        gridLayout3->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout3->setContentsMargins(9, 9, 9, 9);
#endif
        gridLayout3->setObjectName("gridLayout3");
        rbKilograms = new QRadioButton(gbWeight);
        rbKilograms->setObjectName("rbKilograms");
        rbKilograms->setMinimumSize(QSize(0, 0));
        rbKilograms->setMaximumSize(QSize(16777215, 16777215));
        rbKilograms->setChecked(true);

        gridLayout3->addWidget(rbKilograms, 1, 1, 1, 1);

        spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout3->addItem(spacerItem1, 1, 0, 2, 1);

        rbPounds = new QRadioButton(gbWeight);
        rbPounds->setObjectName("rbPounds");
        rbPounds->setMinimumSize(QSize(0, 0));
        rbPounds->setMaximumSize(QSize(16777215, 16777215));
        rbPounds->setSizeIncrement(QSize(67, 0));

        gridLayout3->addWidget(rbPounds, 2, 1, 1, 1);

        dsbWeight = new QDoubleSpinBox(gbWeight);
        dsbWeight->setObjectName("dsbWeight");
        sizePolicy1.setHeightForWidth(dsbWeight->sizePolicy().hasHeightForWidth());
        dsbWeight->setSizePolicy(sizePolicy1);
        dsbWeight->setMinimumSize(QSize(0, 0));

        gridLayout3->addWidget(dsbWeight, 0, 0, 1, 2);


        gridLayout1->addWidget(gbWeight, 0, 0, 1, 1);

        gbMetabolism = new QGroupBox(GasPatient);
        gbMetabolism->setObjectName("gbMetabolism");
        sizePolicy.setHeightForWidth(gbMetabolism->sizePolicy().hasHeightForWidth());
        gbMetabolism->setSizePolicy(sizePolicy);
        gridLayout4 = new QGridLayout(gbMetabolism);
#ifndef Q_OS_MAC
        gridLayout4->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout4->setContentsMargins(9, 9, 9, 9);
#endif
        gridLayout4->setObjectName("gridLayout4");
        dsbMetabolismCO = new QDoubleSpinBox(gbMetabolism);
        dsbMetabolismCO->setObjectName("dsbMetabolismCO");
        sizePolicy1.setHeightForWidth(dsbMetabolismCO->sizePolicy().hasHeightForWidth());
        dsbMetabolismCO->setSizePolicy(sizePolicy1);

        gridLayout4->addWidget(dsbMetabolismCO, 1, 1, 1, 1);

        dsbMetabolismVA = new QDoubleSpinBox(gbMetabolism);
        dsbMetabolismVA->setObjectName("dsbMetabolismVA");
        sizePolicy1.setHeightForWidth(dsbMetabolismVA->sizePolicy().hasHeightForWidth());
        dsbMetabolismVA->setSizePolicy(sizePolicy1);

        gridLayout4->addWidget(dsbMetabolismVA, 0, 1, 1, 1);

        lMetabolismCO = new QLabel(gbMetabolism);
        lMetabolismCO->setObjectName("lMetabolismCO");

        gridLayout4->addWidget(lMetabolismCO, 1, 0, 1, 1);

        lMetabolismVA = new QLabel(gbMetabolism);
        lMetabolismVA->setObjectName("lMetabolismVA");

        gridLayout4->addWidget(lMetabolismVA, 0, 0, 1, 1);


        gridLayout1->addWidget(gbMetabolism, 0, 1, 1, 1);

        spacerItem2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout1->addItem(spacerItem2, 2, 1, 1, 1);

        gbVolumes = new QGroupBox(GasPatient);
        gbVolumes->setObjectName("gbVolumes");
        sizePolicy.setHeightForWidth(gbVolumes->sizePolicy().hasHeightForWidth());
        gbVolumes->setSizePolicy(sizePolicy);
        gridLayout5 = new QGridLayout(gbVolumes);
#ifndef Q_OS_MAC
        gridLayout5->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout5->setContentsMargins(9, 9, 9, 9);
#endif
        gridLayout5->setObjectName("gridLayout5");
        lVEN = new QLabel(gbVolumes);
        lVEN->setObjectName("lVEN");

        gridLayout5->addWidget(lVEN, 4, 0, 1, 1);

        dsbMUS = new QDoubleSpinBox(gbVolumes);
        dsbMUS->setObjectName("dsbMUS");
        sizePolicy1.setHeightForWidth(dsbMUS->sizePolicy().hasHeightForWidth());
        dsbMUS->setSizePolicy(sizePolicy1);

        gridLayout5->addWidget(dsbMUS, 2, 1, 1, 1);

        lFAT = new QLabel(gbVolumes);
        lFAT->setObjectName("lFAT");

        gridLayout5->addWidget(lFAT, 3, 0, 1, 1);

        dsbFAT = new QDoubleSpinBox(gbVolumes);
        dsbFAT->setObjectName("dsbFAT");
        sizePolicy1.setHeightForWidth(dsbFAT->sizePolicy().hasHeightForWidth());
        dsbFAT->setSizePolicy(sizePolicy1);

        gridLayout5->addWidget(dsbFAT, 3, 1, 1, 1);

        lVRG = new QLabel(gbVolumes);
        lVRG->setObjectName("lVRG");

        gridLayout5->addWidget(lVRG, 1, 0, 1, 1);

        dsbVRG = new QDoubleSpinBox(gbVolumes);
        dsbVRG->setObjectName("dsbVRG");
        sizePolicy1.setHeightForWidth(dsbVRG->sizePolicy().hasHeightForWidth());
        dsbVRG->setSizePolicy(sizePolicy1);

        gridLayout5->addWidget(dsbVRG, 1, 1, 1, 1);

        dsbVEN = new QDoubleSpinBox(gbVolumes);
        dsbVEN->setObjectName("dsbVEN");
        sizePolicy1.setHeightForWidth(dsbVEN->sizePolicy().hasHeightForWidth());
        dsbVEN->setSizePolicy(sizePolicy1);

        gridLayout5->addWidget(dsbVEN, 4, 1, 1, 1);

        lMUS = new QLabel(gbVolumes);
        lMUS->setObjectName("lMUS");

        gridLayout5->addWidget(lMUS, 2, 0, 1, 1);

        lALV = new QLabel(gbVolumes);
        lALV->setObjectName("lALV");

        gridLayout5->addWidget(lALV, 0, 0, 1, 1);

        dsbALV = new QDoubleSpinBox(gbVolumes);
        dsbALV->setObjectName("dsbALV");
        sizePolicy1.setHeightForWidth(dsbALV->sizePolicy().hasHeightForWidth());
        dsbALV->setSizePolicy(sizePolicy1);

        gridLayout5->addWidget(dsbALV, 0, 1, 1, 1);


        gridLayout1->addWidget(gbVolumes, 1, 0, 2, 1);


        gridLayout->addLayout(gridLayout1, 0, 0, 1, 1);


        retranslateUi(GasPatient);

        QMetaObject::connectSlotsByName(GasPatient);
    } // setupUi

    void retranslateUi(QWidget *GasPatient)
    {
        GasPatient->setWindowTitle(QCoreApplication::translate("GasPatient", "Form", nullptr));
        gbFlowPercentages->setTitle(QCoreApplication::translate("GasPatient", "Flow Percentages", nullptr));
        lFlowVRG->setText(QCoreApplication::translate("GasPatient", "VRG", nullptr));
        lFlowFAT->setText(QCoreApplication::translate("GasPatient", "FAT", nullptr));
        lFlowMUS->setText(QCoreApplication::translate("GasPatient", "MUS", nullptr));
        gbWeight->setTitle(QCoreApplication::translate("GasPatient", "Weight", nullptr));
        rbKilograms->setText(QCoreApplication::translate("GasPatient", "Kilograms", nullptr));
        rbPounds->setText(QCoreApplication::translate("GasPatient", "Pounds", nullptr));
        gbMetabolism->setTitle(QCoreApplication::translate("GasPatient", "Metabolism", nullptr));
        lMetabolismCO->setText(QCoreApplication::translate("GasPatient", "CO", nullptr));
        lMetabolismVA->setText(QCoreApplication::translate("GasPatient", "VA", nullptr));
        gbVolumes->setTitle(QCoreApplication::translate("GasPatient", "Volumes", nullptr));
        lVEN->setText(QCoreApplication::translate("GasPatient", "VEN", nullptr));
        lFAT->setText(QCoreApplication::translate("GasPatient", "FAT", nullptr));
        lVRG->setText(QCoreApplication::translate("GasPatient", "VRG", nullptr));
        lMUS->setText(QCoreApplication::translate("GasPatient", "MUS", nullptr));
        lALV->setText(QCoreApplication::translate("GasPatient", "ALV", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GasPatient: public Ui_GasPatient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASPATIENT_H
