/********************************************************************************
** Form generated from reading UI file 'gasbkpoint.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASBKPOINT_H
#define UI_GASBKPOINT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_gasBkPointClass
{
public:
    QVBoxLayout *vboxLayout;
    QLabel *label;
    QTextEdit *textEdit;
    QLabel *lb_caution;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *bt_OK;
    QPushButton *bt_Cancel;

    void setupUi(QDialog *gasBkPointClass)
    {
        if (gasBkPointClass->objectName().isEmpty())
            gasBkPointClass->setObjectName("gasBkPointClass");
        gasBkPointClass->resize(432, 267);
        vboxLayout = new QVBoxLayout(gasBkPointClass);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName("vboxLayout");
        label = new QLabel(gasBkPointClass);
        label->setObjectName("label");

        vboxLayout->addWidget(label);

        textEdit = new QTextEdit(gasBkPointClass);
        textEdit->setObjectName("textEdit");

        vboxLayout->addWidget(textEdit);

        lb_caution = new QLabel(gasBkPointClass);
        lb_caution->setObjectName("lb_caution");

        vboxLayout->addWidget(lb_caution);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName("hboxLayout");
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout->addItem(spacerItem);

        bt_OK = new QPushButton(gasBkPointClass);
        bt_OK->setObjectName("bt_OK");

        hboxLayout->addWidget(bt_OK);

        bt_Cancel = new QPushButton(gasBkPointClass);
        bt_Cancel->setObjectName("bt_Cancel");

        hboxLayout->addWidget(bt_Cancel);


        vboxLayout->addLayout(hboxLayout);


        retranslateUi(gasBkPointClass);

        QMetaObject::connectSlotsByName(gasBkPointClass);
    } // setupUi

    void retranslateUi(QDialog *gasBkPointClass)
    {
        gasBkPointClass->setWindowTitle(QCoreApplication::translate("gasBkPointClass", "gasBkPoint", nullptr));
        label->setText(QCoreApplication::translate("gasBkPointClass", "Legal: CKT,ALV,VRG,MUS,FAT,FGF,DEL,+,-,*,/,=", nullptr));
        lb_caution->setText(QCoreApplication::translate("gasBkPointClass", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; color:#ff0000;\">The condition is illegal, please check it again</span><span style=\" font-size:8pt; color:#ff0000;\">.</span></p></body></html>", nullptr));
        bt_OK->setText(QCoreApplication::translate("gasBkPointClass", "&Set", nullptr));
        bt_Cancel->setText(QCoreApplication::translate("gasBkPointClass", "&Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class gasBkPointClass: public Ui_gasBkPointClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASBKPOINT_H
