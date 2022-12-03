/********************************************************************************
** Form generated from reading UI file 'gasregisterdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASREGISTERDLG_H
#define UI_GASREGISTERDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GasRegister
{
public:
    QVBoxLayout *vboxLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *nameLineEdit;
    QLabel *label_2;
    QLineEdit *companyLineEdit;
    QLabel *label_3;
    QLineEdit *keyLineEdit;
    QLineEdit *infoLineEdit;
    QLabel *label_4;
    QGroupBox *groupBox;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *pbTemp;
    QPushButton *openFile;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *GasRegister)
    {
        if (GasRegister->objectName().isEmpty())
            GasRegister->setObjectName(QString::fromUtf8("GasRegister"));
        GasRegister->resize(438, 188);
        vboxLayout = new QVBoxLayout(GasRegister);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(GasRegister);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        nameLineEdit = new QLineEdit(GasRegister);
        nameLineEdit->setObjectName(QString::fromUtf8("nameLineEdit"));

        gridLayout->addWidget(nameLineEdit, 0, 1, 1, 1);

        label_2 = new QLabel(GasRegister);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        companyLineEdit = new QLineEdit(GasRegister);
        companyLineEdit->setObjectName(QString::fromUtf8("companyLineEdit"));

        gridLayout->addWidget(companyLineEdit, 1, 1, 1, 1);

        label_3 = new QLabel(GasRegister);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        keyLineEdit = new QLineEdit(GasRegister);
        keyLineEdit->setObjectName(QString::fromUtf8("keyLineEdit"));

        gridLayout->addWidget(keyLineEdit, 3, 1, 1, 1);

        infoLineEdit = new QLineEdit(GasRegister);
        infoLineEdit->setObjectName(QString::fromUtf8("infoLineEdit"));

        gridLayout->addWidget(infoLineEdit, 2, 1, 1, 1);

        label_4 = new QLabel(GasRegister);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 2, 0, 1, 1);


        vboxLayout->addLayout(gridLayout);

        groupBox = new QGroupBox(GasRegister);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        hboxLayout = new QHBoxLayout(groupBox);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        pbTemp = new QPushButton(groupBox);
        pbTemp->setObjectName(QString::fromUtf8("pbTemp"));

        hboxLayout->addWidget(pbTemp);

        openFile = new QPushButton(groupBox);
        openFile->setObjectName(QString::fromUtf8("openFile"));

        hboxLayout->addWidget(openFile);


        vboxLayout->addWidget(groupBox);

        buttonBox = new QDialogButtonBox(GasRegister);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

        vboxLayout->addWidget(buttonBox);

        QWidget::setTabOrder(nameLineEdit, companyLineEdit);
        QWidget::setTabOrder(companyLineEdit, infoLineEdit);
        QWidget::setTabOrder(infoLineEdit, keyLineEdit);
        QWidget::setTabOrder(keyLineEdit, openFile);
        QWidget::setTabOrder(openFile, buttonBox);
        QWidget::setTabOrder(buttonBox, pbTemp);

        retranslateUi(GasRegister);
        QObject::connect(buttonBox, SIGNAL(rejected()), GasRegister, SLOT(reject()));

        QMetaObject::connectSlotsByName(GasRegister);
    } // setupUi

    void retranslateUi(QDialog *GasRegister)
    {
        GasRegister->setWindowTitle(QCoreApplication::translate("GasRegister", "Register GasMan", nullptr));
        label->setText(QCoreApplication::translate("GasRegister", "Name:", nullptr));
        label_2->setText(QCoreApplication::translate("GasRegister", "Company:", nullptr));
        label_3->setText(QCoreApplication::translate("GasRegister", "License Key:", nullptr));
        label_4->setText(QCoreApplication::translate("GasRegister", "Information", nullptr));
        groupBox->setTitle(QCoreApplication::translate("GasRegister", "Register using key file", nullptr));
        pbTemp->setText(QCoreApplication::translate("GasRegister", "PushButton", nullptr));
        openFile->setText(QCoreApplication::translate("GasRegister", "&Open File...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GasRegister: public Ui_GasRegister {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASREGISTERDLG_H
