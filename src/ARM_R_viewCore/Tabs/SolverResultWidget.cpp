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

void SolverResultWidget::appendSolverResult(QByteArray &data)
{
	QDataStream ds(&data, QIODevice::ReadOnly);
	int sourceType;
	QString result;

	ds >> sourceType;
	ds >> result;

	QString source;

	switch(sourceType)
	{
		case 0:
			source = tr("AUTO");
			break;
		case 1:
			source = tr("MANUAL");
			break;
		case 2:
			source = tr("ALONE");
			break;
		default:
			break;
	}

	QString log = source + "   " + result + "\n";
	ui->teLogResults->append(log);
}
