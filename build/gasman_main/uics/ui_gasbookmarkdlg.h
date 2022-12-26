/********************************************************************************
** Form generated from reading UI file 'gasbookmarkdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASBOOKMARKDLG_H
#define UI_GASBOOKMARKDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GasBookmark
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_3;
    QSpacerItem *verticalSpacer_1;
    QListWidget *lwBookmarks;
    QGridLayout *gridLayout;
    QSpinBox *sbHours;
    QLabel *label;
    QSpinBox *sbMin;
    QLabel *label_2;
    QSpinBox *sbSec;
    QLabel *label_3;
    QPushButton *pbAddBookmark;
    QPushButton *pbDeleteBookmark;
    QPushButton *pbDeleteAllBookmarks;
    QListWidget *lwTargets;
    QGridLayout *gridLayout_2;
    QComboBox *cbTargetCompartment;
    QLabel *label_5;
    QPushButton *pbDeleteTarget;
    QPushButton *pbDeleteAllTargets;
    QPushButton *pbAddTarget;
    QLabel *label_4;
    QSpinBox *sbPercent;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonOK;
    QPushButton *pushButtonCancel;

    void setupUi(QDialog *GasBookmark)
    {
        if (GasBookmark->objectName().isEmpty())
            GasBookmark->setObjectName(QString::fromUtf8("GasBookmark"));
        GasBookmark->resize(269, 375);
        GasBookmark->setModal(true);
        verticalLayout = new QVBoxLayout(GasBookmark);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        verticalSpacer_1 = new QSpacerItem(10, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_3->addItem(verticalSpacer_1, 1, 0, 1, 1);

        lwBookmarks = new QListWidget(GasBookmark);
        lwBookmarks->setObjectName(QString::fromUtf8("lwBookmarks"));
        lwBookmarks->setMinimumSize(QSize(130, 0));

        gridLayout_3->addWidget(lwBookmarks, 0, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        sbHours = new QSpinBox(GasBookmark);
        sbHours->setObjectName(QString::fromUtf8("sbHours"));
        sbHours->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        sbHours->setMaximum(99);

        gridLayout->addWidget(sbHours, 0, 0, 1, 1);

        label = new QLabel(GasBookmark);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 1, 1, 1);

        sbMin = new QSpinBox(GasBookmark);
        sbMin->setObjectName(QString::fromUtf8("sbMin"));
        sbMin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        sbMin->setMaximum(59);

        gridLayout->addWidget(sbMin, 1, 0, 1, 1);

        label_2 = new QLabel(GasBookmark);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 1, 1, 1);

        sbSec = new QSpinBox(GasBookmark);
        sbSec->setObjectName(QString::fromUtf8("sbSec"));
        sbSec->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        sbSec->setMaximum(59);

        gridLayout->addWidget(sbSec, 2, 0, 1, 1);

        label_3 = new QLabel(GasBookmark);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 1, 1, 1);

        pbAddBookmark = new QPushButton(GasBookmark);
        pbAddBookmark->setObjectName(QString::fromUtf8("pbAddBookmark"));
        pbAddBookmark->setAutoDefault(false);

        gridLayout->addWidget(pbAddBookmark, 3, 0, 1, 2);

        pbDeleteBookmark = new QPushButton(GasBookmark);
        pbDeleteBookmark->setObjectName(QString::fromUtf8("pbDeleteBookmark"));
        pbDeleteBookmark->setAutoDefault(false);

        gridLayout->addWidget(pbDeleteBookmark, 4, 0, 1, 2);

        pbDeleteAllBookmarks = new QPushButton(GasBookmark);
        pbDeleteAllBookmarks->setObjectName(QString::fromUtf8("pbDeleteAllBookmarks"));
        pbDeleteAllBookmarks->setAutoDefault(false);

        gridLayout->addWidget(pbDeleteAllBookmarks, 5, 0, 1, 2);

        gridLayout->setColumnStretch(0, 1);

        gridLayout_3->addLayout(gridLayout, 0, 1, 1, 1);

        lwTargets = new QListWidget(GasBookmark);
        lwTargets->setObjectName(QString::fromUtf8("lwTargets"));
        lwTargets->setMinimumSize(QSize(130, 0));

        gridLayout_3->addWidget(lwTargets, 2, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        cbTargetCompartment = new QComboBox(GasBookmark);
        cbTargetCompartment->setObjectName(QString::fromUtf8("cbTargetCompartment"));
        cbTargetCompartment->setMaxVisibleItems(5);

        gridLayout_2->addWidget(cbTargetCompartment, 0, 0, 1, 1);

        label_5 = new QLabel(GasBookmark);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_5, 0, 1, 1, 1);

        pbDeleteTarget = new QPushButton(GasBookmark);
        pbDeleteTarget->setObjectName(QString::fromUtf8("pbDeleteTarget"));
        pbDeleteTarget->setAutoDefault(false);

        gridLayout_2->addWidget(pbDeleteTarget, 3, 0, 1, 2);

        pbDeleteAllTargets = new QPushButton(GasBookmark);
        pbDeleteAllTargets->setObjectName(QString::fromUtf8("pbDeleteAllTargets"));
        pbDeleteAllTargets->setAutoDefault(false);

        gridLayout_2->addWidget(pbDeleteAllTargets, 4, 0, 1, 2);

        pbAddTarget = new QPushButton(GasBookmark);
        pbAddTarget->setObjectName(QString::fromUtf8("pbAddTarget"));
        pbAddTarget->setAutoDefault(false);

        gridLayout_2->addWidget(pbAddTarget, 2, 0, 1, 2);

        label_4 = new QLabel(GasBookmark);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 1, 1, 1, 1);

        sbPercent = new QSpinBox(GasBookmark);
        sbPercent->setObjectName(QString::fromUtf8("sbPercent"));
        sbPercent->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        sbPercent->setMinimum(5);
        sbPercent->setMaximum(999);
        sbPercent->setSingleStep(5);
        sbPercent->setValue(100);

        gridLayout_2->addWidget(sbPercent, 1, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 2, 1, 1, 1);

        verticalSpacer_4 = new QSpacerItem(10, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_3->addItem(verticalSpacer_4, 3, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(10, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_3->addItem(verticalSpacer_3, 3, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(10, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_3->addItem(verticalSpacer_2, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(178, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonOK = new QPushButton(GasBookmark);
        pushButtonOK->setObjectName(QString::fromUtf8("pushButtonOK"));
        pushButtonOK->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonOK);

        pushButtonCancel = new QPushButton(GasBookmark);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));
        pushButtonCancel->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonCancel);


        verticalLayout->addLayout(horizontalLayout);

        QWidget::setTabOrder(sbHours, sbMin);
        QWidget::setTabOrder(sbMin, sbSec);
        QWidget::setTabOrder(sbSec, pbAddBookmark);
        QWidget::setTabOrder(pbAddBookmark, pbDeleteBookmark);
        QWidget::setTabOrder(pbDeleteBookmark, pbDeleteAllBookmarks);
        QWidget::setTabOrder(pbDeleteAllBookmarks, cbTargetCompartment);
        QWidget::setTabOrder(cbTargetCompartment, sbPercent);
        QWidget::setTabOrder(sbPercent, pbAddTarget);
        QWidget::setTabOrder(pbAddTarget, pbDeleteTarget);
        QWidget::setTabOrder(pbDeleteTarget, pbDeleteAllTargets);
        QWidget::setTabOrder(pbDeleteAllTargets, pushButtonOK);
        QWidget::setTabOrder(pushButtonOK, pushButtonCancel);
        QWidget::setTabOrder(pushButtonCancel, lwBookmarks);
        QWidget::setTabOrder(lwBookmarks, lwTargets);

        retranslateUi(GasBookmark);

        pbAddBookmark->setDefault(false);
        pbAddTarget->setDefault(false);


        QMetaObject::connectSlotsByName(GasBookmark);
    } // setupUi

    void retranslateUi(QDialog *GasBookmark)
    {
        GasBookmark->setWindowTitle(QCoreApplication::translate("GasBookmark", "Place or Remove a Bookmark", nullptr));
        label->setText(QCoreApplication::translate("GasBookmark", "hours", nullptr));
        label_2->setText(QCoreApplication::translate("GasBookmark", "min", nullptr));
        label_3->setText(QCoreApplication::translate("GasBookmark", "sec", nullptr));
        pbAddBookmark->setText(QCoreApplication::translate("GasBookmark", "Add &Bookmark", nullptr));
        pbDeleteBookmark->setText(QCoreApplication::translate("GasBookmark", "Delete Bookmark", nullptr));
        pbDeleteAllBookmarks->setText(QCoreApplication::translate("GasBookmark", "Delete All Bookmarks", nullptr));
        label_5->setText(QCoreApplication::translate("GasBookmark", "at", nullptr));
        pbDeleteTarget->setText(QCoreApplication::translate("GasBookmark", "Delete Target", nullptr));
        pbDeleteAllTargets->setText(QCoreApplication::translate("GasBookmark", "Delete All Targets", nullptr));
        pbAddTarget->setText(QCoreApplication::translate("GasBookmark", "Add &Target", nullptr));
        label_4->setText(QCoreApplication::translate("GasBookmark", "% MAC", nullptr));
        pushButtonOK->setText(QCoreApplication::translate("GasBookmark", "&OK", nullptr));
        pushButtonCancel->setText(QCoreApplication::translate("GasBookmark", "&Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GasBookmark: public Ui_GasBookmark {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASBOOKMARKDLG_H
