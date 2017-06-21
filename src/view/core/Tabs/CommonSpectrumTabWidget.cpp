#include "CommonSpectrumTabWidget.h"
#include "ui_CommonSpectrumTabWidget.h"

#include "Info/StationConfiguration.h"

CommonSpectrumTabWidget::CommonSpectrumTabWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CommonSpectrumTabWidget)
{
	ui->setupUi(this);

	m_rpcFlakonClient = NULL;

	m_correlationControllers = NULL;
	m_treeModel = NULL;
	m_dbManager = NULL;

	m_treeDelegate = new TreeWidgetDelegate(this);

	m_pmRoundRed = new QPixmap(":/images/bullet_red.png");
	m_pmRoundGreen = new QPixmap(":/images/bullet_green.png");

	m_indicatorLabel = new QLabel(this);
	m_indicatorLabel->setFixedSize(1, 1);
	m_indicatorLabel->setPixmap(m_pmRoundRed->scaled(16,16,Qt::KeepAspectRatio));

	connect(this, SIGNAL(setIndicatorStateSignal(int)), this, SLOT(setIndicatorStateSlot(int)));

	ui->settingsTreeView->setVisible(false);
}

CommonSpectrumTabWidget::~CommonSpectrumTabWidget()
{
	//log_debug("<<<<<<");
	delete m_pmRoundRed;
	delete m_pmRoundGreen;
	resetDbManager();

	ui->cpLayout->removeWidget(m_cpView);
	m_cpView->setParent(NULL);

	delete ui;
}

void CommonSpectrumTabWidget::setDbManager(IDbManager* dbManager)
{
	m_dbManager = dbManager;

	QStringList headers;
	headers << tr("Name") << tr("Property");
	m_treeModel = new TreeModel(m_dbManager, headers, this);
	ui->settingsTreeView->setModel(m_treeModel);
	ui->settingsTreeView->setItemDelegate(m_treeDelegate);

	m_dbManager->registerReceiver(m_treeModel);
	connect(m_treeModel, SIGNAL(onItemAddedSignal()), ui->settingsTreeView , SLOT(expandAll()));
    connect(m_treeModel, SIGNAL(signalFreqChanged(int)), this, SIGNAL(signalFreqChanged(int)));
}

void CommonSpectrumTabWidget::resetDbManager()
{
	if(m_dbManager) {
		m_dbManager->deregisterReceiver(m_treeModel);
		m_dbManager = NULL;
	}
}

void CommonSpectrumTabWidget::setDbStationController(DBStationController *)
{

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

//	for(int i = 0; i < m_correlationControllers->count(); i++){
//		ui->correlationsGroupWidget->insertCorrelationWidget(m_correlationControllers->get(i));
//	}
}

void CommonSpectrumTabWidget::setAnalysisComponent(IAnalysisWidget *analysis)
{
//	m_analysis = analysis;
//	ui->analysisGroupWidget->insertAnalysisWidget(analysis);
}


void CommonSpectrumTabWidget::activate()
{
	ui->cpLayout->addWidget( m_cpView );

	foreach (ISpectrumWidget* widget , m_widgetList) {
		ui->spectumWidgetsContainer->insertWidget(ui->spectumWidgetsContainer->count(), widget->getWidget(), 1);

		widget->updateSelection();
	}

	ui->correlationsGroupWidget->clearWidgetContainer();
	for(int i = 0; i < m_correlationControllers->count(); i++){
		ui->correlationsGroupWidget->insertCorrelationWidget(i, m_correlationControllers->get(i));
	}
}

void CommonSpectrumTabWidget::deactivate()
{
	ui->correlationsGroupWidget->clearWidgetContainer();

	foreach (ISpectrumWidget* widget , m_widgetList) {
		ui->spectumWidgetsContainer->removeWidget(widget->getWidget());
	}
}

void CommonSpectrumTabWidget::updateListsSelections()
{

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
	return m_indicatorLabel;
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

void CommonSpectrumTabWidget::setFlakonRpcClient(RpcFlakonClientWrapper *rpcClient)
{
	m_rpcFlakonClient =rpcClient;
}


void CommonSpectrumTabWidget::insertSpectrumWidget(ISpectrumWidget *spectrumWidget)
{
	if (NULL ==spectrumWidget) {
		return;
	}

	m_widgetList.append(spectrumWidget);

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


void CommonSpectrumTabWidget::setStationNamesList(const QStringList &stationsList)
{
    m_treeDelegate->setStationNamesList(stationsList);
}

void CommonSpectrumTabWidget::setControlPanelWidget(ControlPanelWidget *wgt)
{
    m_cpView = wgt;
}

void CommonSpectrumTabWidget::setIndicatorState(int id, int state)
{
	emit setIndicatorStateSignal(state);
}

void CommonSpectrumTabWidget::setIndicatorStateSlot(int state)
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


void CommonSpectrumTabWidget::onMethodCalled(const QString &method, const QVariant &argument)
{
}

