/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "ControlPanel/ControlPanelWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew;
    QAction *actionExit;
    QAction *actionAtlantSetup;
    QAction *actionLists;
    QAction *actionSolverLogs;
    QAction *actionRestartServer;
    QAction *actionSolverErrors;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    ControlPanelWidget *controlWidget;
    QStackedWidget *stackedWidget;
    QWidget *tabPage;
    QVBoxLayout *verticalLayout_5;
    QTabWidget *worlTabsWidget;
    QWidget *initializatioPage;
    QVBoxLayout *verticalLayout_6;
    QLabel *initializationLabel;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuSettings;
    QMenu *menuServer;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 600);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionNew->setEnabled(false);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionAtlantSetup = new QAction(MainWindow);
        actionAtlantSetup->setObjectName(QStringLiteral("actionAtlantSetup"));
        actionLists = new QAction(MainWindow);
        actionLists->setObjectName(QStringLiteral("actionLists"));
        actionSolverLogs = new QAction(MainWindow);
        actionSolverLogs->setObjectName(QStringLiteral("actionSolverLogs"));
        actionRestartServer = new QAction(MainWindow);
        actionRestartServer->setObjectName(QStringLiteral("actionRestartServer"));
        actionSolverErrors = new QAction(MainWindow);
        actionSolverErrors->setObjectName(QStringLiteral("actionSolverErrors"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        controlWidget = new ControlPanelWidget(centralwidget);
        controlWidget->setObjectName(QStringLiteral("controlWidget"));

        verticalLayout->addWidget(controlWidget);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        sizePolicy.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy);
        stackedWidget->setFrameShadow(QFrame::Plain);
        tabPage = new QWidget();
        tabPage->setObjectName(QStringLiteral("tabPage"));
        verticalLayout_5 = new QVBoxLayout(tabPage);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        worlTabsWidget = new QTabWidget(tabPage);
        worlTabsWidget->setObjectName(QStringLiteral("worlTabsWidget"));
        worlTabsWidget->setEnabled(true);

        verticalLayout_5->addWidget(worlTabsWidget);

        stackedWidget->addWidget(tabPage);
        initializatioPage = new QWidget();
        initializatioPage->setObjectName(QStringLiteral("initializatioPage"));
        verticalLayout_6 = new QVBoxLayout(initializatioPage);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        initializationLabel = new QLabel(initializatioPage);
        initializationLabel->setObjectName(QStringLiteral("initializationLabel"));
        QFont font;
        font.setPointSize(18);
        initializationLabel->setFont(font);
        initializationLabel->setLayoutDirection(Qt::LeftToRight);
        initializationLabel->setTextFormat(Qt::AutoText);
        initializationLabel->setScaledContents(false);
        initializationLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(initializationLabel);

        stackedWidget->addWidget(initializatioPage);

        verticalLayout->addWidget(stackedWidget);


        verticalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuSettings = new QMenu(menubar);
        menuSettings->setObjectName(QStringLiteral("menuSettings"));
        menuServer = new QMenu(menubar);
        menuServer->setObjectName(QStringLiteral("menuServer"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuSettings->menuAction());
        menubar->addAction(menuServer->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionExit);
        menuSettings->addAction(actionAtlantSetup);
        menuSettings->addAction(actionLists);
        menuSettings->addAction(actionSolverLogs);
        menuSettings->addAction(actionSolverErrors);
        menuServer->addAction(actionRestartServer);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);
        worlTabsWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionNew->setText(QApplication::translate("MainWindow", "New", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionAtlantSetup->setText(QApplication::translate("MainWindow", "Atlant setup", 0));
        actionLists->setText(QApplication::translate("MainWindow", "Lists", 0));
        actionSolverLogs->setText(QApplication::translate("MainWindow", "SolverLogs", 0));
        actionRestartServer->setText(QApplication::translate("MainWindow", "Restart Server", 0));
        actionSolverErrors->setText(QApplication::translate("MainWindow", "SolverErrors", 0));
        initializationLabel->setText(QApplication::translate("MainWindow", "Initialization...", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuSettings->setTitle(QApplication::translate("MainWindow", "Settings", 0));
        menuServer->setTitle(QApplication::translate("MainWindow", "Server", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
