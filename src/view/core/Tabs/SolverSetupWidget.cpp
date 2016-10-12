#include "SolverSetupWidget.h"
#include "ui_SolverSetupWidget.h"

SolverSetupWidget::SolverSetupWidget(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SolverSetupWidget)
{
	ui->setupUi(this);

	// Hide "?" button
	Qt::WindowFlags flags = windowFlags();
	Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
	flags = flags & (~helpFlag);

	setWindowFlags(flags);

	this->setWindowTitle(tr("Solver setup"));
	setWindowIcon(QIcon(":/images/icons/ARM_R.png"));

	this->hide();

    connect( ui->pushButton, SIGNAL(clicked(bool)), this, SIGNAL(onGetVersion()) );
}

SolverSetupWidget::~SolverSetupWidget()
{
	delete ui;
}

void SolverSetupWidget::setSolverVersion(const QString &version)
{
	ui->lblSolverVersion->setText( version );
}
