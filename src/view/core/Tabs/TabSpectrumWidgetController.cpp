#include "TabSpectrumWidgetController.h"
#include "UiDefines.h"

TabSpectrumWidgetController::TabSpectrumWidgetController(
		IStation* station,
		ICorrelationControllersContainer* correlationControllers,
		ITabManager* tabManager,
		TabSpectrumWidget* tabSpectrumWidget,
		const QString rpcHost,
		const quint16 rpcPort,
		QObject *parent)
	: QObject(parent)
	, m_view( tabSpectrumWidget )
	, m_rpcPrmClient( NULL )
	, m_treeModel( NULL )
	, m_dbManager( NULL )
	, m_dbStationController( NULL )
	, m_indicatorLabel( NULL )
	, m_spectrumDataSource( NULL )
	, m_spectrumWidget( NULL )
	, m_threshold( -1 )
	, m_station( station )
	, m_correlationControllers( correlationControllers )
	, m_tabManager( tabManager )
	, m_treeDelegate( new TreeWidgetDelegate(this) )
	, m_rpcHost( rpcHost )
	, m_rpcPort( rpcPort )
	, m_rpcFlakonClient( NULL )
	, m_controlPanelController( NULL )
	, m_currentCorrelation( 0 )
	, m_isPanoramaEnabled(false)
	, m_controlPanelControllerTrue( NULL)
{
	connect(this, SIGNAL(signalGetPointsFromRPCFlakon(QByteArray)), this, SLOT(slotGetPointsFromRpc(QByteArray)));
	connect(this, SIGNAL(signalPanoramaState(bool)), this, SLOT(enablePanoramaSlot(bool)));
	connect(&m_timerStatus, SIGNAL(timeout()), this, SLOT(slotCheckStatus()));

	m_timerStatus.start(2000);
}

TabSpectrumWidgetController::~TabSpectrumWidgetController()
{
	log_debug("~() <<<<<<<<<<<");
	m_dbManager->deregisterReceiver(m_treeModel);

	m_rpcPrmClient->deregisterReceiver( m_spectrumDataSource );
	m_rpcPrmClient->deregisterReceiver( this );

	foreach (CorrelationWidgetDataSource* correlationWidgetDataSource, m_correlationDataSourcesList){
		m_rpcPrmClient->deregisterReceiver(correlationWidgetDataSource);
		if (m_rpcFlakonClient != NULL) {
			m_rpcFlakonClient->deregisterReceiver(correlationWidgetDataSource);
		}
	}

	foreach (CorrelationWidgetDataSource* correlationWidgetDataSource, m_correlationDataSourcesList){
		correlationWidgetDataSource->deregisterCorrelationReceiver( (ICorrelationListener*) m_spectrumWidget );

		correlationWidgetDataSource->deleteLater();
	}

	m_spectrumDataSource->deregisterReceiver(m_spectrumWidget);
	if (m_rpcFlakonClient != NULL) {
		m_rpcFlakonClient->deregisterReceiver( m_spectrumDataSource );
	}

	foreach (QThread* thread, m_threadSrcList) {
		thread->exit();
		//thread->deleteLater();
	}

	log_debug("~() >>>>>>>>>>>>");
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

	QVariant value = m_dbManager->getPropertyValue(m_station->getName(), DB_FREQUENCY_PROPERTY);
	m_view->getSpectrumWidget()->setZeroFrequency(value.toDouble());

	value = m_dbManager->getPropertyValue(m_station->getName(), DB_START_PROPERTY);

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
	m_rpcPrmClient = new RpcPrmClient( m_station, m_dbManager, NULL, this );
	m_rpcPrmClient->start( m_rpcPort, QHostAddress( m_rpcHost ) );
	m_rpcPrmClient->registerReceiver( m_spectrumDataSource );
	m_rpcPrmClient->registerReceiver( this );

	foreach (CorrelationWidgetDataSource* correlationWidgetDataSource, m_correlationDataSourcesList){
		m_rpcPrmClient->registerReceiver(correlationWidgetDataSource);
	}

	m_view->setRpcPrmClient(m_rpcPrmClient);

//	m_spectrumDataSource->setPrmRpcClient(m_rpcPrmClient);
}

void TabSpectrumWidgetController::createView()
{
	for(int i = 0; i < m_correlationControllers->count(); i++){
		ICorrelationWidget* correlationWidget = m_correlationControllers->get(i);
		m_view->insertCorrelationWidget(correlationWidget);

		CorrelationWidgetDataSource* correlationDataSource = new CorrelationWidgetDataSource(correlationWidget, m_tabManager, i, 0);
		correlationDataSource->registerReceiver(correlationWidget);


		//Maybe it will better perfomance
		QThread* thread = new QThread;
		connect(correlationDataSource, SIGNAL(destroyed()), thread, SLOT(terminate()));
		connect(thread, SIGNAL(finished()), correlationDataSource, SLOT(deleteLater()), Qt::DirectConnection);
		connect(thread, SIGNAL(destroyed()), correlationDataSource, SLOT(deleteLater()));

		correlationDataSource->moveToThread(thread);
		thread->start();

		m_threadSrcList.append(thread);

		//---------------------------------------

		if( m_rpcFlakonClient != NULL ) {
			m_rpcFlakonClient->registerReceiver( correlationDataSource );
		}

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
    m_spectrumDataSource->setDBManager(m_dbManager);
    m_spectrumDataSource->setName(m_station->getName());
	m_spectrumDataSource->registerReceiver(m_spectrumWidget);

    m_spectrumDataSource->setTabManager(m_tabManager);

	foreach (CorrelationWidgetDataSource* correlationWidgetDataSource, m_correlationDataSourcesList){
		correlationWidgetDataSource->registerCorrelationReceiver(dynamic_cast<ICorrelationListener*>(m_spectrumWidget));
	}

	if (m_rpcFlakonClient != NULL) {
		m_rpcFlakonClient->registerReceiver( m_spectrumDataSource );
	}
	if(m_rpcPrmClient != NULL)
	{
		m_rpcPrmClient->registerReceiver( m_spectrumDataSource );
	}
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
    /// 2 - change frequency
    /// 3 - changed frequency
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
        if(state < 2)
        {
		if(m_timerStatus.isActive())
		{
			m_timerStatus.stop();
		}
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
	m_station->setSelectedArea( selection );
}

void TabSpectrumWidgetController::sendCommand(TypeCommand type, IMessage *msg)
{
	switch (type){
		case TypePanoramaCommand :
			emit signalPanoramaState(m_spectrumDataSource->isPanoramaEnabled());
			return;
		case TypeGraphicCommand :
			m_rpcPrmClient->setCommand(msg);
			return;
	}
}

void TabSpectrumWidgetController::enableCorrelation(bool enable)
{
	if (m_rpcFlakonClient == NULL) return;

	m_rpcFlakonClient->sendBandwidth(m_station->getId(), m_station->getBandwidth());
	m_rpcFlakonClient->sendShift(m_station->getId(), m_station->getShift());
	m_rpcFlakonClient->sendCenter(m_station->getId(), m_station->getCenter());

	m_rpcFlakonClient->sendCorrelation( m_station->getId(), m_station->getCenter(), enable );

	m_currentCorrelation = m_station->getCenter();
	dynamic_cast<ControlPanelController*>(m_controlPanelController)->setCorrelationFrequencyValue(m_currentCorrelation);
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
	sendCommand(TypeGraphicCommand, msg);
}

void TabSpectrumWidgetController::recognize()
{
	m_station->recognize();
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
    m_isPanoramaEnabled = isEnabled;

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

void TabSpectrumWidgetController::onSettingsNodeChanged(const SettingsNode& property)
{
}

void TabSpectrumWidgetController::onPropertyChanged(const Property & property)
{
	QString stationName = m_dbManager->getObjectName(property.pid);

	if (stationName != m_station->getName()){
		return;
	}

	if( DB_LEADING_OP_PROPERTY == property.name ) {
		if (m_rpcFlakonClient == NULL) return;

		log_debug( QString("Property changed: ") + DB_LEADING_OP_PROPERTY );

		m_rpcFlakonClient->sendMainStationCorrelation( m_station->getId(), property.value.toString() );
		return;
	}

	if( DB_AVERAGING_PROPERTY == property.name ) {
		if (m_rpcFlakonClient == NULL) return;

		m_rpcFlakonClient->sendAvarageSpectrum( m_station->getId(), property.value.toInt() );
		return;
	}

	int commandCode = 0;
	Property inProperty = property;
	TypeCommand commandType = TypeUnknownCommand;

	if(DB_FREQUENCY_PROPERTY == inProperty.name) {
        m_view->setIndicatorState(2);
        if(!m_isPanoramaEnabled)
            m_spectrumWidget->setZeroFrequency(property.value.toDouble());	//remove it to class then answer from prm
		commandCode = COMMAND_PRM_SET_FREQ;
		commandType = TypeGraphicCommand;
	}
	else if(DB_PANORAMA_START_PROPERTY == inProperty.name) {
		commandCode = COMMAND_SET_PANORAMA_START_VALUE;
		commandType = TypePanoramaCommand;
	}
	else if(DB_PANORAMA_END_PROPERTY == inProperty.name) {
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

void TabSpectrumWidgetController::updateDbStationsLists()
{
    emit signalUpdateDBStationsLists();
    /// TODO
}

void TabSpectrumWidgetController::updateListsSelections()
{
    m_view->updateDBListsAreas();
}

void TabSpectrumWidgetController::onMethodCalled(const QString& method, const QVariant& argument)
{
	if( method == RPC_PRM_STATE_CHANGED ) {
		setIndicator( argument.toInt() );
		return;
	}
    if( method == RPC_PRM_FREQUENCY_CHANGED) {
        setIndicator( argument.toInt() );
        return;
}
}

void TabSpectrumWidgetController::setControlPanelController(ICorrelationListener* controller)
{
	m_controlPanelController = controller;

	foreach (CorrelationWidgetDataSource* correlationWidgetDataSource, m_correlationDataSourcesList){
		correlationWidgetDataSource->registerCorrelationReceiver(m_controlPanelController);
	}
}

void TabSpectrumWidgetController::setControlPanelControllerTrue(ControlPanelController *controller)
{
	m_controlPanelControllerTrue = controller;
	m_spectrumWidget->setControlPanelController(m_controlPanelControllerTrue);
}


void TabSpectrumWidgetController::setStationNamesList(const QStringList &stationsList)
{
	m_treeDelegate->setStationNamesList(stationsList);
}

void TabSpectrumWidgetController::setRpcFlakonClient(RpcFlakonClientWrapper *client)
{
	RpcFlakonClientWrapper *old = m_rpcFlakonClient;

	m_rpcFlakonClient = client;

	if( m_spectrumDataSource != NULL ) {
		m_rpcFlakonClient->registerReceiver( m_spectrumDataSource );

		if (old != NULL) {
			old->deregisterReceiver( m_spectrumDataSource );
		}
	}

	foreach( CorrelationWidgetDataSource *ds, m_correlationDataSourcesList ) {
		m_rpcFlakonClient->registerReceiver( ds );
		if( old != NULL ) {
			old->deregisterReceiver( ds );
		}
	}
}
