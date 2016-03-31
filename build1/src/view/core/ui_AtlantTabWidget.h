/********************************************************************************
** Form generated from reading UI file 'AtlantTabWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ATLANTTABWIDGET_H
#define UI_ATLANTTABWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AtlantTabWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QComboBox *modeCB;
    QDoubleSpinBox *freqSB;
    QDoubleSpinBox *widthSB;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbOpenRDP;
    QPushButton *sendPB;
    QTextEdit *logsTE;

    void setupUi(QWidget *AtlantTabWidget)
    {
        if (AtlantTabWidget->objectName().isEmpty())
            AtlantTabWidget->setObjectName(QStringLiteral("AtlantTabWidget"));
        AtlantTabWidget->resize(991, 586);
        verticalLayout = new QVBoxLayout(AtlantTabWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        modeCB = new QComboBox(AtlantTabWidget);
        modeCB->setObjectName(QStringLiteral("modeCB"));

        horizontalLayout->addWidget(modeCB);

        freqSB = new QDoubleSpinBox(AtlantTabWidget);
        freqSB->setObjectName(QStringLiteral("freqSB"));
        freqSB->setMinimum(800);
        freqSB->setMaximum(3000);

        horizontalLayout->addWidget(freqSB);

        widthSB = new QDoubleSpinBox(AtlantTabWidget);
        widthSB->setObjectName(QStringLiteral("widthSB"));
        widthSB->setMaximum(20);

        horizontalLayout->addWidget(widthSB);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pbOpenRDP = new QPushButton(AtlantTabWidget);
        pbOpenRDP->setObjectName(QStringLiteral("pbOpenRDP"));

        horizontalLayout->addWidget(pbOpenRDP);

        sendPB = new QPushButton(AtlantTabWidget);
        sendPB->setObjectName(QStringLiteral("sendPB"));

        horizontalLayout->addWidget(sendPB);


        verticalLayout->addLayout(horizontalLayout);

        logsTE = new QTextEdit(AtlantTabWidget);
        logsTE->setObjectName(QStringLiteral("logsTE"));
        logsTE->setReadOnly(true);

        verticalLayout->addWidget(logsTE);


        retranslateUi(AtlantTabWidget);

        QMetaObject::connectSlotsByName(AtlantTabWidget);
    } // setupUi

    void retranslateUi(QWidget *AtlantTabWidget)
    {
        AtlantTabWidget->setWindowTitle(QApplication::translate("AtlantTabWidget", "Form", 0));
        modeCB->clear();
        modeCB->insertItems(0, QStringList()
         << QApplication::translate("AtlantTabWidget", "Adaptation", 0)
         << QApplication::translate("AtlantTabWidget", "Watch", 0)
         << QApplication::translate("AtlantTabWidget", "Check", 0)
         << QApplication::translate("AtlantTabWidget", "Work", 0)
        );
        pbOpenRDP->setText(QApplication::translate("AtlantTabWidget", "OpenRDP", 0));
        sendPB->setText(QApplication::translate("AtlantTabWidget", "Send", 0));
    } // retranslateUi

};

namespace Ui {
    class AtlantTabWidget: public Ui_AtlantTabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ATLANTTABWIDGET_H
