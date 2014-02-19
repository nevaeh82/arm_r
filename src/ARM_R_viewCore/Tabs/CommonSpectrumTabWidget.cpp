#include "CommonSpectrumTabWidget.h"
#include "ui_CommonSpectrumTabWidget.h"

CommonSpectrumTabWidget::CommonSpectrumTabWidget(IDbManager* dbManager, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CommonSpectrumTabWidget)
{
	ui->setupUi(this);

	m_dbManager = dbManager;

	m_correlationComponent = NULL;

	QStringList headers;
	headers << tr("Name") << tr("Property");
	m_treeModel = new TreeModel(m_dbManager, headers, this);

	connect(m_treeModel, SIGNAL(onItemAddedSignal()), ui->settingsTreeView , SLOT(expandAll()));

	ui->settingsTreeView->setModel(m_treeModel);
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
	foreach (ISpectrumWidget* widget , m_widgetList) {

		ui->spectumWidgetsContainer->removeWidget(widget->getWidget());
	}

	m_widgetList.clear();
}

void CommonSpectrumTabWidget::setCorrelationComponent(ICommonComponents *correlation)
{
	//ui->correlationsGroupWidget->
	m_correlationComponent = correlation;

	for(int i = 0; i < m_correlationComponent->count(0); i++){
		ui->correlationsGroupWidget->insertCorrelationWidget(m_correlationComponent->get(i));
	}

}


void CommonSpectrumTabWidget::activate()
{
	for(int i = 0; i < m_correlationComponent->count(0); i++){
		ui->correlationsGroupWidget->insertCorrelationWidget(m_correlationComponent->get(i));
	}

	foreach (ISpectrumWidget* widget , m_widgetList) {

		ui->spectumWidgetsContainer->insertWidget(ui->spectumWidgetsContainer->count(), widget->getWidget());
	}
}

void CommonSpectrumTabWidget::deactivate()
{
	foreach (ISpectrumWidget* widget , m_widgetList) {

		ui->spectumWidgetsContainer->removeWidget(widget->getWidget());
	}
}

QWidget *CommonSpectrumTabWidget::getWidget()
{
	return this;
}

ISpectrumWidget *CommonSpectrumTabWidget::getSpectrumWidget()
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


void CommonSpectrumTabWidget::insertSpectrumWidget(ISpectrumWidget *spectrumWidget)
{
	if (NULL ==spectrumWidget) {
		return;
	}

	m_widgetList.append(spectrumWidget);

	ui->spectumWidgetsContainer->insertWidget(ui->spectumWidgetsContainer->count(), spectrumWidget->getWidget());

	m_treeModel->onCleanSettings();

	QStringList stationsList;

	foreach (ISpectrumWidget* widget , m_widgetList) {
		stationsList << widget->getSpectrumName();
	}

	m_treeModel->setStationsList(stationsList);

}

TypeTabWidgetEnum CommonSpectrumTabWidget::getWidgetType() const
{
	return TypeCommonSpectrum;
}
