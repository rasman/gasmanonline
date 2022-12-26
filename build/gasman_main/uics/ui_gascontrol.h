/********************************************************************************
** Form generated from reading UI file 'gascontrol.ui'
**
** Created by: Qt User Interface Compiler version 5.15.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GASCONTROL_H
#define UI_GASCONTROL_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_gascontrol
{
public:
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QGroupBox *groupBox;
    QHBoxLayout *hboxLayout1;
    QVBoxLayout *vboxLayout1;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_3;
    QVBoxLayout *vboxLayout2;
    QDoubleSpinBox *weightDoubleSpinBox;
    QComboBox *circuitComboBox;
    QComboBox *speedComboBox;
    QComboBox *viewComboBox;
    QVBoxLayout *vboxLayout3;
    QGroupBox *specialGroupBox;
    QVBoxLayout *vboxLayout4;
    QRadioButton *disUpRadioButton;
    QRadioButton *disRetRadioButton;
    QRadioButton *enVapRadioButton;
    QHBoxLayout *hboxLayout2;
    QSpacerItem *spacerItem;
    QPushButton *runPushButton;
    QPushButton *afapPushButton;
    QGroupBox *agentGroupBox;
    QVBoxLayout *vboxLayout5;
    QHBoxLayout *hboxLayout3;
    QSpacerItem *spacerItem1;
    QPushButton *upPushButton;
    QPushButton *downPushButton;
    QTableWidget *agentTableWidget;

    void setupUi(QWidget *gascontrol)
    {
        if (gascontrol->objectName().isEmpty())
            gascontrol->setObjectName(QString::fromUtf8("gascontrol"));
        gascontrol->resize(645, 400);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gascontrol->sizePolicy().hasHeightForWidth());
        gascontrol->setSizePolicy(sizePolicy);
        gascontrol->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/gasman_sm.png"), QSize(), QIcon::Normal, QIcon::Off);
        gascontrol->setWindowIcon(icon);
        hboxLayout = new QHBoxLayout(gascontrol);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        vboxLayout = new QVBoxLayout();
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        groupBox = new QGroupBox(gascontrol);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(10);
        groupBox->setFont(font);
        hboxLayout1 = new QHBoxLayout(groupBox);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        vboxLayout1->addWidget(label);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        vboxLayout1->addWidget(label_2);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        vboxLayout1->addWidget(label_4);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        vboxLayout1->addWidget(label_3);


        hboxLayout1->addLayout(vboxLayout1);

        vboxLayout2 = new QVBoxLayout();
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        weightDoubleSpinBox = new QDoubleSpinBox(groupBox);
        weightDoubleSpinBox->setObjectName(QString::fromUtf8("weightDoubleSpinBox"));

        vboxLayout2->addWidget(weightDoubleSpinBox);

        circuitComboBox = new QComboBox(groupBox);
        circuitComboBox->setObjectName(QString::fromUtf8("circuitComboBox"));

        vboxLayout2->addWidget(circuitComboBox);

        speedComboBox = new QComboBox(groupBox);
        speedComboBox->setObjectName(QString::fromUtf8("speedComboBox"));

        vboxLayout2->addWidget(speedComboBox);

        viewComboBox = new QComboBox(groupBox);
        viewComboBox->setObjectName(QString::fromUtf8("viewComboBox"));

        vboxLayout2->addWidget(viewComboBox);


        hboxLayout1->addLayout(vboxLayout2);


        vboxLayout->addWidget(groupBox);

        vboxLayout3 = new QVBoxLayout();
        vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
        specialGroupBox = new QGroupBox(gascontrol);
        specialGroupBox->setObjectName(QString::fromUtf8("specialGroupBox"));
        vboxLayout4 = new QVBoxLayout(specialGroupBox);
        vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
        disUpRadioButton = new QRadioButton(specialGroupBox);
        disUpRadioButton->setObjectName(QString::fromUtf8("disUpRadioButton"));
        disUpRadioButton->setAutoExclusive(false);

        vboxLayout4->addWidget(disUpRadioButton);

        disRetRadioButton = new QRadioButton(specialGroupBox);
        disRetRadioButton->setObjectName(QString::fromUtf8("disRetRadioButton"));
        disRetRadioButton->setAutoExclusive(false);

        vboxLayout4->addWidget(disRetRadioButton);

        enVapRadioButton = new QRadioButton(specialGroupBox);
        enVapRadioButton->setObjectName(QString::fromUtf8("enVapRadioButton"));

        vboxLayout4->addWidget(enVapRadioButton);


        vboxLayout3->addWidget(specialGroupBox);


        vboxLayout->addLayout(vboxLayout3);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem);

        runPushButton = new QPushButton(gascontrol);
        runPushButton->setObjectName(QString::fromUtf8("runPushButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(runPushButton->sizePolicy().hasHeightForWidth());
        runPushButton->setSizePolicy(sizePolicy1);
        runPushButton->setMinimumSize(QSize(0, 0));
        runPushButton->setMaximumSize(QSize(16777215, 16777215));

        hboxLayout2->addWidget(runPushButton);

        afapPushButton = new QPushButton(gascontrol);
        afapPushButton->setObjectName(QString::fromUtf8("afapPushButton"));

        hboxLayout2->addWidget(afapPushButton);


        vboxLayout->addLayout(hboxLayout2);


        hboxLayout->addLayout(vboxLayout);

        agentGroupBox = new QGroupBox(gascontrol);
        agentGroupBox->setObjectName(QString::fromUtf8("agentGroupBox"));
        sizePolicy.setHeightForWidth(agentGroupBox->sizePolicy().hasHeightForWidth());
        agentGroupBox->setSizePolicy(sizePolicy);
        agentGroupBox->setFont(font);
        vboxLayout5 = new QVBoxLayout(agentGroupBox);
        vboxLayout5->setObjectName(QString::fromUtf8("vboxLayout5"));
        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout3->addItem(spacerItem1);

        upPushButton = new QPushButton(agentGroupBox);
        upPushButton->setObjectName(QString::fromUtf8("upPushButton"));
        upPushButton->setMaximumSize(QSize(30, 16777215));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/up.png"), QSize(), QIcon::Normal, QIcon::Off);
        upPushButton->setIcon(icon1);

        hboxLayout3->addWidget(upPushButton);

        downPushButton = new QPushButton(agentGroupBox);
        downPushButton->setObjectName(QString::fromUtf8("downPushButton"));
        downPushButton->setMinimumSize(QSize(15, 15));
        downPushButton->setMaximumSize(QSize(30, 16777215));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/down.png"), QSize(), QIcon::Normal, QIcon::Off);
        downPushButton->setIcon(icon2);

        hboxLayout3->addWidget(downPushButton);


        vboxLayout5->addLayout(hboxLayout3);

        agentTableWidget = new QTableWidget(agentGroupBox);
        if (agentTableWidget->columnCount() < 2)
            agentTableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        agentTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        agentTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        if (agentTableWidget->rowCount() < 1)
            agentTableWidget->setRowCount(1);
        agentTableWidget->setObjectName(QString::fromUtf8("agentTableWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(agentTableWidget->sizePolicy().hasHeightForWidth());
        agentTableWidget->setSizePolicy(sizePolicy2);
        agentTableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
        agentTableWidget->setFrameShape(QFrame::StyledPanel);
        agentTableWidget->setFrameShadow(QFrame::Sunken);
        agentTableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        agentTableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
        agentTableWidget->setAlternatingRowColors(true);
        agentTableWidget->setShowGrid(true);
        agentTableWidget->setGridStyle(Qt::DotLine);
        agentTableWidget->setCornerButtonEnabled(false);
        agentTableWidget->setRowCount(1);

        vboxLayout5->addWidget(agentTableWidget);


        hboxLayout->addWidget(agentGroupBox);


        retranslateUi(gascontrol);

        QMetaObject::connectSlotsByName(gascontrol);
    } // setupUi

    void retranslateUi(QWidget *gascontrol)
    {
        gascontrol->setWindowTitle(QCoreApplication::translate("gascontrol", "Form", nullptr));
        groupBox->setTitle(QCoreApplication::translate("gascontrol", "Simulation Controls", nullptr));
        label->setText(QCoreApplication::translate("gascontrol", "Weight", nullptr));
        label_2->setText(QCoreApplication::translate("gascontrol", "Circuit", nullptr));
        label_4->setText(QCoreApplication::translate("gascontrol", "Speed", nullptr));
        label_3->setText(QCoreApplication::translate("gascontrol", "View", nullptr));
        specialGroupBox->setTitle(QCoreApplication::translate("gascontrol", "Special", nullptr));
        disUpRadioButton->setText(QCoreApplication::translate("gascontrol", "Disable Uptake", nullptr));
        disRetRadioButton->setText(QCoreApplication::translate("gascontrol", "Disable Return", nullptr));
        enVapRadioButton->setText(QCoreApplication::translate("gascontrol", "Enable Vapor", nullptr));
        runPushButton->setText(QCoreApplication::translate("gascontrol", "Begin", nullptr));
        afapPushButton->setText(QCoreApplication::translate("gascontrol", "AFAP", nullptr));
        agentGroupBox->setTitle(QCoreApplication::translate("gascontrol", "Agents", nullptr));
        upPushButton->setText(QString());
        downPushButton->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = agentTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("gascontrol", "Agent", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = agentTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("gascontrol", "Color", nullptr));
    } // retranslateUi

};

namespace Ui {
    class gascontrol: public Ui_gascontrol {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GASCONTROL_H
