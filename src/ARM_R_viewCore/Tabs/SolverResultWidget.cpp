#include "SolverResultWidget.h"
#include "ui_SolverResultWidget.h"

SolverResultWidget::SolverResultWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SolverResultWidget)
{
	ui->setupUi(this);

	this->setWindowTitle(tr("Solver log"));
	setWindowIcon(QIcon(":/images/icons/ARM_R.png"));

	this->hide();
}

SolverResultWidget::~SolverResultWidget()
{
	delete ui;
}

void SolverResultWidget::appendSolverResult(const QString &log)
{
	ui->teLogResults->append(log);

	QScrollBar *tempScrollBar = ui->teLogResults->verticalScrollBar();
	if(ui->checkBoxAutoScroll->isChecked()) {
		tempScrollBar->setValue(tempScrollBar->maximum());
	}
}
