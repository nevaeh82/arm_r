/********************************************************************************
** Form generated from reading UI file 'CorrelationWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CORRELATIONWIDGET_H
#define UI_CORRELATIONWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "modules/gui/multi_graphics_spectrum_interface/mg_spectrum_interface.h"

QT_BEGIN_NAMESPACE

class Ui_CorrelationWidget
{
public:
    QVBoxLayout *verticalLayout;
    Q_MG_SpectrumInterface *spectrumWidget;

    void setupUi(QWidget *CorrelationWidget)
    {
        if (CorrelationWidget->objectName().isEmpty())
            CorrelationWidget->setObjectName(QStringLiteral("CorrelationWidget"));
        CorrelationWidget->resize(365, 185);
        verticalLayout = new QVBoxLayout(CorrelationWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        spectrumWidget = new Q_MG_SpectrumInterface(CorrelationWidget);
        spectrumWidget->setObjectName(QStringLiteral("spectrumWidget"));

        verticalLayout->addWidget(spectrumWidget);


        retranslateUi(CorrelationWidget);

        QMetaObject::connectSlotsByName(CorrelationWidget);
    } // setupUi

    void retranslateUi(QWidget *CorrelationWidget)
    {
        CorrelationWidget->setWindowTitle(QApplication::translate("CorrelationWidget", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class CorrelationWidget: public Ui_CorrelationWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CORRELATIONWIDGET_H
