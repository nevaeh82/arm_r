#include "CommonSpectrumTabWidget.h"
#include "ui_CommonSpectrumTabWidget.h"

CommonSpectrumTabWidget::CommonSpectrumTabWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CommonSpectrumTabWidget)
{
	ui->setupUi(this);

	m_correlationComponent = NULL;
}

CommonSpectrumTabWidget::~CommonSpectrumTabWidget()
{
	delete ui;
}

//void CommonSpectrumTabWidget::insertSpectrumWidget(GraphicWidget *widget)
//{
//	if (NULL ==widget) {
//		return;
//	}

//	m_widgetList.append(widget);

//	ui->spectumWidgetsContainer->insertWidget(ui->spectumWidgetsContainer->count(), widget);
//}

void CommonSpectrumTabWidget::clearSpectrumWidgetsContainer()
{
	foreach (SpectrumWidget* widget , m_widgetList) {

		ui->spectumWidgetsContainer->removeWidget(widget);
	}

	m_widgetList.clear();
}

void CommonSpectrumTabWidget::setCorrelationComponent(ICommonComponents *correlation)
{
	//ui->correlationsGroupWidget->
	m_correlationComponent = correlation;

	for(int i = 0; i < m_correlationComponent->count(0); i++){
		ui->correlationsGroupWidget->insertCorrelationWidget((static_cast<CorrelationWidget *>(m_correlationComponent->get(i))));
	}

}


void CommonSpectrumTabWidget::activate()
{
	for(int i = 0; i < m_correlationComponent->count(0); i++){
		ui->correlationsGroupWidget->insertCorrelationWidget((static_cast<CorrelationWidget *>(m_correlationComponent->get(i))));
	}

	foreach (SpectrumWidget* widget , m_widgetList) {

		ui->spectumWidgetsContainer->insertWidget(ui->spectumWidgetsContainer->count(), widget);
	}
}

void CommonSpectrumTabWidget::deactivate()
{
	foreach (SpectrumWidget* widget , m_widgetList) {

		ui->spectumWidgetsContainer->removeWidget(widget);
	}
}

QWidget *CommonSpectrumTabWidget::getWidget()
{
	return this;
}

SpectrumWidget *CommonSpectrumTabWidget::getSpectrumWidget()
{
	return NULL;
}

QLabel *CommonSpectrumTabWidget::getIndicator()
{
	return NULL;
}

int CommonSpectrumTabWidget::createRPC()
{
	return 0;
}

int CommonSpectrumTabWidget::closeRPC()
{
	return 0;
}

int CommonSpectrumTabWidget::createView(QWidget *view)
{
	return 0;
}

int CommonSpectrumTabWidget::createTree()
{
	return 0;
}


void CommonSpectrumTabWidget::insertSpectrumWidget(SpectrumWidget *spectrumWidget)
{
	if (NULL ==spectrumWidget) {
		return;
	}

	m_widgetList.append(spectrumWidget);

	ui->spectumWidgetsContainer->insertWidget(ui->spectumWidgetsContainer->count(), spectrumWidget);

}

TypeTabWidgetEnum CommonSpectrumTabWidget::getWidgetType() const
{
	return TypeCommonSpectrum;
}
