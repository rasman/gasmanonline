/********************************************************************************
** Form generated from reading UI file 'gasgraph.ui'
**
** Created by: Qt User Interface Compiler version 5.15.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASGRAPH_H
#define UI_GASGRAPH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_gasgraph
{
public:

    void setupUi(QWidget *gasgraph)
    {
        if (gasgraph->objectName().isEmpty())
            gasgraph->setObjectName(QString::fromUtf8("gasgraph"));
        gasgraph->resize(800, 600);

        retranslateUi(gasgraph);

        QMetaObject::connectSlotsByName(gasgraph);
    } // setupUi

    void retranslateUi(QWidget *gasgraph)
    {
        gasgraph->setWindowTitle(QCoreApplication::translate("gasgraph", "gasgraph", nullptr));
    } // retranslateUi

};

namespace Ui {
    class gasgraph: public Ui_gasgraph {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASGRAPH_H
