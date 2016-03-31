/********************************************************************************
** Form generated from reading UI file 'CorrelationGroupWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CORRELATIONGROUPWIDGET_H
#define UI_CORRELATIONGROUPWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CorrelationGroupWidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *correlationWidgetsContainer;

    void setupUi(QWidget *CorrelationGroupWidget)
    {
        if (CorrelationGroupWidget->objectName().isEmpty())
            CorrelationGroupWidget->setObjectName(QStringLiteral("CorrelationGroupWidget"));
        CorrelationGroupWidget->resize(271, 686);
        verticalLayout_2 = new QVBoxLayout(CorrelationGroupWidget);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetNoConstraint);
        correlationWidgetsContainer = new QVBoxLayout();
        correlationWidgetsContainer->setObjectName(QStringLiteral("correlationWidgetsContainer"));

        verticalLayout_2->addLayout(correlationWidgetsContainer);


        retranslateUi(CorrelationGroupWidget);

        QMetaObject::connectSlotsByName(CorrelationGroupWidget);
    } // setupUi

    void retranslateUi(QWidget *CorrelationGroupWidget)
    {
        CorrelationGroupWidget->setWindowTitle(QApplication::translate("CorrelationGroupWidget", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class CorrelationGroupWidget: public Ui_CorrelationGroupWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CORRELATIONGROUPWIDGET_H
