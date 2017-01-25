#include "TabSpectrumWidget.h"
#include <QDebug>

#include "ui_TabSpectrumWidget.h"

TabSpectrumWidget::TabSpectrumWidget(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::TabSpectrumWidget),
	m_cpView(NULL),
	m_id(-1)
{
	ui->setupUi(this);

	m_spectrumWidget = new SpectrumWidget(this);

	m_spectrumWidgetController = new SpectrumWidgetController(m_spectrumWidget);
	m_spectrumWidgetController->appendView(m_spectrumWidget);

	insertSpectrumWidget(m_spectrumWidgetController);

	m_pmRoundRed = new QPixmap(":/images/bullet_red.png");
	m_pmRoundGreen = new QPixmap(":/images/bullet_green.png");
	m_pmRoundYellow = new QPixmap(":/images/bullet_yellow.png");

	m_indicatorLabel = new QLabel(this);
	m_indicatorLabel->setFixedSize(16, 16);
	m_indicatorLabel->setPixmap(m_pmRoundRed->scaled(16,16,Qt::KeepAspectRatio));

	connect(this, SIGNAL(setIndicatorStateSignal(int)), this, SLOT(setIndicatorStateSlot(int)));

	connect(m_spectrumWidgetController, SIGNAL(doubleClickedSignal(int)), this, SIGNAL(spectrumDoubleClickedSignal(int)));

	ui->AlanysisGroupWidget->setVisible(true);

	ui->settingsTreeView->setVisible(false);
}

TabSpectrumWidget::~TabSpectrumWidget()
{
	delete m_pmRoundRed;
	delete m_pmRoundGreen;
	delete m_pmRoundYellow;

	ui->cpLayout->removeWidget( m_cpView );
	m_cpView->setParent(NULL);
}

SpectrumWidgetController *TabSpectrumWidget::getSpectrumController()
{
	return m_spectrumWidgetController;
}

void TabSpectrumWidget::setControlPanelWidget(ControlPanelWidget *widget)
{
	m_cpView = widget;
}

void TabSpectrumWidget::activate()
{
	ui->cpLayout->addWidget( m_cpView );

	for(int i = 0; i < m_correlationWidgetsList.count(); i++){
		ui->correlationsGroupWidget->insertCorrelationWidget(i, m_correlationWidgetsList.at(i));
	}

	for(int i = 0; i < m_analysisWidgetsList.count(); i++){
		int test = m_analysisWidgetsList.at(i)->getAnalysisWorkId();
		if(m_analysisWidgetsList.at(i)->getAnalysisWorkId() == m_id) {
			ui->AlanysisGroupWidget->insertAnalysisWidget(m_analysisWidgetsList.at(i));
		}
	}

	foreach (ISpectrumWidget* spectrumWidget, m_spectrumWidgetsList) {
		ui->spectumWidgetsContainer->insertWidget(ui->spectumWidgetsContainer->count(), spectrumWidget->getWidget());
	}

	getSpectrumWidget()->updateSelection();
}

void TabSpectrumWidget::deactivate()
{
	ui->correlationsGroupWidget->clearWidgetContainer();
	ui->AlanysisGroupWidget->clearWidgetContainer();

	foreach (ISpectrumWidget* spectrumWidget , m_spectrumWidgetsList) {
		ui->spectumWidgetsContainer->removeWidget(spectrumWidget->getWidget());
	}

//	for(int i = 0; i < m_analysisWidgetsList.count(); i++){
//		ui->AlanysisGroupWidget->insertAnalysisWidget(m_analysisWidgetsList.at(i));
//	}
}

QWidget *TabSpectrumWidget::getWidget()
{
	return this;
}

QLabel *TabSpectrumWidget::getIndicator()
{
	return m_indicatorLabel;
}

ISpectrumWidget *TabSpectrumWidget::getSpectrumWidget()
{
	return m_spectrumWidgetController;
}

void TabSpectrumWidget::insertSpectrumWidget(ISpectrumWidget *spectrumWidget)
{
	if (NULL == spectrumWidget) {
		return;
	}

	m_spectrumWidgetsList.append(spectrumWidget);

	ui->spectumWidgetsContainer->insertWidget(ui->spectumWidgetsContainer->count(), spectrumWidget->getWidget());
}

void TabSpectrumWidget::setIndicatorState(int state)
{
	emit setIndicatorStateSignal(state);
}

void TabSpectrumWidget::updateDBListsAreas()
{
	m_spectrumWidgetController->updateDBAreas();
}

QTreeView *TabSpectrumWidget::getTreeView() const
{
	return ui->settingsTreeView;
}

void TabSpectrumWidget::insertCorrelationWidget(ICorrelationWidget *correlationWidget)
{
	if (NULL == correlationWidget) {
		return;
	}

	m_correlationWidgetsList.append(correlationWidget);

	//ui->correlationsGroupWidget->insertCorrelationWidget(correlationWidget);
}

void TabSpectrumWidget::insertAnalysisWidget(IAnalysisWidget *analysisWidget)
{
	if (NULL == analysisWidget) {
		return;
	}

	m_analysisWidgetsList.append(analysisWidget);
}

void TabSpectrumWidget::setRpcFlakonClient(RpcFlakonClientWrapper *client)
{
	m_rpcFlakonClient = client;
	m_spectrumWidgetController->setRpcFlakonClient(m_rpcFlakonClient);
}

RpcFlakonClientWrapper* TabSpectrumWidget::getRpcFlakonClient()
{
	return m_rpcFlakonClient;
}

void TabSpectrumWidget::setIndicatorStateSlot(int state)
{
	switch(state)
	{
	case 1:
		m_indicatorLabel->setPixmap(m_pmRoundGreen->scaled(16,16,Qt::KeepAspectRatio));
		break;
	case 0:
		m_indicatorLabel->setPixmap(m_pmRoundRed->scaled(16,16,Qt::KeepAspectRatio));
		break;
	case 2:
		m_indicatorLabel->setPixmap(m_pmRoundYellow->scaled(16,16,Qt::KeepAspectRatio));
		break;
	case 3:
		m_indicatorLabel->setPixmap(m_pmRoundGreen->scaled(16,16,Qt::KeepAspectRatio));
		break;
	default:
		m_indicatorLabel->setPixmap(m_pmRoundRed->scaled(16,16,Qt::KeepAspectRatio));
		break;
	}
}

