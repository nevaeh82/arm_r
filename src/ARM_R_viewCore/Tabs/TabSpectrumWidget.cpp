#include "TabSpectrumWidget.h"
#include <QDebug>

#include "ui_TabSpectrumWidget.h"

TabSpectrumWidget::TabSpectrumWidget(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::TabSpectrumWidget)
{
	ui->setupUi(this);

	m_spectrumWidget = new SpectrumWidget(this);

	m_spectrumWidgetController = new SpectrumWidgetController(m_spectrumWidget);
	m_spectrumWidgetController->appendView(m_spectrumWidget);

	insertSpectrumWidget(m_spectrumWidgetController);

	_pm_round_red = new QPixmap(":/images/signals/images/bullet_red.png");
	_pm_round_green = new QPixmap(":/images/signals/images/bullet_green.png");
	m_indicatorLabel = new QLabel(this);
	m_indicatorLabel->setFixedSize(16, 16);
	m_indicatorLabel->setPixmap(_pm_round_red->scaled(16,16,Qt::KeepAspectRatio));

	connect(this, SIGNAL(setIndicatorStateSignal(int)), this, SLOT(setIndicatorStateSlot(int)));

	connect(m_spectrumWidgetController, SIGNAL(doubleClickedSignal(int)), this, SIGNAL(spectrumDoubleClickedSignal(int)));

	///TODO: recheck
	activate();
}

TabSpectrumWidget::~TabSpectrumWidget()
{
}


void TabSpectrumWidget::activate()
{
	ui->correlationsGroupWidget->clearWidgetContainer();

	for(int i = 0; i < m_correlationWidgetsList.count(); i++){

		ui->correlationsGroupWidget->insertCorrelationWidget(m_correlationWidgetsList.at(i));
	}

	foreach (ISpectrumWidget* spectrumWidget, m_spectrumWidgetsList) {
		ui->spectumWidgetsContainer->insertWidget(ui->spectumWidgetsContainer->count(), spectrumWidget->getWidget());
	}
}

void TabSpectrumWidget::deactivate()
{

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
	/*
	if (m_spectrumWidgetsList.isEmpty()) {
		return NULL;
	}

	return m_spectrumWidgetsList.at(0);*/
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

void TabSpectrumWidget::insertCorrelationWidget(CorrelationWidget *correlationWidget)
{
	if (NULL == correlationWidget) {
		return;
	}

	m_correlationWidgetsList.append(correlationWidget);

	ui->correlationsGroupWidget->insertCorrelationWidget(correlationWidget);
}


void TabSpectrumWidget::setIndicatorStateSlot(int state)
{
	switch(state)
	{
		case 1:
			m_indicatorLabel->setPixmap(_pm_round_green->scaled(16,16,Qt::KeepAspectRatio));
			break;
		case 0:
			m_indicatorLabel->setPixmap(_pm_round_red->scaled(16,16,Qt::KeepAspectRatio));
			break;
		default:
			m_indicatorLabel->setPixmap(_pm_round_red->scaled(16,16,Qt::KeepAspectRatio));
			break;
	}
}
