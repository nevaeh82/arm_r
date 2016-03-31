/********************************************************************************
** Form generated from reading UI file 'SpectrumWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPECTRUMWIDGET_H
#define UI_SPECTRUMWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Prm300ControlWidget.h"
#include "modules/gui/multi_graphics_spectrum_interface/mg_spectrum_interface.h"

QT_BEGIN_NAMESPACE

class Ui_SpectrumWidget
{
public:
    QGridLayout *gridLayout;
    Q_MG_SpectrumInterface *graphicsWidget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QCheckBox *autosearchCB;
    QCheckBox *panoramaCB;
    QCheckBox *thresholdCB;
    QCheckBox *getSpectrumCB;
    QCheckBox *maximumsCB;
    QCheckBox *prmControlCB;
    QLabel *spectrumNameLB;
    Prm300ControlWidget *PrmControlWidget;

    void setupUi(QWidget *SpectrumWidget)
    {
        if (SpectrumWidget->objectName().isEmpty())
            SpectrumWidget->setObjectName(QStringLiteral("SpectrumWidget"));
        SpectrumWidget->resize(971, 653);
        gridLayout = new QGridLayout(SpectrumWidget);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        graphicsWidget = new Q_MG_SpectrumInterface(SpectrumWidget);
        graphicsWidget->setObjectName(QStringLiteral("graphicsWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(graphicsWidget->sizePolicy().hasHeightForWidth());
        graphicsWidget->setSizePolicy(sizePolicy);

        gridLayout->addWidget(graphicsWidget, 1, 1, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(2);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        autosearchCB = new QCheckBox(SpectrumWidget);
        autosearchCB->setObjectName(QStringLiteral("autosearchCB"));

        verticalLayout->addWidget(autosearchCB);

        panoramaCB = new QCheckBox(SpectrumWidget);
        panoramaCB->setObjectName(QStringLiteral("panoramaCB"));

        verticalLayout->addWidget(panoramaCB);

        thresholdCB = new QCheckBox(SpectrumWidget);
        thresholdCB->setObjectName(QStringLiteral("thresholdCB"));

        verticalLayout->addWidget(thresholdCB);

        getSpectrumCB = new QCheckBox(SpectrumWidget);
        getSpectrumCB->setObjectName(QStringLiteral("getSpectrumCB"));
        getSpectrumCB->setChecked(true);

        verticalLayout->addWidget(getSpectrumCB);

        maximumsCB = new QCheckBox(SpectrumWidget);
        maximumsCB->setObjectName(QStringLiteral("maximumsCB"));

        verticalLayout->addWidget(maximumsCB);

        prmControlCB = new QCheckBox(SpectrumWidget);
        prmControlCB->setObjectName(QStringLiteral("prmControlCB"));

        verticalLayout->addWidget(prmControlCB);


        gridLayout->addLayout(verticalLayout, 1, 2, 1, 1);

        spectrumNameLB = new QLabel(SpectrumWidget);
        spectrumNameLB->setObjectName(QStringLiteral("spectrumNameLB"));
        spectrumNameLB->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(spectrumNameLB, 0, 1, 1, 1);

        PrmControlWidget = new Prm300ControlWidget(SpectrumWidget);
        PrmControlWidget->setObjectName(QStringLiteral("PrmControlWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(PrmControlWidget->sizePolicy().hasHeightForWidth());
        PrmControlWidget->setSizePolicy(sizePolicy1);
        PrmControlWidget->setMinimumSize(QSize(100, 0));
        PrmControlWidget->setBaseSize(QSize(0, 0));

        gridLayout->addWidget(PrmControlWidget, 1, 0, 1, 1);

        gridLayout->setColumnStretch(1, 10);

        retranslateUi(SpectrumWidget);

        QMetaObject::connectSlotsByName(SpectrumWidget);
    } // setupUi

    void retranslateUi(QWidget *SpectrumWidget)
    {
        SpectrumWidget->setWindowTitle(QApplication::translate("SpectrumWidget", "Form", 0));
        autosearchCB->setText(QApplication::translate("SpectrumWidget", "Autosearch", 0));
        panoramaCB->setText(QApplication::translate("SpectrumWidget", "Panorama", 0));
        thresholdCB->setText(QApplication::translate("SpectrumWidget", "Threshold", 0));
        getSpectrumCB->setText(QApplication::translate("SpectrumWidget", "Get spectrum", 0));
        maximumsCB->setText(QApplication::translate("SpectrumWidget", " Maximums", 0));
        prmControlCB->setText(QApplication::translate("SpectrumWidget", "PRM control", 0));
        spectrumNameLB->setText(QApplication::translate("SpectrumWidget", "Unknown", 0));
    } // retranslateUi

};

namespace Ui {
    class SpectrumWidget: public Ui_SpectrumWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPECTRUMWIDGET_H
