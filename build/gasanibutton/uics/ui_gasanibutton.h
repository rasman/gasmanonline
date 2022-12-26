/********************************************************************************
** Form generated from reading UI file 'gasanibutton.ui'
**
** Created by: Qt User Interface Compiler version 5.15.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASANIBUTTON_H
#define UI_GASANIBUTTON_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_gasanibutton
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *gasanibutton)
    {
        if (gasanibutton->objectName().isEmpty())
            gasanibutton->setObjectName(QString::fromUtf8("gasanibutton"));
        gasanibutton->resize(800, 600);
        centralwidget = new QWidget(gasanibutton);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gasanibutton->setCentralWidget(centralwidget);
        menubar = new QMenuBar(gasanibutton);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        gasanibutton->setMenuBar(menubar);
        statusbar = new QStatusBar(gasanibutton);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        gasanibutton->setStatusBar(statusbar);

        retranslateUi(gasanibutton);

        QMetaObject::connectSlotsByName(gasanibutton);
    } // setupUi

    void retranslateUi(QMainWindow *gasanibutton)
    {
        gasanibutton->setWindowTitle(QCoreApplication::translate("gasanibutton", "gasanibutton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class gasanibutton: public Ui_gasanibutton {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASANIBUTTON_H
