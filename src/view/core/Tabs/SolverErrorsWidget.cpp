#include "SolverErrorsWidget.h"
#include "ui_SolverErrorsWidget.h"

#include <QScrollBar>

SolverErrorsWidget::SolverErrorsWidget(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SolverErrorsWidget)
{
	ui->setupUi(this);

	// Hide "?" button
	Qt::WindowFlags flags = windowFlags();
	Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
	flags = flags & (~helpFlag);

	setWindowFlags(flags);

	this->setWindowTitle(tr("Solver errors"));
	setWindowIcon(QIcon(":/images/icons/ARM_R.png"));
}

SolverErrorsWidget::~SolverErrorsWidget()
{
	delete ui;
}

void SolverErrorsWidget::appendSolverError(const QString &error)
{
    ui->teLogResults->clear();

	ui->teLogResults->append(error);

	QScrollBar *tempScrollBar = ui->teLogResults->verticalScrollBar();
	if(ui->checkBoxAutoScroll->isChecked()) {
		tempScrollBar->setValue(tempScrollBar->maximum());
	}
}
