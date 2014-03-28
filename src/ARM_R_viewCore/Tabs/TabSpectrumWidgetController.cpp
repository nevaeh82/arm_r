#include "TabSpectrumWidgetController.h"
#include "UiDefines.h"

#define DEFAULT_RPC_PORT		24500

TabSpectrumWidgetController::TabSpectrumWidgetController(IStation* station,
														 ICorrelationControllersContainer* correlationControllers,
														 ITabManager* tab_manager, QObject *parent) :
	QObject(parent)
{
	m_view = NULL;
	m_rpcClient = NULL;
	m_treeModel = NULL;
	m_dbManager = NULL;
    m_dbStationController = NULL;
//	m_controlPRM = NULL;
	m_indicatorLabel = NULL;

	m_spectrumDataSource = NULL;
	m_spectrumWidget = NULL;

	m_threshold = -1;

	m_station = station;
	m_correlationControllers = correlationControllers;
	m_tabManager = tab_manager;

	m_treeDelegate = new TreeWidgetDelegate(this);

	readSettings();

	connect(this, SIGNAL(signalGetPointsFromRPCFlakon(QByteArray)), this, SLOT(slotGetPointsFromRpc(QByteArray)));
	connect(this, SIGNAL(signalPanoramaState(bool)), this, SLOT(enablePanoramaSlot(bool)));
	connect(&m_timerStatus, SIGNAL(timeout()), this, SLOT(slotCheckStatus()));
}

TabSpectrumWidgetController::~TabSpectrumWidgetController()
{
}

void TabSpectrumWidgetController::appendView(TabSpectrumWidget *view)
{
	m_view = view;

	init();
}

void TabSpectrumWidgetController::activate()
{
	m_view->activate();

	double start = 0;
	double end = 0;

	QVariant value = m_dbManager->getPropertyValue(m_station->getName(), DB_START_PROPERTY);

	if (value.isValid()) {
		start = value.toDouble();
	}

	value = m_dbManager->getPropertyValue(m_station->getName(), DB_STOP_PROPERTY);

	if (value.isValid()) {
		end = value.toDouble();
	}

	if(start || end) {
		m_view->getSpectrumWidget()->setSelection(start, end);
	}

	m_timerStatus.start(2000);
}

void TabSpectrumWidgetController::deactivate()
{
	m_view->deactivate();
}

QWidget *TabSpectrumWidgetController::getWidget()
{
	return m_view;
}

QLabel *TabSpectrumWidgetController::getIndicator()
{
	return m_indicatorLabel;
}

ISpectrumWidget *TabSpectrumWidgetController::getSpectrumWidget()
{
	return m_view->getSpectrumWidget();
}

void TabSpectrumWidgetController::insertSpectrumWidget(ISpectrumWidget *spectrumWidget)
{
	m_view->insertSpectrumWidget(spectrumWidget);
}

TypeTabWidgetEnum TabSpectrumWidgetController::getWidgetType() const
{
	return TypeSignleSpectrum;
}

void TabSpectrumWidgetController::setDbManager(IDbManager* dbManager)
{
	m_dbManager = dbManager;

	QStringList headers;
	headers << tr("Name") << tr("Property");
	m_treeModel = new TreeModel(m_dbManager, headers, this);
    m_dbManager->registerReceiver(m_treeModel);
}

void TabSpectrumWidgetController::setDbStationController(DBStationController *controller)
{
    m_dbStationController = controller;
}

void TabSpectrumWidgetController::init()
{
	createView();
	createTree();
	createRPC();
}

void TabSpectrumWidgetController::createRPC()
{
	m_rpcClient = new RPCClient(m_station, m_dbManager, this, NULL/*m_controlPRM*/, this);
	m_rpcClient->start(m_rpcHostPort, QHostAddress(m_rpcHostAddress));

	m_rpcClient->registerReceiver(m_spectrumDataSource);

	foreach (CorrelationWidgetDataSource* correlationWidgetDataSource, m_correlationDataSourcesList){
		m_rpcClient->registerReceiver(correlationWidgetDataSource);
	}

	m_view->setRpcClient(m_rpcClient);
}

void TabSpectrumWidgetController::createView()
{
	for(int i = 0; i < m_correlationControllers->count(); i++){
		ICorrelationWidget* correlationWidget = m_correlationControllers->get(i);
		m_view->insertCorrelationWidget(correlationWidget);

		CorrelationWidgetDataSource* correlationDataSource = new CorrelationWidgetDataSource(correlationWidget, m_tabManager, i, this);
		correlationDataSource->registerReceiver(correlationWidget);

		m_correlationDataSourcesList.append(correlationDataSource);
	}

	m_spectrumWidget = m_view->getSpectrumWidget();

	m_spectrumWidget->setTab(this);
	m_spectrumWidget->setId(m_station->getId());
	m_spectrumWidget->setSpectrumName(m_station->getName());
    m_spectrumWidget->setDbManager(m_dbManager);
    m_spectrumWidget->setDbStationController(m_dbStationController);

	connect(m_view, SIGNAL(spectrumDoubleClickedSignal(int)), this, SLOT(spectrumDoubleClickedSlot(int)));

	m_spectrumDataSource = new SpectrumWidgetDataSource(m_spectrumWidget, this);
	m_spectrumDataSource->registerReceiver(m_spectrumWidget);
}

void TabSpectrumWidgetController::createTree()
{
	connect(m_treeModel, SIGNAL(onItemAddedSignal()), m_view->getTreeView(), SLOT(expandAll()));

	m_view->getTreeView()->setModel(m_treeModel);
	m_view->getTreeView()->setItemDelegate(m_treeDelegate);

	m_treeModel->setStationsList(QStringList(m_station->getName()));
}

void TabSpectrumWidgetController::setIndicator(int state)
{
	m_view->setIndicatorState(state);
	if(state < 1)
	{
		if(!m_timerStatus.isActive())
		{
			m_timerStatus.start(2000);
		}
	}
	else
	{
		if(m_timerStatus.isActive())
		{
			m_timerStatus.stop();
		}
	}
}

double TabSpectrumWidgetController::getCurrentFrequency()
{
	double frequency = 1830;

	QVariant value = m_dbManager->getPropertyValue(m_station->getName(), DB_FREQUENCY_PROPERTY);

	if (value.isValid()) {
		frequency = value.toDouble();
	}

	return frequency;
}

void TabSpectrumWidgetController::setShowControlPrm(bool state)
{
	/// TODO: update
	//_dock_controlPRM->show();
}

void TabSpectrumWidgetController::setDoubleClicked(int id, double d1, double d2)
{
	emit signalDoubleClicked(id, d1, d2);
}

void TabSpectrumWidgetController::setSelectedArea(const SpectrumSelection& selection)
{
	/// To HZ
	double x1 = selection.start.x() / 1000;
	double x2 = selection.end.x() / 1000;

	double dx = qAbs(x1 - x2);
	double center = (x1 + x2)/ 2;

	m_dbManager->updatePropertyValue(m_station->getName(), DB_SELECTED_PROPERTY, QString::number(dx, 'f', 3));
	m_dbManager->updatePropertyValue(m_station->getName(), DB_CENTER_PROPERTY, QString::number(center, 'f', 3));
	m_dbManager->updatePropertyValue(m_station->getName(), DB_START_PROPERTY, QString::number(x1, 'f', 3));
	m_dbManager->updatePropertyValue(m_station->getName(), DB_STOP_PROPERTY, QString::number(x2, 'f', 3));
}

void TabSpectrumWidgetController::sendCommand(TypeCommand type, IMessage *msg)
{
	if (TypePanoramaCommand == type) {
		emit signalPanoramaState(m_spectrumDataSource->isPanoramaEnabled());
	}

	if (TypeGraphicCommand != type) {
		return;
	}

	m_rpcClient->setCommand(msg);
}

///getting points from rpc (flakon)
// METHOD IS NOT USED
void TabSpectrumWidgetController::setPointsRpc(QVector<QPointF> points)
{
	/*QByteArray outBA;
	QDataStream stream(&outBA, QIODevice::WriteOnly);
	stream << points;

	emit signalGetPointsFromRPCFlakon(outBA);*/
}

void TabSpectrumWidgetController::setThreshold(double y)
{
	m_threshold = y;
}

void TabSpectrumWidgetController::checkStatus()
{
	CommandMessage* msg = new CommandMessage(COMMAND_REQUEST_STATUS, QVariant());
	m_tabManager->sendCommand(m_station->getName(), TypeCommand(TypeGraphicCommand), msg);
}

void TabSpectrumWidgetController::setPanorama(bool state)
{
	emit signalPanoramaState(state);
}

/// in this thread set points from rpc (flakon)
// METHOD IS NOT USED
void TabSpectrumWidgetController::slotGetPointsFromRpc(QByteArray points)
{
	//	_spectrumData->set_data(points, false);
}

void TabSpectrumWidgetController::slotShowControlPrm(bool state)
{
	m_view->getSpectrumWidget()->setControlPrmState(state);
	checkStatus();
}

void TabSpectrumWidgetController::spectrumDoubleClickedSlot(int id)
{
	m_tabManager->setActiveTab(id);
}

void TabSpectrumWidgetController::enablePanoramaSlot(bool isEnabled)
{
	qDebug() << "slot Panorama = " << isEnabled;

	double panoramaStartValue = 300;
	double panoramaEndValue = 300;

	if (isEnabled) {
		panoramaStartValue = m_dbManager->getPropertyValue(m_station->getName(), DB_PANORAMA_START_PROPERTY).toDouble();
		panoramaEndValue = m_dbManager->getPropertyValue(m_station->getName(), DB_PANORAMA_END_PROPERTY).toDouble();
	}

	m_spectrumDataSource->setPanorama(isEnabled, panoramaStartValue, panoramaEndValue);
}

void TabSpectrumWidgetController::slotCheckStatus()
{
	checkStatus();
}

void TabSpectrumWidgetController::readSettings()
{
	QString settingsFile = QCoreApplication::applicationDirPath();
	settingsFile.append("./Tabs/RPC.ini");

	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QSettings settings(settingsFile, QSettings::IniFormat);

	settings.setIniCodec(codec);

	m_rpcHostAddress = settings.value("RPC_UI/IP", "127.0.0.1").toString();
	m_rpcHostPort = settings.value("RPC_UI/Port", DEFAULT_RPC_PORT).toInt();
}

void TabSpectrumWidgetController::onSettingsNodeChanged(const SettingsNode& property)
{
}

void TabSpectrumWidgetController::onPropertyChanged(const Property & property)
{
	QString stationName = m_dbManager->getObjectName(property.pid);

	if (stationName != m_station->getName()){
		return;
	}

	Property inProperty = property;

	TypeCommand commandType = TypeUnknownCommand;

	int commandCode = 0;

	if(DB_FREQUENCY_PROPERTY == inProperty.name) {
		m_spectrumWidget->setZeroFrequency(property.value.toDouble());
		commandCode = COMMAND_PRM_SET_FREQ;
		commandType = TypeGraphicCommand;
	} else if(DB_LEADING_OP_PROPERTY == inProperty.name) {
		commandCode = COMMAND_FLAKON_SET_MAIN_STATION_COR;
		commandType = TypeGraphicCommand;
	} else if(DB_AVERAGING_PROPERTY == inProperty.name) {
		commandCode = COMMAND_FLAKON_SET_AVARAGE;
		commandType = TypeGraphicCommand;
	} else if(DB_PANORAMA_START_PROPERTY == inProperty.name) {
		commandCode = COMMAND_SET_PANORAMA_START_VALUE;
		commandType = TypePanoramaCommand;
	} else if(DB_PANORAMA_END_PROPERTY == inProperty.name) {
		commandCode = COMMAND_SET_PANORAMA_END_VALUE;
		commandType = TypePanoramaCommand;
	}

	if (0 == commandCode) {
		return;
	}

	CommandMessage *msg = new CommandMessage(commandCode, property.value);

	sendCommand(commandType, msg);
}

void TabSpectrumWidgetController::onCleanSettings()
{
}


void TabSpectrumWidgetController::setStationNamesList(const QStringList &stationsList)
{
	m_treeDelegate->setStationNamesList(stationsList);
}
