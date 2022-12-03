/********************************************************************************
** Form generated from reading UI file 'gasprintselectdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASPRINTSELECTDLG_H
#define UI_GASPRINTSELECTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GasPrintSelect
{
public:
    QVBoxLayout *vboxLayout;
    QCheckBox *cbPrintout;
    QCheckBox *cbGraphs;
    QSpacerItem *spacerItem;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *GasPrintSelect)
    {
        if (GasPrintSelect->objectName().isEmpty())
            GasPrintSelect->setObjectName(QString::fromUtf8("GasPrintSelect"));
        GasPrintSelect->resize(178, 120);
        GasPrintSelect->setModal(true);
        vboxLayout = new QVBoxLayout(GasPrintSelect);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        cbPrintout = new QCheckBox(GasPrintSelect);
        cbPrintout->setObjectName(QString::fromUtf8("cbPrintout"));

        vboxLayout->addWidget(cbPrintout);

        cbGraphs = new QCheckBox(GasPrintSelect);
        cbGraphs->setObjectName(QString::fromUtf8("cbGraphs"));

        vboxLayout->addWidget(cbGraphs);

        spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem);

        buttonBox = new QDialogButtonBox(GasPrintSelect);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

        vboxLayout->addWidget(buttonBox);


        retranslateUi(GasPrintSelect);

        QMetaObject::connectSlotsByName(GasPrintSelect);
    } // setupUi

    void retranslateUi(QDialog *GasPrintSelect)
    {
        GasPrintSelect->setWindowTitle(QCoreApplication::translate("GasPrintSelect", "Select Printer Output", nullptr));
        cbPrintout->setText(QCoreApplication::translate("GasPrintSelect", "Printout", nullptr));
        cbGraphs->setText(QCoreApplication::translate("GasPrintSelect", "Graphs", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GasPrintSelect: public Ui_GasPrintSelect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASPRINTSELECTDLG_H
