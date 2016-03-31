/********************************************************************************
** Form generated from reading UI file 'CommonSpectrumTabWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMMONSPECTRUMTABWIDGET_H
#define UI_COMMONSPECTRUMTABWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Correlations/CorrelationGroupWidget.h"

QT_BEGIN_NAMESPACE

class Ui_CommonSpectrumTabWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *spectumWidgetsContainer;
    CorrelationGroupWidget *correlationsGroupWidget;
    QVBoxLayout *correlationWidgetsContainer;
    QTreeView *settingsTreeView;

    void setupUi(QWidget *CommonSpectrumTabWidget)
    {
        if (CommonSpectrumTabWidget->objectName().isEmpty())
            CommonSpectrumTabWidget->setObjectName(QStringLiteral("CommonSpectrumTabWidget"));
        CommonSpectrumTabWidget->resize(997, 640);
        horizontalLayout = new QHBoxLayout(CommonSpectrumTabWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        spectumWidgetsContainer = new QVBoxLayout();
        spectumWidgetsContainer->setObjectName(QStringLiteral("spectumWidgetsContainer"));

        horizontalLayout->addLayout(spectumWidgetsContainer);

        correlationsGroupWidget = new CorrelationGroupWidget(CommonSpectrumTabWidget);
        correlationsGroupWidget->setObjectName(QStringLiteral("correlationsGroupWidget"));
        correlationWidgetsContainer = new QVBoxLayout(correlationsGroupWidget);
        correlationWidgetsContainer->setObjectName(QStringLiteral("correlationWidgetsContainer"));

        horizontalLayout->addWidget(correlationsGroupWidget);

        settingsTreeView = new QTreeView(CommonSpectrumTabWidget);
        settingsTreeView->setObjectName(QStringLiteral("settingsTreeView"));

        horizontalLayout->addWidget(settingsTreeView);

        horizontalLayout->setStretch(0, 4);
        horizontalLayout->setStretch(1, 1);
        horizontalLayout->setStretch(2, 1);

        retranslateUi(CommonSpectrumTabWidget);

        QMetaObject::connectSlotsByName(CommonSpectrumTabWidget);
    } // setupUi

    void retranslateUi(QWidget *CommonSpectrumTabWidget)
    {
        CommonSpectrumTabWidget->setWindowTitle(QApplication::translate("CommonSpectrumTabWidget", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class CommonSpectrumTabWidget: public Ui_CommonSpectrumTabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMMONSPECTRUMTABWIDGET_H
