/********************************************************************************
** Form generated from reading UI file 'AddStationDataDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDSTATIONDATADIALOG_H
#define UI_ADDSTATIONDATADIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_AddStationDataDialog
{
public:
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbAccept;
    QPushButton *pbClear;
    QPushButton *pbClose;
    QGridLayout *gridLayout;
    QLabel *lblPort;
    QSpinBox *sbPort;
    QLabel *lblCategory;
    QComboBox *cbStation;
    QLabel *lblStation;
    QLabel *lblFrequency;
    QComboBox *cbCategory;
    QLabel *lblBandwidth;
    QDoubleSpinBox *dsbFrequency;
    QDoubleSpinBox *dsbBandwidth;
    QLabel *lblSigType;
    QComboBox *cbSignalType;
    QLabel *lblMemo;
    QLineEdit *leMemo;

    void setupUi(QDialog *AddStationDataDialog)
    {
        if (AddStationDataDialog->objectName().isEmpty())
            AddStationDataDialog->setObjectName(QStringLiteral("AddStationDataDialog"));
        AddStationDataDialog->resize(400, 208);
        gridLayout_3 = new QGridLayout(AddStationDataDialog);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pbAccept = new QPushButton(AddStationDataDialog);
        pbAccept->setObjectName(QStringLiteral("pbAccept"));

        horizontalLayout->addWidget(pbAccept);

        pbClear = new QPushButton(AddStationDataDialog);
        pbClear->setObjectName(QStringLiteral("pbClear"));

        horizontalLayout->addWidget(pbClear);

        pbClose = new QPushButton(AddStationDataDialog);
        pbClose->setObjectName(QStringLiteral("pbClose"));

        horizontalLayout->addWidget(pbClose);


        gridLayout_2->addLayout(horizontalLayout, 1, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        lblPort = new QLabel(AddStationDataDialog);
        lblPort->setObjectName(QStringLiteral("lblPort"));

        gridLayout->addWidget(lblPort, 1, 0, 1, 1);

        sbPort = new QSpinBox(AddStationDataDialog);
        sbPort->setObjectName(QStringLiteral("sbPort"));
        sbPort->setMaximum(65535);
        sbPort->setValue(1024);

        gridLayout->addWidget(sbPort, 1, 1, 1, 1);

        lblCategory = new QLabel(AddStationDataDialog);
        lblCategory->setObjectName(QStringLiteral("lblCategory"));

        gridLayout->addWidget(lblCategory, 2, 0, 1, 1);

        cbStation = new QComboBox(AddStationDataDialog);
        cbStation->setObjectName(QStringLiteral("cbStation"));

        gridLayout->addWidget(cbStation, 0, 1, 1, 1);

        lblStation = new QLabel(AddStationDataDialog);
        lblStation->setObjectName(QStringLiteral("lblStation"));

        gridLayout->addWidget(lblStation, 0, 0, 1, 1);

        lblFrequency = new QLabel(AddStationDataDialog);
        lblFrequency->setObjectName(QStringLiteral("lblFrequency"));

        gridLayout->addWidget(lblFrequency, 3, 0, 1, 1);

        cbCategory = new QComboBox(AddStationDataDialog);
        cbCategory->setObjectName(QStringLiteral("cbCategory"));

        gridLayout->addWidget(cbCategory, 2, 1, 1, 1);

        lblBandwidth = new QLabel(AddStationDataDialog);
        lblBandwidth->setObjectName(QStringLiteral("lblBandwidth"));

        gridLayout->addWidget(lblBandwidth, 4, 0, 1, 1);

        dsbFrequency = new QDoubleSpinBox(AddStationDataDialog);
        dsbFrequency->setObjectName(QStringLiteral("dsbFrequency"));
        dsbFrequency->setMinimum(300);
        dsbFrequency->setMaximum(7000);
        dsbFrequency->setSingleStep(1);

        gridLayout->addWidget(dsbFrequency, 3, 1, 1, 1);

        dsbBandwidth = new QDoubleSpinBox(AddStationDataDialog);
        dsbBandwidth->setObjectName(QStringLiteral("dsbBandwidth"));
        dsbBandwidth->setMaximum(6700);

        gridLayout->addWidget(dsbBandwidth, 4, 1, 1, 1);

        lblSigType = new QLabel(AddStationDataDialog);
        lblSigType->setObjectName(QStringLiteral("lblSigType"));

        gridLayout->addWidget(lblSigType, 5, 0, 1, 1);

        cbSignalType = new QComboBox(AddStationDataDialog);
        cbSignalType->setObjectName(QStringLiteral("cbSignalType"));

        gridLayout->addWidget(cbSignalType, 5, 1, 1, 1);

        lblMemo = new QLabel(AddStationDataDialog);
        lblMemo->setObjectName(QStringLiteral("lblMemo"));

        gridLayout->addWidget(lblMemo, 6, 0, 1, 1);

        leMemo = new QLineEdit(AddStationDataDialog);
        leMemo->setObjectName(QStringLiteral("leMemo"));

        gridLayout->addWidget(leMemo, 6, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 0, 0, 1, 1);


        retranslateUi(AddStationDataDialog);

        QMetaObject::connectSlotsByName(AddStationDataDialog);
    } // setupUi

    void retranslateUi(QDialog *AddStationDataDialog)
    {
        AddStationDataDialog->setWindowTitle(QApplication::translate("AddStationDataDialog", "Dialog", 0));
        pbAccept->setText(QApplication::translate("AddStationDataDialog", "Accept", 0));
        pbClear->setText(QApplication::translate("AddStationDataDialog", "Clear", 0));
        pbClose->setText(QApplication::translate("AddStationDataDialog", "Close", 0));
        lblPort->setText(QApplication::translate("AddStationDataDialog", "Port", 0));
        lblCategory->setText(QApplication::translate("AddStationDataDialog", "Category", 0));
        lblStation->setText(QApplication::translate("AddStationDataDialog", "Station", 0));
        lblFrequency->setText(QApplication::translate("AddStationDataDialog", "Frequency", 0));
        lblBandwidth->setText(QApplication::translate("AddStationDataDialog", "Bandwidth", 0));
        lblSigType->setText(QApplication::translate("AddStationDataDialog", "Signal Type", 0));
        lblMemo->setText(QApplication::translate("AddStationDataDialog", "Memo", 0));
    } // retranslateUi

};

namespace Ui {
    class AddStationDataDialog: public Ui_AddStationDataDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDSTATIONDATADIALOG_H
