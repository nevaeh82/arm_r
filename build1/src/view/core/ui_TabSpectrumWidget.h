/********************************************************************************
** Form generated from reading UI file 'TabSpectrumWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABSPECTRUMWIDGET_H
#define UI_TABSPECTRUMWIDGET_H

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

class Ui_TabSpectrumWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *spectumWidgetsContainer;
    CorrelationGroupWidget *correlationsGroupWidget;
    QTreeView *settingsTreeView;

    void setupUi(QWidget *TabSpectrumWidget)
    {
        if (TabSpectrumWidget->objectName().isEmpty())
            TabSpectrumWidget->setObjectName(QStringLiteral("TabSpectrumWidget"));
        TabSpectrumWidget->resize(996, 668);
        horizontalLayout = new QHBoxLayout(TabSpectrumWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        spectumWidgetsContainer = new QVBoxLayout();
        spectumWidgetsContainer->setObjectName(QStringLiteral("spectumWidgetsContainer"));

        horizontalLayout->addLayout(spectumWidgetsContainer);

        correlationsGroupWidget = new CorrelationGroupWidget(TabSpectrumWidget);
        correlationsGroupWidget->setObjectName(QStringLiteral("correlationsGroupWidget"));
        correlationsGroupWidget->setMinimumSize(QSize(200, 0));

        horizontalLayout->addWidget(correlationsGroupWidget);

        settingsTreeView = new QTreeView(TabSpectrumWidget);
        settingsTreeView->setObjectName(QStringLiteral("settingsTreeView"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(settingsTreeView->sizePolicy().hasHeightForWidth());
        settingsTreeView->setSizePolicy(sizePolicy);
        settingsTreeView->setMinimumSize(QSize(200, 0));

        horizontalLayout->addWidget(settingsTreeView);

        horizontalLayout->setStretch(0, 4);
        horizontalLayout->setStretch(1, 1);
        horizontalLayout->setStretch(2, 1);

        retranslateUi(TabSpectrumWidget);

        QMetaObject::connectSlotsByName(TabSpectrumWidget);
    } // setupUi

    void retranslateUi(QWidget *TabSpectrumWidget)
    {
        TabSpectrumWidget->setWindowTitle(QApplication::translate("TabSpectrumWidget", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class TabSpectrumWidget: public Ui_TabSpectrumWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABSPECTRUMWIDGET_H
