/********************************************************************************
** Form generated from reading UI file 'blaperehvatdialog.ui'
**
** Created: Thu 6. Mar 17:46:27 2014
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLAPEREHVATDIALOG_H
#define UI_BLAPEREHVATDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_BLAPerehvatDialog
{
public:
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *BLAPerehvatDialog)
    {
        if (BLAPerehvatDialog->objectName().isEmpty())
            BLAPerehvatDialog->setObjectName(QString::fromUtf8("BLAPerehvatDialog"));
        BLAPerehvatDialog->resize(400, 300);
        buttonBox = new QDialogButtonBox(BLAPerehvatDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        retranslateUi(BLAPerehvatDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), BLAPerehvatDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), BLAPerehvatDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(BLAPerehvatDialog);
    } // setupUi

    void retranslateUi(QDialog *BLAPerehvatDialog)
    {
        BLAPerehvatDialog->setWindowTitle(QApplication::translate("BLAPerehvatDialog", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BLAPerehvatDialog: public Ui_BLAPerehvatDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLAPEREHVATDIALOG_H
