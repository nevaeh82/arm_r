#include "AnalysisWidget.h"

#include "ui_AnalysisWidget.h"

#include <Logger/Logger.h>

AnalysisWidget::AnalysisWidget(int type, QWidget *parent, Qt::WFlags flags):
	QWidget(parent, flags),
	ui(new Ui::AnalysisWidget),
	m_type(type)
{
	ui->setupUi(this);

	connect(ui->widget, SIGNAL(analysisReady(int)), this, SIGNAL(analysisReady(int)));
	connect(ui->widget, SIGNAL(analysisSelection(int,double,double)), this, SIGNAL(signalAnalysisSelection(int, double, double)));
	connect(ui->widget, SIGNAL(onAddToList(double,double)), this, SIGNAL(signalAddToList(double, double)));
	connect(ui->widget, SIGNAL(signalContinue(int, bool)), this, SIGNAL(signalAnalysisContinue(int, bool)));
	connect(ui->widget, SIGNAL(signalClose()), this, SIGNAL(signalAnalysisClose()));
}

AnalysisWidget::~AnalysisWidget()
{
}

void AnalysisWidget::setAnalysisData(const RdsProtobuf::ServerMessage_OneShotData_AnalysisData &adata)
{
	this->setVisible(true);
	ui->widget->setAnalysisData( adata );
}

int AnalysisWidget::getId()
{
	return ui->widget->getCurrentIndex();
}
