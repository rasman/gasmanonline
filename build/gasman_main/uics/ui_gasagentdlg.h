/********************************************************************************
** Form generated from reading UI file 'gasagentdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASAGENTDLG_H
#define UI_GASAGENTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GasAgent
{
public:
    QVBoxLayout *vboxLayout;
    QGridLayout *gridLayout;
    QComboBox *cbCircuit;
    QLabel *lAgent;
    QLabel *lCircuit;
    QComboBox *cbAgent;
    QSpacerItem *spacerItem;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *GasAgent)
    {
        if (GasAgent->objectName().isEmpty())
            GasAgent->setObjectName(QString::fromUtf8("GasAgent"));
        GasAgent->resize(215, 109);
        GasAgent->setModal(true);
        vboxLayout = new QVBoxLayout(GasAgent);
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
        cbCircuit = new QComboBox(GasAgent);
        cbCircuit->setObjectName(QString::fromUtf8("cbCircuit"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cbCircuit->sizePolicy().hasHeightForWidth());
        cbCircuit->setSizePolicy(sizePolicy);

        gridLayout->addWidget(cbCircuit, 1, 1, 1, 1);

        lAgent = new QLabel(GasAgent);
        lAgent->setObjectName(QString::fromUtf8("lAgent"));

        gridLayout->addWidget(lAgent, 0, 0, 1, 1);

        lCircuit = new QLabel(GasAgent);
        lCircuit->setObjectName(QString::fromUtf8("lCircuit"));

        gridLayout->addWidget(lCircuit, 1, 0, 1, 1);

        cbAgent = new QComboBox(GasAgent);
        cbAgent->setObjectName(QString::fromUtf8("cbAgent"));
        sizePolicy.setHeightForWidth(cbAgent->sizePolicy().hasHeightForWidth());
        cbAgent->setSizePolicy(sizePolicy);

        gridLayout->addWidget(cbAgent, 0, 1, 1, 1);


        vboxLayout->addLayout(gridLayout);

        spacerItem = new QSpacerItem(197, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem);

        buttonBox = new QDialogButtonBox(GasAgent);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(buttonBox->sizePolicy().hasHeightForWidth());
        buttonBox->setSizePolicy(sizePolicy1);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

        vboxLayout->addWidget(buttonBox);


        retranslateUi(GasAgent);

        QMetaObject::connectSlotsByName(GasAgent);
    } // setupUi

    void retranslateUi(QDialog *GasAgent)
    {
        GasAgent->setWindowTitle(QCoreApplication::translate("GasAgent", "Change Anesthetic Agent and Circuit Type", nullptr));
        lAgent->setText(QCoreApplication::translate("GasAgent", "Agent:", nullptr));
        lCircuit->setText(QCoreApplication::translate("GasAgent", "Circuit:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GasAgent: public Ui_GasAgent {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASAGENTDLG_H
