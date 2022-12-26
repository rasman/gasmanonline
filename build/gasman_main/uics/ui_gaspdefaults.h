/********************************************************************************
** Form generated from reading UI file 'gaspdefaults.ui'
**
** Created by: Qt User Interface Compiler version 5.15.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASPDEFAULTS_H
#define UI_GASPDEFAULTS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GasPDefaults
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *spacerItem;
    QPushButton *pbCustom;
    QGroupBox *groupBox;
    QGridLayout *gridLayout1;
    QRadioButton *rbAudibleSystem1;
    QRadioButton *rbAudibleSystem3;
    QRadioButton *rbAudibleOff;
    QRadioButton *rbAudibleSystem2;
    QRadioButton *rbAudibleBeep;
    QRadioButton *rbAudibleSystem4;
    QGroupBox *groupBox_2;
    QHBoxLayout *hboxLayout;
    QRadioButton *rbGraphLabelsOff;
    QRadioButton *rbGraphLabelsOn;
    QRadioButton *rbGraphLabelsAuto;
    QGroupBox *groupBox_3;
    QHBoxLayout *hboxLayout1;
    QRadioButton *rbGraphThicknessOne;
    QRadioButton *rbGraphThicknessTwo;
    QRadioButton *rbGraphThicknessThree;
    QHBoxLayout *hboxLayout2;
    QGroupBox *groupBox_4;
    QHBoxLayout *hboxLayout3;
    QSlider *hsGraphPercent;
    QLabel *lPercentValue;
    QVBoxLayout *vboxLayout;
    QCheckBox *cbUseGraphPaper;
    QSpacerItem *spacerItem1;

    void setupUi(QWidget *GasPDefaults)
    {
        if (GasPDefaults->objectName().isEmpty())
            GasPDefaults->setObjectName(QString::fromUtf8("GasPDefaults"));
        GasPDefaults->resize(261, 311);
        gridLayout = new QGridLayout(GasPDefaults);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setContentsMargins(9, 9, 9, 9);
#endif
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        spacerItem = new QSpacerItem(191, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem, 1, 0, 1, 1);

        pbCustom = new QPushButton(GasPDefaults);
        pbCustom->setObjectName(QString::fromUtf8("pbCustom"));

        gridLayout->addWidget(pbCustom, 1, 1, 1, 1);

        groupBox = new QGroupBox(GasPDefaults);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout1 = new QGridLayout(groupBox);
#ifndef Q_OS_MAC
        gridLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout1->setContentsMargins(9, 9, 9, 9);
#endif
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        rbAudibleSystem1 = new QRadioButton(groupBox);
        rbAudibleSystem1->setObjectName(QString::fromUtf8("rbAudibleSystem1"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(rbAudibleSystem1->sizePolicy().hasHeightForWidth());
        rbAudibleSystem1->setSizePolicy(sizePolicy);

        gridLayout1->addWidget(rbAudibleSystem1, 0, 1, 1, 1);

        rbAudibleSystem3 = new QRadioButton(groupBox);
        rbAudibleSystem3->setObjectName(QString::fromUtf8("rbAudibleSystem3"));

        gridLayout1->addWidget(rbAudibleSystem3, 0, 2, 1, 1);

        rbAudibleOff = new QRadioButton(groupBox);
        rbAudibleOff->setObjectName(QString::fromUtf8("rbAudibleOff"));
        rbAudibleOff->setChecked(true);

        gridLayout1->addWidget(rbAudibleOff, 0, 0, 1, 1);

        rbAudibleSystem2 = new QRadioButton(groupBox);
        rbAudibleSystem2->setObjectName(QString::fromUtf8("rbAudibleSystem2"));

        gridLayout1->addWidget(rbAudibleSystem2, 1, 1, 1, 1);

        rbAudibleBeep = new QRadioButton(groupBox);
        rbAudibleBeep->setObjectName(QString::fromUtf8("rbAudibleBeep"));

        gridLayout1->addWidget(rbAudibleBeep, 1, 0, 1, 1);

        rbAudibleSystem4 = new QRadioButton(groupBox);
        rbAudibleSystem4->setObjectName(QString::fromUtf8("rbAudibleSystem4"));

        gridLayout1->addWidget(rbAudibleSystem4, 1, 2, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 1, 2);

        groupBox_2 = new QGroupBox(GasPDefaults);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        hboxLayout = new QHBoxLayout(groupBox_2);
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        rbGraphLabelsOff = new QRadioButton(groupBox_2);
        rbGraphLabelsOff->setObjectName(QString::fromUtf8("rbGraphLabelsOff"));
        rbGraphLabelsOff->setChecked(true);

        hboxLayout->addWidget(rbGraphLabelsOff);

        rbGraphLabelsOn = new QRadioButton(groupBox_2);
        rbGraphLabelsOn->setObjectName(QString::fromUtf8("rbGraphLabelsOn"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(rbGraphLabelsOn->sizePolicy().hasHeightForWidth());
        rbGraphLabelsOn->setSizePolicy(sizePolicy1);

        hboxLayout->addWidget(rbGraphLabelsOn);

        rbGraphLabelsAuto = new QRadioButton(groupBox_2);
        rbGraphLabelsAuto->setObjectName(QString::fromUtf8("rbGraphLabelsAuto"));

        hboxLayout->addWidget(rbGraphLabelsAuto);


        gridLayout->addWidget(groupBox_2, 2, 0, 1, 2);

        groupBox_3 = new QGroupBox(GasPDefaults);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        hboxLayout1 = new QHBoxLayout(groupBox_3);
#ifndef Q_OS_MAC
        hboxLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout1->setContentsMargins(9, 9, 9, 9);
#endif
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        rbGraphThicknessOne = new QRadioButton(groupBox_3);
        rbGraphThicknessOne->setObjectName(QString::fromUtf8("rbGraphThicknessOne"));
        rbGraphThicknessOne->setChecked(true);

        hboxLayout1->addWidget(rbGraphThicknessOne);

        rbGraphThicknessTwo = new QRadioButton(groupBox_3);
        rbGraphThicknessTwo->setObjectName(QString::fromUtf8("rbGraphThicknessTwo"));

        hboxLayout1->addWidget(rbGraphThicknessTwo);

        rbGraphThicknessThree = new QRadioButton(groupBox_3);
        rbGraphThicknessThree->setObjectName(QString::fromUtf8("rbGraphThicknessThree"));

        hboxLayout1->addWidget(rbGraphThicknessThree);


        gridLayout->addWidget(groupBox_3, 3, 0, 1, 2);

        hboxLayout2 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout2->setSpacing(6);
#endif
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        groupBox_4 = new QGroupBox(GasPDefaults);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        hboxLayout3 = new QHBoxLayout(groupBox_4);
#ifndef Q_OS_MAC
        hboxLayout3->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout3->setContentsMargins(9, 9, 9, 9);
#endif
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        hsGraphPercent = new QSlider(groupBox_4);
        hsGraphPercent->setObjectName(QString::fromUtf8("hsGraphPercent"));
        hsGraphPercent->setMinimum(25);
        hsGraphPercent->setMaximum(100);
        hsGraphPercent->setOrientation(Qt::Horizontal);
        hsGraphPercent->setTickPosition(QSlider::TicksBelow);

        hboxLayout3->addWidget(hsGraphPercent);

        lPercentValue = new QLabel(groupBox_4);
        lPercentValue->setObjectName(QString::fromUtf8("lPercentValue"));
        lPercentValue->setAlignment(Qt::AlignCenter);

        hboxLayout3->addWidget(lPercentValue);


        hboxLayout2->addWidget(groupBox_4);

        vboxLayout = new QVBoxLayout();
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        cbUseGraphPaper = new QCheckBox(GasPDefaults);
        cbUseGraphPaper->setObjectName(QString::fromUtf8("cbUseGraphPaper"));
        cbUseGraphPaper->setChecked(true);

        vboxLayout->addWidget(cbUseGraphPaper);

        spacerItem1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem1);


        hboxLayout2->addLayout(vboxLayout);


        gridLayout->addLayout(hboxLayout2, 4, 0, 1, 2);


        retranslateUi(GasPDefaults);

        QMetaObject::connectSlotsByName(GasPDefaults);
    } // setupUi

    void retranslateUi(QWidget *GasPDefaults)
    {
        GasPDefaults->setWindowTitle(QCoreApplication::translate("GasPDefaults", "Form", nullptr));
        pbCustom->setText(QCoreApplication::translate("GasPDefaults", "Custom...", nullptr));
        groupBox->setTitle(QCoreApplication::translate("GasPDefaults", "Audible Minute Signal", nullptr));
        rbAudibleSystem1->setText(QCoreApplication::translate("GasPDefaults", "System 1", nullptr));
        rbAudibleSystem3->setText(QCoreApplication::translate("GasPDefaults", "System 3", nullptr));
        rbAudibleOff->setText(QCoreApplication::translate("GasPDefaults", "Off", nullptr));
        rbAudibleSystem2->setText(QCoreApplication::translate("GasPDefaults", "System 2", nullptr));
        rbAudibleBeep->setText(QCoreApplication::translate("GasPDefaults", "Beep", nullptr));
        rbAudibleSystem4->setText(QCoreApplication::translate("GasPDefaults", "System 4", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("GasPDefaults", "Graph Line Labels", nullptr));
        rbGraphLabelsOff->setText(QCoreApplication::translate("GasPDefaults", "Off", nullptr));
        rbGraphLabelsOn->setText(QCoreApplication::translate("GasPDefaults", "On", nullptr));
        rbGraphLabelsAuto->setText(QCoreApplication::translate("GasPDefaults", "Auto", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("GasPDefaults", "Graph Line Thickness", nullptr));
        rbGraphThicknessOne->setText(QCoreApplication::translate("GasPDefaults", "One", nullptr));
        rbGraphThicknessTwo->setText(QCoreApplication::translate("GasPDefaults", "Two", nullptr));
        rbGraphThicknessThree->setText(QCoreApplication::translate("GasPDefaults", "Three", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("GasPDefaults", "Graph Jump Percent", nullptr));
        lPercentValue->setText(QCoreApplication::translate("GasPDefaults", "xx", nullptr));
        cbUseGraphPaper->setText(QCoreApplication::translate("GasPDefaults", "Use Graph Paper", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GasPDefaults: public Ui_GasPDefaults {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASPDEFAULTS_H
