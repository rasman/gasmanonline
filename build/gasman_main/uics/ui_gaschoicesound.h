/********************************************************************************
** Form generated from reading UI file 'gaschoicesound.ui'
**
** Created by: Qt User Interface Compiler version 5.15.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASCHOICESOUND_H
#define UI_GASCHOICESOUND_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GasChoiceSound
{
public:
    QVBoxLayout *vboxLayout;
    QGridLayout *gridLayout;
    QLabel *lSystem_1;
    QLabel *lSystem_3;
    QToolButton *tbBrowse_3;
    QLineEdit *leSystem_2;
    QLabel *lSystem_4;
    QLineEdit *leSystem_3;
    QToolButton *tbBrowse_4;
    QLabel *lSystem_2;
    QLineEdit *leSystem_1;
    QToolButton *tbBrowse_2;
    QToolButton *tbBrowse_1;
    QLineEdit *leSystem_4;
    QSpacerItem *spacerItem;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *GasChoiceSound)
    {
        if (GasChoiceSound->objectName().isEmpty())
            GasChoiceSound->setObjectName(QString::fromUtf8("GasChoiceSound"));
        GasChoiceSound->setWindowModality(Qt::WindowModal);
        GasChoiceSound->resize(250, 180);
        vboxLayout = new QVBoxLayout(GasChoiceSound);
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
        lSystem_1 = new QLabel(GasChoiceSound);
        lSystem_1->setObjectName(QString::fromUtf8("lSystem_1"));

        gridLayout->addWidget(lSystem_1, 0, 0, 1, 1);

        lSystem_3 = new QLabel(GasChoiceSound);
        lSystem_3->setObjectName(QString::fromUtf8("lSystem_3"));

        gridLayout->addWidget(lSystem_3, 2, 0, 1, 1);

        tbBrowse_3 = new QToolButton(GasChoiceSound);
        tbBrowse_3->setObjectName(QString::fromUtf8("tbBrowse_3"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbBrowse_3->setIcon(icon);

        gridLayout->addWidget(tbBrowse_3, 2, 2, 1, 1);

        leSystem_2 = new QLineEdit(GasChoiceSound);
        leSystem_2->setObjectName(QString::fromUtf8("leSystem_2"));

        gridLayout->addWidget(leSystem_2, 1, 1, 1, 1);

        lSystem_4 = new QLabel(GasChoiceSound);
        lSystem_4->setObjectName(QString::fromUtf8("lSystem_4"));

        gridLayout->addWidget(lSystem_4, 3, 0, 1, 1);

        leSystem_3 = new QLineEdit(GasChoiceSound);
        leSystem_3->setObjectName(QString::fromUtf8("leSystem_3"));

        gridLayout->addWidget(leSystem_3, 2, 1, 1, 1);

        tbBrowse_4 = new QToolButton(GasChoiceSound);
        tbBrowse_4->setObjectName(QString::fromUtf8("tbBrowse_4"));
        tbBrowse_4->setIcon(icon);

        gridLayout->addWidget(tbBrowse_4, 3, 2, 1, 1);

        lSystem_2 = new QLabel(GasChoiceSound);
        lSystem_2->setObjectName(QString::fromUtf8("lSystem_2"));

        gridLayout->addWidget(lSystem_2, 1, 0, 1, 1);

        leSystem_1 = new QLineEdit(GasChoiceSound);
        leSystem_1->setObjectName(QString::fromUtf8("leSystem_1"));

        gridLayout->addWidget(leSystem_1, 0, 1, 1, 1);

        tbBrowse_2 = new QToolButton(GasChoiceSound);
        tbBrowse_2->setObjectName(QString::fromUtf8("tbBrowse_2"));
        tbBrowse_2->setIcon(icon);

        gridLayout->addWidget(tbBrowse_2, 1, 2, 1, 1);

        tbBrowse_1 = new QToolButton(GasChoiceSound);
        tbBrowse_1->setObjectName(QString::fromUtf8("tbBrowse_1"));
        tbBrowse_1->setIcon(icon);

        gridLayout->addWidget(tbBrowse_1, 0, 2, 1, 1);

        leSystem_4 = new QLineEdit(GasChoiceSound);
        leSystem_4->setObjectName(QString::fromUtf8("leSystem_4"));

        gridLayout->addWidget(leSystem_4, 3, 1, 1, 1);


        vboxLayout->addLayout(gridLayout);

        spacerItem = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem);

        buttonBox = new QDialogButtonBox(GasChoiceSound);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        vboxLayout->addWidget(buttonBox);

        QWidget::setTabOrder(leSystem_1, tbBrowse_1);
        QWidget::setTabOrder(tbBrowse_1, leSystem_2);
        QWidget::setTabOrder(leSystem_2, tbBrowse_2);
        QWidget::setTabOrder(tbBrowse_2, leSystem_3);
        QWidget::setTabOrder(leSystem_3, tbBrowse_3);
        QWidget::setTabOrder(tbBrowse_3, leSystem_4);
        QWidget::setTabOrder(leSystem_4, tbBrowse_4);
        QWidget::setTabOrder(tbBrowse_4, buttonBox);

        retranslateUi(GasChoiceSound);

        QMetaObject::connectSlotsByName(GasChoiceSound);
    } // setupUi

    void retranslateUi(QDialog *GasChoiceSound)
    {
        GasChoiceSound->setWindowTitle(QCoreApplication::translate("GasChoiceSound", "Choice of a sound", nullptr));
        lSystem_1->setText(QCoreApplication::translate("GasChoiceSound", "System 1:", nullptr));
        lSystem_3->setText(QCoreApplication::translate("GasChoiceSound", "System 3:", nullptr));
        tbBrowse_3->setText(QString());
        lSystem_4->setText(QCoreApplication::translate("GasChoiceSound", "System 4:", nullptr));
        tbBrowse_4->setText(QString());
        lSystem_2->setText(QCoreApplication::translate("GasChoiceSound", "System 2:", nullptr));
        tbBrowse_2->setText(QString());
        tbBrowse_1->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class GasChoiceSound: public Ui_GasChoiceSound {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASCHOICESOUND_H
