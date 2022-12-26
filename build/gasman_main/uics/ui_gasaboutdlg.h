/********************************************************************************
** Form generated from reading UI file 'gasaboutdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASABOUTDLG_H
#define UI_GASABOUTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GasAbout
{
public:
    QVBoxLayout *vboxLayout;
    QLabel *lPixmap;
    QLabel *lText;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *GasAbout)
    {
        if (GasAbout->objectName().isEmpty())
            GasAbout->setObjectName(QString::fromUtf8("GasAbout"));
        GasAbout->resize(320, 296);
        GasAbout->setModal(true);
        vboxLayout = new QVBoxLayout(GasAbout);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        lPixmap = new QLabel(GasAbout);
        lPixmap->setObjectName(QString::fromUtf8("lPixmap"));
        lPixmap->setPixmap(QPixmap(QString::fromUtf8(":/images/gasman_lg.png")));
        lPixmap->setAlignment(Qt::AlignCenter);

        vboxLayout->addWidget(lPixmap);

        lText = new QLabel(GasAbout);
        lText->setObjectName(QString::fromUtf8("lText"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lText->sizePolicy().hasHeightForWidth());
        lText->setSizePolicy(sizePolicy);

        vboxLayout->addWidget(lText);

        buttonBox = new QDialogButtonBox(GasAbout);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        vboxLayout->addWidget(buttonBox);


        retranslateUi(GasAbout);

        QMetaObject::connectSlotsByName(GasAbout);
    } // setupUi

    void retranslateUi(QDialog *GasAbout)
    {
        GasAbout->setWindowTitle(QCoreApplication::translate("GasAbout", "About Gas Man\302\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GasAbout: public Ui_GasAbout {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASABOUTDLG_H
