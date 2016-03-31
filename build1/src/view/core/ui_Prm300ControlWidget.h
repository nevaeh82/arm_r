/********************************************************************************
** Form generated from reading UI file 'Prm300ControlWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRM300CONTROLWIDGET_H
#define UI_PRM300CONTROLWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Prm300ControlWidget
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *gbAtt1;
    QGridLayout *gridLayout;
    QToolButton *pbAtt1Down;
    QLCDNumber *lcdAtt1;
    QToolButton *pbAtt1Up;
    QGroupBox *gbAtt2;
    QGridLayout *gridLayout_2;
    QToolButton *pbAtt2Down;
    QLCDNumber *lcdAtt2;
    QToolButton *pbAtt2Up;
    QComboBox *cbFilter;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *Prm300ControlWidget)
    {
        if (Prm300ControlWidget->objectName().isEmpty())
            Prm300ControlWidget->setObjectName(QStringLiteral("Prm300ControlWidget"));
        Prm300ControlWidget->resize(108, 132);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Prm300ControlWidget->sizePolicy().hasHeightForWidth());
        Prm300ControlWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(Prm300ControlWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        gbAtt1 = new QGroupBox(Prm300ControlWidget);
        gbAtt1->setObjectName(QStringLiteral("gbAtt1"));
        gbAtt1->setAlignment(Qt::AlignCenter);
        gridLayout = new QGridLayout(gbAtt1);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 3);
        pbAtt1Down = new QToolButton(gbAtt1);
        pbAtt1Down->setObjectName(QStringLiteral("pbAtt1Down"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pbAtt1Down->sizePolicy().hasHeightForWidth());
        pbAtt1Down->setSizePolicy(sizePolicy1);
        pbAtt1Down->setMinimumSize(QSize(31, 31));
        pbAtt1Down->setIconSize(QSize(5, 5));

        gridLayout->addWidget(pbAtt1Down, 0, 0, 1, 1);

        lcdAtt1 = new QLCDNumber(gbAtt1);
        lcdAtt1->setObjectName(QStringLiteral("lcdAtt1"));
        lcdAtt1->setDigitCount(2);
        lcdAtt1->setProperty("intValue", QVariant(0));

        gridLayout->addWidget(lcdAtt1, 0, 1, 1, 1);

        pbAtt1Up = new QToolButton(gbAtt1);
        pbAtt1Up->setObjectName(QStringLiteral("pbAtt1Up"));
        sizePolicy1.setHeightForWidth(pbAtt1Up->sizePolicy().hasHeightForWidth());
        pbAtt1Up->setSizePolicy(sizePolicy1);
        pbAtt1Up->setMinimumSize(QSize(31, 31));
        pbAtt1Up->setIconSize(QSize(16, 16));

        gridLayout->addWidget(pbAtt1Up, 0, 2, 1, 1);


        verticalLayout->addWidget(gbAtt1);

        gbAtt2 = new QGroupBox(Prm300ControlWidget);
        gbAtt2->setObjectName(QStringLiteral("gbAtt2"));
        gbAtt2->setAlignment(Qt::AlignCenter);
        gridLayout_2 = new QGridLayout(gbAtt2);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 3);
        pbAtt2Down = new QToolButton(gbAtt2);
        pbAtt2Down->setObjectName(QStringLiteral("pbAtt2Down"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pbAtt2Down->sizePolicy().hasHeightForWidth());
        pbAtt2Down->setSizePolicy(sizePolicy2);
        pbAtt2Down->setMinimumSize(QSize(31, 31));
        pbAtt2Down->setIconSize(QSize(16, 16));

        gridLayout_2->addWidget(pbAtt2Down, 0, 0, 1, 1);

        lcdAtt2 = new QLCDNumber(gbAtt2);
        lcdAtt2->setObjectName(QStringLiteral("lcdAtt2"));
        lcdAtt2->setDigitCount(2);
        lcdAtt2->setProperty("intValue", QVariant(0));

        gridLayout_2->addWidget(lcdAtt2, 0, 1, 1, 1);

        pbAtt2Up = new QToolButton(gbAtt2);
        pbAtt2Up->setObjectName(QStringLiteral("pbAtt2Up"));
        sizePolicy2.setHeightForWidth(pbAtt2Up->sizePolicy().hasHeightForWidth());
        pbAtt2Up->setSizePolicy(sizePolicy2);
        pbAtt2Up->setMinimumSize(QSize(31, 31));
        pbAtt2Up->setIconSize(QSize(16, 16));

        gridLayout_2->addWidget(pbAtt2Up, 0, 2, 1, 1);


        verticalLayout->addWidget(gbAtt2);

        cbFilter = new QComboBox(Prm300ControlWidget);
        cbFilter->setObjectName(QStringLiteral("cbFilter"));

        verticalLayout->addWidget(cbFilter);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(Prm300ControlWidget);

        QMetaObject::connectSlotsByName(Prm300ControlWidget);
    } // setupUi

    void retranslateUi(QWidget *Prm300ControlWidget)
    {
        Prm300ControlWidget->setWindowTitle(QApplication::translate("Prm300ControlWidget", "Form", 0));
        gbAtt1->setTitle(QApplication::translate("Prm300ControlWidget", "Attenuator 1", 0));
        pbAtt1Down->setText(QApplication::translate("Prm300ControlWidget", "-", 0));
        pbAtt1Up->setText(QApplication::translate("Prm300ControlWidget", "+", 0));
        gbAtt2->setTitle(QApplication::translate("Prm300ControlWidget", "Attenuator 2", 0));
        pbAtt2Down->setText(QApplication::translate("Prm300ControlWidget", "-", 0));
        pbAtt2Up->setText(QApplication::translate("Prm300ControlWidget", "+", 0));
        cbFilter->clear();
        cbFilter->insertItems(0, QStringList()
         << QApplication::translate("Prm300ControlWidget", "140/70 MHz", 0)
         << QApplication::translate("Prm300ControlWidget", "140/22 MHz", 0)
         << QApplication::translate("Prm300ControlWidget", "140/20 MHz", 0)
         << QApplication::translate("Prm300ControlWidget", "150/80 MHz", 0)
        );
    } // retranslateUi

};

namespace Ui {
    class Prm300ControlWidget: public Ui_Prm300ControlWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRM300CONTROLWIDGET_H
