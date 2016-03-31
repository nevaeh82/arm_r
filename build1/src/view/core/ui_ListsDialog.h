/********************************************************************************
** Form generated from reading UI file 'ListsDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISTSDIALOG_H
#define UI_LISTSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ListsDialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QComboBox *cbLists;
    QTableView *tvLists;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbDelete;
    QPushButton *pbReport;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbAdd;
    QPushButton *pbClose;

    void setupUi(QWidget *ListsDialog)
    {
        if (ListsDialog->objectName().isEmpty())
            ListsDialog->setObjectName(QStringLiteral("ListsDialog"));
        ListsDialog->resize(516, 365);
        gridLayout = new QGridLayout(ListsDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        cbLists = new QComboBox(ListsDialog);
        cbLists->setObjectName(QStringLiteral("cbLists"));

        verticalLayout->addWidget(cbLists);

        tvLists = new QTableView(ListsDialog);
        tvLists->setObjectName(QStringLiteral("tvLists"));
        tvLists->setSelectionBehavior(QAbstractItemView::SelectRows);
        tvLists->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        tvLists->setShowGrid(true);
        tvLists->verticalHeader()->setHighlightSections(false);

        verticalLayout->addWidget(tvLists);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pbDelete = new QPushButton(ListsDialog);
        pbDelete->setObjectName(QStringLiteral("pbDelete"));

        horizontalLayout->addWidget(pbDelete);

        pbReport = new QPushButton(ListsDialog);
        pbReport->setObjectName(QStringLiteral("pbReport"));

        horizontalLayout->addWidget(pbReport);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pbAdd = new QPushButton(ListsDialog);
        pbAdd->setObjectName(QStringLiteral("pbAdd"));

        horizontalLayout->addWidget(pbAdd);

        pbClose = new QPushButton(ListsDialog);
        pbClose->setObjectName(QStringLiteral("pbClose"));

        horizontalLayout->addWidget(pbClose);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);


        retranslateUi(ListsDialog);

        QMetaObject::connectSlotsByName(ListsDialog);
    } // setupUi

    void retranslateUi(QWidget *ListsDialog)
    {
        ListsDialog->setWindowTitle(QApplication::translate("ListsDialog", "Form", 0));
        cbLists->clear();
        cbLists->insertItems(0, QStringList()
         << QApplication::translate("ListsDialog", "All", 0)
         << QApplication::translate("ListsDialog", "White", 0)
         << QApplication::translate("ListsDialog", "Black", 0)
        );
        pbDelete->setText(QApplication::translate("ListsDialog", "Delete", 0));
        pbReport->setText(QApplication::translate("ListsDialog", "Report", 0));
        pbAdd->setText(QApplication::translate("ListsDialog", "Add", 0));
        pbClose->setText(QApplication::translate("ListsDialog", "Close", 0));
    } // retranslateUi

};

namespace Ui {
    class ListsDialog: public Ui_ListsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISTSDIALOG_H
