/********************************************************************************
** Form generated from reading UI file 'gasmchn.ui'
**
** Created by: Qt User Interface Compiler version 5.15.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASMCHN_H
#define UI_GASMCHN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_gasmchn
{
public:

    void setupUi(QWidget *gasmchn)
    {
        if (gasmchn->objectName().isEmpty())
            gasmchn->setObjectName(QString::fromUtf8("gasmchn"));
        gasmchn->resize(800, 600);

        retranslateUi(gasmchn);

        QMetaObject::connectSlotsByName(gasmchn);
    } // setupUi

    void retranslateUi(QWidget *gasmchn)
    {
        gasmchn->setWindowTitle(QCoreApplication::translate("gasmchn", "gasmchn", nullptr));
    } // retranslateUi

};

namespace Ui {
    class gasmchn: public Ui_gasmchn {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASMCHN_H
