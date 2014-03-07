/********************************************************************************
** Form generated from reading UI file 'arm_od_view.ui'
**
** Created: Thu 6. Mar 17:46:27 2014
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARM_OD_VIEW_H
#define UI_ARM_OD_VIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ARM_OD_view
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ARM_OD_view)
    {
        if (ARM_OD_view->objectName().isEmpty())
            ARM_OD_view->setObjectName(QString::fromUtf8("ARM_OD_view"));
        ARM_OD_view->resize(800, 600);
        centralwidget = new QWidget(ARM_OD_view);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        ARM_OD_view->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ARM_OD_view);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        ARM_OD_view->setMenuBar(menubar);
        statusbar = new QStatusBar(ARM_OD_view);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ARM_OD_view->setStatusBar(statusbar);

        retranslateUi(ARM_OD_view);

        QMetaObject::connectSlotsByName(ARM_OD_view);
    } // setupUi

    void retranslateUi(QMainWindow *ARM_OD_view)
    {
        ARM_OD_view->setWindowTitle(QApplication::translate("ARM_OD_view", "MainWindow", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ARM_OD_view: public Ui_ARM_OD_view {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARM_OD_VIEW_H
