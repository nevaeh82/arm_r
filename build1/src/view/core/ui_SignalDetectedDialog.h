/********************************************************************************
** Form generated from reading UI file 'SignalDetectedDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNALDETECTEDDIALOG_H
#define UI_SIGNALDETECTEDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_SignalDetectedDialog
{
public:
    QGridLayout *gridLayout;
    QTextEdit *teDetected;
    QHBoxLayout *horizontalLayout;
    QLabel *lblFreq;
    QLabel *lblMessage;
    QDialogButtonBox *buttonBox;
    QLineEdit *leName;

    void setupUi(QDialog *SignalDetectedDialog)
    {
        if (SignalDetectedDialog->objectName().isEmpty())
            SignalDetectedDialog->setObjectName(QStringLiteral("SignalDetectedDialog"));
        SignalDetectedDialog->resize(343, 256);
        gridLayout = new QGridLayout(SignalDetectedDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        teDetected = new QTextEdit(SignalDetectedDialog);
        teDetected->setObjectName(QStringLiteral("teDetected"));
        teDetected->setReadOnly(true);
        teDetected->setTabStopWidth(8000);

        gridLayout->addWidget(teDetected, 3, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lblFreq = new QLabel(SignalDetectedDialog);
        lblFreq->setObjectName(QStringLiteral("lblFreq"));

        horizontalLayout->addWidget(lblFreq);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);

        lblMessage = new QLabel(SignalDetectedDialog);
        lblMessage->setObjectName(QStringLiteral("lblMessage"));
        QFont font;
        font.setFamily(QStringLiteral("Arial Black"));
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        lblMessage->setFont(font);
        lblMessage->setLineWidth(1);
        lblMessage->setScaledContents(false);

        gridLayout->addWidget(lblMessage, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(SignalDetectedDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 4, 0, 1, 1);

        leName = new QLineEdit(SignalDetectedDialog);
        leName->setObjectName(QStringLiteral("leName"));
        leName->setEnabled(false);
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setWeight(75);
        leName->setFont(font1);
        leName->setMaxLength(20);
        leName->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(leName, 1, 0, 1, 1);


        retranslateUi(SignalDetectedDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), SignalDetectedDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SignalDetectedDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(SignalDetectedDialog);
    } // setupUi

    void retranslateUi(QDialog *SignalDetectedDialog)
    {
        SignalDetectedDialog->setWindowTitle(QApplication::translate("SignalDetectedDialog", "Dialog", 0));
        lblFreq->setText(QApplication::translate("SignalDetectedDialog", "Overthreshols on follow Frequencies", 0));
        lblMessage->setText(QApplication::translate("SignalDetectedDialog", "Attention! Signal has been detected!", 0));
    } // retranslateUi

};

namespace Ui {
    class SignalDetectedDialog: public Ui_SignalDetectedDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNALDETECTEDDIALOG_H
