/********************************************************************************
** Form generated from reading UI file 'ControlPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLPANEL_H
#define UI_CONTROLPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlPanelWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QSpinBox *startFreqSB;
    QLabel *toLabel;
    QSpinBox *endFreqSB;
    QPushButton *panoramaPB;
    QVBoxLayout *verticalLayout;
    QCheckBox *autosearchCB;
    QCheckBox *panoramaCB;
    QComboBox *cbMode;
    QSpacerItem *horizontalSpacer;
    QGridLayout *gridLayout;
    QLabel *correlationStatusLabelActive;
    QLabel *correlationStatusLabelValue;
    QLabel *correlationStatusLabel;
    QLabel *solverQualityNameLB;
    QLabel *solverQualityLB;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pbDown100MHz;
    QPushButton *pbDown10MHz;
    QPushButton *pbDown1MHz;
    QSpinBox *commonFreqSB;
    QPushButton *pbUp1MHz;
    QPushButton *pbUp10MHz;
    QPushButton *pbUp100MHz;
    QPushButton *commonFrequencyPB;

    void setupUi(QWidget *ControlPanelWidget)
    {
        if (ControlPanelWidget->objectName().isEmpty())
            ControlPanelWidget->setObjectName(QStringLiteral("ControlPanelWidget"));
        ControlPanelWidget->resize(1136, 60);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(160, 160, 164, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        ControlPanelWidget->setPalette(palette);
        horizontalLayout = new QHBoxLayout(ControlPanelWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        startFreqSB = new QSpinBox(ControlPanelWidget);
        startFreqSB->setObjectName(QStringLiteral("startFreqSB"));
        startFreqSB->setMinimumSize(QSize(70, 0));
        startFreqSB->setMinimum(20);
        startFreqSB->setMaximum(7000);
        startFreqSB->setValue(300);

        horizontalLayout->addWidget(startFreqSB);

        toLabel = new QLabel(ControlPanelWidget);
        toLabel->setObjectName(QStringLiteral("toLabel"));

        horizontalLayout->addWidget(toLabel);

        endFreqSB = new QSpinBox(ControlPanelWidget);
        endFreqSB->setObjectName(QStringLiteral("endFreqSB"));
        endFreqSB->setMinimumSize(QSize(70, 0));
        endFreqSB->setMinimum(20);
        endFreqSB->setMaximum(7000);
        endFreqSB->setValue(340);

        horizontalLayout->addWidget(endFreqSB);

        panoramaPB = new QPushButton(ControlPanelWidget);
        panoramaPB->setObjectName(QStringLiteral("panoramaPB"));

        horizontalLayout->addWidget(panoramaPB);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        autosearchCB = new QCheckBox(ControlPanelWidget);
        autosearchCB->setObjectName(QStringLiteral("autosearchCB"));

        verticalLayout->addWidget(autosearchCB);

        panoramaCB = new QCheckBox(ControlPanelWidget);
        panoramaCB->setObjectName(QStringLiteral("panoramaCB"));

        verticalLayout->addWidget(panoramaCB);


        horizontalLayout->addLayout(verticalLayout);

        cbMode = new QComboBox(ControlPanelWidget);
        cbMode->setObjectName(QStringLiteral("cbMode"));

        horizontalLayout->addWidget(cbMode);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        correlationStatusLabelActive = new QLabel(ControlPanelWidget);
        correlationStatusLabelActive->setObjectName(QStringLiteral("correlationStatusLabelActive"));

        gridLayout->addWidget(correlationStatusLabelActive, 0, 2, 1, 1);

        correlationStatusLabelValue = new QLabel(ControlPanelWidget);
        correlationStatusLabelValue->setObjectName(QStringLiteral("correlationStatusLabelValue"));

        gridLayout->addWidget(correlationStatusLabelValue, 0, 1, 1, 1);

        correlationStatusLabel = new QLabel(ControlPanelWidget);
        correlationStatusLabel->setObjectName(QStringLiteral("correlationStatusLabel"));

        gridLayout->addWidget(correlationStatusLabel, 0, 0, 1, 1);

        solverQualityNameLB = new QLabel(ControlPanelWidget);
        solverQualityNameLB->setObjectName(QStringLiteral("solverQualityNameLB"));

        gridLayout->addWidget(solverQualityNameLB, 1, 0, 1, 1);

        solverQualityLB = new QLabel(ControlPanelWidget);
        solverQualityLB->setObjectName(QStringLiteral("solverQualityLB"));

        gridLayout->addWidget(solverQualityLB, 1, 2, 1, 1);


        horizontalLayout->addLayout(gridLayout);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pbDown100MHz = new QPushButton(ControlPanelWidget);
        pbDown100MHz->setObjectName(QStringLiteral("pbDown100MHz"));
        pbDown100MHz->setMaximumSize(QSize(30, 16777215));

        horizontalLayout->addWidget(pbDown100MHz);

        pbDown10MHz = new QPushButton(ControlPanelWidget);
        pbDown10MHz->setObjectName(QStringLiteral("pbDown10MHz"));
        pbDown10MHz->setMaximumSize(QSize(30, 16777215));

        horizontalLayout->addWidget(pbDown10MHz);

        pbDown1MHz = new QPushButton(ControlPanelWidget);
        pbDown1MHz->setObjectName(QStringLiteral("pbDown1MHz"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pbDown1MHz->sizePolicy().hasHeightForWidth());
        pbDown1MHz->setSizePolicy(sizePolicy);
        pbDown1MHz->setMaximumSize(QSize(30, 16777215));

        horizontalLayout->addWidget(pbDown1MHz);

        commonFreqSB = new QSpinBox(ControlPanelWidget);
        commonFreqSB->setObjectName(QStringLiteral("commonFreqSB"));
        commonFreqSB->setMinimumSize(QSize(70, 0));
        commonFreqSB->setMinimum(20);
        commonFreqSB->setMaximum(7000);
        commonFreqSB->setValue(20);

        horizontalLayout->addWidget(commonFreqSB);

        pbUp1MHz = new QPushButton(ControlPanelWidget);
        pbUp1MHz->setObjectName(QStringLiteral("pbUp1MHz"));
        pbUp1MHz->setMaximumSize(QSize(30, 16777215));

        horizontalLayout->addWidget(pbUp1MHz);

        pbUp10MHz = new QPushButton(ControlPanelWidget);
        pbUp10MHz->setObjectName(QStringLiteral("pbUp10MHz"));
        pbUp10MHz->setMaximumSize(QSize(30, 16777215));

        horizontalLayout->addWidget(pbUp10MHz);

        pbUp100MHz = new QPushButton(ControlPanelWidget);
        pbUp100MHz->setObjectName(QStringLiteral("pbUp100MHz"));
        pbUp100MHz->setMaximumSize(QSize(30, 16777215));

        horizontalLayout->addWidget(pbUp100MHz);

        commonFrequencyPB = new QPushButton(ControlPanelWidget);
        commonFrequencyPB->setObjectName(QStringLiteral("commonFrequencyPB"));

        horizontalLayout->addWidget(commonFrequencyPB);


        retranslateUi(ControlPanelWidget);

        QMetaObject::connectSlotsByName(ControlPanelWidget);
    } // setupUi

    void retranslateUi(QWidget *ControlPanelWidget)
    {
        ControlPanelWidget->setWindowTitle(QApplication::translate("ControlPanelWidget", "Form", 0));
        startFreqSB->setSpecialValueText(QString());
        startFreqSB->setSuffix(QApplication::translate("ControlPanelWidget", " MHz", 0));
        toLabel->setText(QApplication::translate("ControlPanelWidget", "-", 0));
        endFreqSB->setSpecialValueText(QString());
        endFreqSB->setSuffix(QApplication::translate("ControlPanelWidget", " MHz", 0));
        endFreqSB->setPrefix(QString());
        panoramaPB->setText(QApplication::translate("ControlPanelWidget", "Set panorama", 0));
        autosearchCB->setText(QApplication::translate("ControlPanelWidget", "Autosearch", 0));
        panoramaCB->setText(QApplication::translate("ControlPanelWidget", "Panorama", 0));
        cbMode->clear();
        cbMode->insertItems(0, QStringList()
         << QApplication::translate("ControlPanelWidget", "Manual", 0)
         << QApplication::translate("ControlPanelWidget", "Scanning", 0)
         << QApplication::translate("ControlPanelWidget", "Checking", 0)
         << QApplication::translate("ControlPanelWidget", "Escorting", 0)
        );
        correlationStatusLabelActive->setText(QString());
        correlationStatusLabelValue->setText(QApplication::translate("ControlPanelWidget", "---", 0));
        correlationStatusLabel->setText(QApplication::translate("ControlPanelWidget", "Correlation on frequency ", 0));
        solverQualityNameLB->setText(QApplication::translate("ControlPanelWidget", "Solver quality", 0));
        solverQualityLB->setText(QString());
        pbDown100MHz->setText(QApplication::translate("ControlPanelWidget", "<<<", 0));
        pbDown10MHz->setText(QApplication::translate("ControlPanelWidget", "<<", 0));
        pbDown1MHz->setText(QApplication::translate("ControlPanelWidget", "<", 0));
        commonFreqSB->setSuffix(QApplication::translate("ControlPanelWidget", " MHz", 0));
        pbUp1MHz->setText(QApplication::translate("ControlPanelWidget", ">", 0));
        pbUp10MHz->setText(QApplication::translate("ControlPanelWidget", ">>", 0));
        pbUp100MHz->setText(QApplication::translate("ControlPanelWidget", ">>>", 0));
        commonFrequencyPB->setText(QApplication::translate("ControlPanelWidget", "Set common frequency", 0));
    } // retranslateUi

};

namespace Ui {
    class ControlPanelWidget: public Ui_ControlPanelWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLPANEL_H
