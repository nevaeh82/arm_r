#include "CommonSpectrumTabWidget.h"
#include "ui_CommonSpectrumTabWidget.h"

CommonSpectrumTabWidget::CommonSpectrumTabWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CommonSpectrumTabWidget)
{
	ui->setupUi(this);

	m_correlationControllers = NULL;
	m_treeModel = NULL;
	m_dbManager = NULL;

	m_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(CommonSpectrumTabWidget));
}

CommonSpectrumTabWidget::~CommonSpectrumTabWidget()
{
	delete ui;
}

void CommonSpectrumTabWidget::setDbManager(IDbManager* dbManager)
{
	m_dbManager = dbManager;

	QStringList headers;
	headers << tr("Name") << tr("Property");
	m_treeModel = new TreeModel(m_dbManager, headers, this);
	ui->settingsTreeView->setModel(m_treeModel);

	m_dbManager->registerReceiver(m_treeModel);
	connect(m_treeModel, SIGNAL(onItemAddedSignal()), ui->settingsTreeView , SLOT(expandAll()));
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

void CommonSpectrumTabWidget::setCorrelationComponent(ICorrelationControllersContainer* correlationControllers)
{
	m_correlationControllers = correlationControllers;

	for(int i = 0; i < m_correlationControllers->count(); i++){
		ui->correlationsGroupWidget->insertCorrelationWidget(m_correlationControllers->get(i));
	}
}


void CommonSpectrumTabWidget::activate()
{
	m_logger->debug("Activate common tab");
	for(int i = 0; i < m_correlationControllers->count(); i++){
		ui->correlationsGroupWidget->insertCorrelationWidget(m_correlationControllers->get(i));
	}

	foreach (ISpectrumWidget* widget , m_widgetList) {
		ui->spectumWidgetsContainer->insertWidget(ui->spectumWidgetsContainer->count(), widget->getWidget());
	}
}

void CommonSpectrumTabWidget::deactivate()
{
	m_logger->debug("Deactivate common tab");
	ui->correlationsGroupWidget->clearWidgetContainer();

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

int CommonSpectrumTabWidget::createView(QWidget*)
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

//	ui->spectumWidgetsContainer->insertWidget(ui->spectumWidgetsContainer->count(), spectrumWidget->getWidget());

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
