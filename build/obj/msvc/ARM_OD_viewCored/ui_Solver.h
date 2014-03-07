/********************************************************************************
** Form generated from reading UI file 'Solver.ui'
**
** Created: Thu 6. Mar 17:46:27 2014
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOLVER_H
#define UI_SOLVER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Solver
{
public:

    void setupUi(QWidget *Solver)
    {
        if (Solver->objectName().isEmpty())
            Solver->setObjectName(QString::fromUtf8("Solver"));
        Solver->resize(400, 300);

        retranslateUi(Solver);

        QMetaObject::connectSlotsByName(Solver);
    } // setupUi

    void retranslateUi(QWidget *Solver)
    {
        Solver->setWindowTitle(QApplication::translate("Solver", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Solver: public Ui_Solver {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOLVER_H
