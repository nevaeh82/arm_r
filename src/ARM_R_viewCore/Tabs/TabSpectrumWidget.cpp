#include "TabSpectrumWidget.h"
#include <QDebug>

#include "ui_TabSpectrumWidget.h"

TabSpectrumWidget::TabSpectrumWidget(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::TabSpectrumWidget)
{
	ui->setupUi(this);

	m_rpcPrmClient = NULL;

	m_spectrumWidget = new SpectrumWidget(this);

	m_spectrumWidgetController = new SpectrumWidgetController(m_spectrumWidget);
	m_spectrumWidgetController->appendView(m_spectrumWidget);

	insertSpectrumWidget(m_spectrumWidgetController);

	m_pmRoundRed = new QPixmap(":/images/bullet_red.png");
	m_pmRoundGreen = new QPixmap(":/images/bullet_green.png");
	m_indicatorLabel = new QLabel(this);
	m_indicatorLabel->setFixedSize(16, 16);
	m_indicatorLabel->setPixmap(m_pmRoundRed->scaled(16,16,Qt::KeepAspectRatio));

	connect(this, SIGNAL(setIndicatorStateSignal(int)), this, SLOT(setIndicatorStateSlot(int)));

	connect(m_spectrumWidgetController, SIGNAL(doubleClickedSignal(int)), this, SIGNAL(spectrumDoubleClickedSignal(int)));
}

TabSpectrumWidget::~TabSpectrumWidget()
{
}

void TabSpectrumWidget::activate()
{
	log_debug("Activate tab");
	for(int i = 0; i < m_correlationWidgetsList.count(); i++){
		ui->correlationsGroupWidget->insertCorrelationWidget(m_correlationWidgetsList.at(i));
	}

	foreach (ISpectrumWidget* spectrumWidget, m_spectrumWidgetsList) {
		ui->spectumWidgetsContainer->insertWidget(ui->spectumWidgetsContainer->count(), spectrumWidget->getWidget());
	}

	log_debug(QString::number(ui->spectumWidgetsContainer->count()));
}

void TabSpectrumWidget::deactivate()
{
	log_debug("Deactivate tab");
	ui->correlationsGroupWidget->clearWidgetContainer();

	foreach (ISpectrumWidget* spectrumWidget , m_spectrumWidgetsList) {
		ui->spectumWidgetsContainer->removeWidget(spectrumWidget->getWidget());
	}

	log_debug(QString::number(ui->spectumWidgetsContainer->count()));
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

	ui->correlationsGroupWidget->insertCorrelationWidget(correlationWidget);
}

void TabSpectrumWidget::setRpcPrmClient(RpcPrmClient* client)
{
	m_rpcPrmClient = client;
	m_spectrumWidgetController->setRpcPrmClient(m_rpcPrmClient);
}

RpcPrmClient *TabSpectrumWidget::getRpcClient()
{
	return m_rpcPrmClient;
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
		default:
			m_indicatorLabel->setPixmap(m_pmRoundRed->scaled(16,16,Qt::KeepAspectRatio));
			break;
	}
}
