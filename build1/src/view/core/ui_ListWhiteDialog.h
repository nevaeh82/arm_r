/********************************************************************************
** Form generated from reading UI file 'ListWhiteDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISTWHITEDIALOG_H
#define UI_LISTWHITEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ListWhiteDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTableView *tvWhiteList;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbAccept;
    QPushButton *pbCancel;

    void setupUi(QWidget *ListWhiteDialog)
    {
        if (ListWhiteDialog->objectName().isEmpty())
            ListWhiteDialog->setObjectName(QStringLiteral("ListWhiteDialog"));
        ListWhiteDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(ListWhiteDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tvWhiteList = new QTableView(ListWhiteDialog);
        tvWhiteList->setObjectName(QStringLiteral("tvWhiteList"));

        verticalLayout->addWidget(tvWhiteList);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pbAccept = new QPushButton(ListWhiteDialog);
        pbAccept->setObjectName(QStringLiteral("pbAccept"));

        horizontalLayout->addWidget(pbAccept);

        pbCancel = new QPushButton(ListWhiteDialog);
        pbCancel->setObjectName(QStringLiteral("pbCancel"));

        horizontalLayout->addWidget(pbCancel);


        horizontalLayout_2->addLayout(horizontalLayout);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(ListWhiteDialog);

        QMetaObject::connectSlotsByName(ListWhiteDialog);
    } // setupUi

    void retranslateUi(QWidget *ListWhiteDialog)
    {
        ListWhiteDialog->setWindowTitle(QApplication::translate("ListWhiteDialog", "White List Dialog", 0));
        pbAccept->setText(QApplication::translate("ListWhiteDialog", "Accept", 0));
        pbCancel->setText(QApplication::translate("ListWhiteDialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class ListWhiteDialog: public Ui_ListWhiteDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISTWHITEDIALOG_H
