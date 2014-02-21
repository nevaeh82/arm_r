#include "TabSpectrumWidgetController.h"
#include "UiDefines.h"

#define DEFAULT_RPC_PORT		24500

TabSpectrumWidgetController::TabSpectrumWidgetController(IStation* prop, ICorrelationControllersContainer* correlationControllers, IDbManager* db_manager, ITabManager* tab_manager, QObject *parent) :
	QObject(parent)
{
	m_view = NULL;

	m_rpcHostAddress = "127.0.0.1";
	m_rpcHostPort = DEFAULT_RPC_PORT;

	_threshold = -1;
	m_correlationControllers = correlationControllers;
	m_tabManager = tab_manager;
	m_dbManager = db_manager;
	m_dbManager->registerReceiver(this);

	_map_correlation_widget = new QMap<int, IGraphicWidget *>;

	m_station = prop;

	QStringList headers;
	headers << tr("Name") << tr("Property");
	m_treeModel = new TreeModel(m_dbManager, headers, this);
	m_dbManager->registerReceiver(m_treeModel);

	m_treeDelegate = new TreeWidgetDelegate(this);

	m_rpcClient = NULL;

	QString settingsFile = QCoreApplication::applicationDirPath();
	settingsFile.append("./Tabs/RPC.ini");
	readSettings(settingsFile);

	connect(this, SIGNAL(signalGetPointsFromRPCFlakon(QByteArray)), this, SLOT(_slot_get_points_from_rpc(QByteArray)));

	connect(this, SIGNAL(signalPanoramaState(bool)), this, SLOT(enablePanoramaSlot(bool)));
}

TabSpectrumWidgetController::~TabSpectrumWidgetController()
{
	emit signalStopRPC();
	closeRPC();
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

	m_view->getSpectrumWidget()->setSelection(start, end);
}

void TabSpectrumWidgetController::deactivate()
{

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

int TabSpectrumWidgetController::init()
{
	int error = createView();
	if(error != 0)
	{
		return error;
	}

	error = createTree();
	if(error != 0)
	{
		return error;
	}

	error = createRPC();
	if(error != 0)
	{
		return error;
	}

	return 0;
}

int TabSpectrumWidgetController::createRPC()
{
	m_rpcClient = new RPCClient(m_station, m_dbManager, this, _controlPRM, this);
	m_rpcClient->start(m_rpcHostPort, QHostAddress(m_rpcHostAddress));

	m_rpcClient->registerReceiver(m_spectrumDataSource);

	foreach (CorrelationWidgetDataSource* correlationWidgetDataSource, m_correlationDataSourcesList){
		m_rpcClient->registerReceiver(correlationWidgetDataSource);
	}

	return 0;
}

int TabSpectrumWidgetController::closeRPC()
{
	emit signalFinishRPC();
	return 0;
}

int TabSpectrumWidgetController::createView()
{
	if(NULL == m_view) {
		return 0;
	}

	for(int i = 0; i < m_correlationControllers->count(); i++){
		ICorrelationWidget* correlationWidget = m_correlationControllers->get(i);
		m_view->insertCorrelationWidget(correlationWidget);

		CorrelationWidgetDataSource* correlationDataSource = new CorrelationWidgetDataSource(correlationWidget, m_tabManager, i, this);
		correlationDataSource->registerReceiver(dynamic_cast<IDataSourceListener*>(correlationWidget));

		m_correlationDataSourcesList.append(correlationDataSource);
	}

	m_spectrumWidget = m_view->getSpectrumWidget();

	if (NULL == m_spectrumWidget) {
		return 0;
	}

	m_spectrumWidget->setTab(this);
	m_spectrumWidget->setId(m_station->getId());
	m_spectrumWidget->setSpectrumName(m_station->getName());

	connect(m_view, SIGNAL(spectrumDoubleClickedSignal(int)), this, SLOT(spectrumDoubleClickedSlot(int)));

	m_spectrumDataSource = new SpectrumWidgetDataSource(m_spectrumWidget, this);
	m_spectrumDataSource->registerReceiver(m_spectrumWidget);

	return 0;
}

int TabSpectrumWidgetController::createTree()
{
	if (NULL == m_view) {
		return 0;
	}

	connect(m_treeModel, SIGNAL(onItemAddedSignal()), m_view->getTreeView(), SLOT(expandAll()));

	m_view->getTreeView()->setModel(m_treeModel);
	m_view->getTreeView()->setItemDelegate(m_treeDelegate);

	m_treeModel->setStationsList(QStringList(m_station->getName()));

	return 0;
}

void TabSpectrumWidgetController::set_indicator(int state)
{
	if (NULL == m_view) {
		return;
	}

	m_view->setIndicatorState(state);
}

double TabSpectrumWidgetController::get_current_frequency()
{
	double frequency = 1830;

	QVariant value = m_dbManager->getPropertyValue(m_station->getName(), DB_FREQUENCY_PROPERTY);

	if (value.isValid()) {
		frequency = value.toDouble();
	}

	return frequency;
}

void TabSpectrumWidgetController::set_show_controlPRM(bool state)
{
	/// TODO: update
	/*switch(state)
	{
	case true:
		_dock_controlPRM->show();
		break;
	case false:
		_dock_controlPRM->hide();
		break;
	default:
		_dock_controlPRM->show();
		break;

	}*/
}

void TabSpectrumWidgetController::set_double_clicked(int id, double d1, double d2)
{
	emit signalDoubleClicked(id, d1, d2);
}



void TabSpectrumWidgetController::set_selected_area(const SpectrumSelection& selection)
{
	double x1 = selection.start.x();
	double x2 = selection.end.x();

	/// Ho HZ
	x1 /= 1000;
	x2 /= 1000;
	double dx;
	double center;

	if(x2 >= x1) {
		dx = x2 - x1;
		center = x1 + dx/2;
	} else {
		dx = x1 - x2;
		center = x2 + dx/2;
	}

	SettingsNode  settingsNode = m_dbManager->getSettingsNode(m_station->getName());

	foreach (Property prop, settingsNode.properties) {
		if (prop.name == DB_SELECTED_PROPERTY) {
			prop.value = QString::number(dx, 'f', 3);
			m_dbManager->updateProperty(prop);
		} else if (prop.name == DB_CENTER_PROPERTY) {
			prop.value = QString::number(center, 'f', 3);
			m_dbManager->updateProperty(prop);
		} else if (prop.name == DB_START_PROPERTY) {
			prop.value = QString::number(x1, 'f', 3);
			m_dbManager->updateProperty(prop);
		} else if (prop.name == DB_STOP_PROPERTY) {
			prop.value = QString::number(x2, 'f', 3);
			m_dbManager->updateProperty(prop);
		}
	}
}

void TabSpectrumWidgetController::set_command(TypeCommand type, IMessage *msg)
{
	if (TypeGraphicCommand != type) {
		return;
	}

	m_rpcClient->setCommand(msg);
}


///getting points from rpc (flakon)
// METHOD IS NOT USED
void TabSpectrumWidgetController::set_points_rpc(QVector<QPointF> points)
{
	/*QByteArray outBA;
	QDataStream stream(&outBA, QIODevice::WriteOnly);
	stream << points;

	emit signalGetPointsFromRPCFlakon(outBA);*/
}

void TabSpectrumWidgetController::set_thershold(double y)
{
	_threshold = y;
}

void TabSpectrumWidgetController::check_status()
{
	CommandMessage* msg = new CommandMessage(COMMAND_REQUEST_STATUS, QVariant());
	m_tabManager->sendCommand(m_station->getName(), TypeCommand(TypeGraphicCommand), msg);
}

void TabSpectrumWidgetController::set_panorama(bool state)
{
	emit signalPanoramaState(state);
}


/// in this thread set points from rpc
void TabSpectrumWidgetController::_slot_get_points_from_rpc(QByteArray points)
{
	//	_spectrumData->set_data(points, false);
}


void TabSpectrumWidgetController::_slot_show_controlPRM(bool state)
{
	if (NULL ==m_view) {
		return;
	}

	m_view->getSpectrumWidget()->setControlPrmState(state);
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

		SettingsNode settingsNode =  m_dbManager->getSettingsNode(m_station->getName());

		foreach (Property property, settingsNode.properties) {
			if (DB_PANORAMA_START_PROPERTY == property.name) {
				panoramaStartValue = property.value.toDouble();
			}
			else if (DB_PANORAMA_END_PROPERTY == property.name) {
				panoramaEndValue = property.value.toDouble();
			}
		}
		m_spectrumDataSource->setPanorama(true, panoramaStartValue, panoramaEndValue);

	} else {
		m_spectrumDataSource->setPanorama(false);
	}

}

void TabSpectrumWidgetController::readSettings(const QString &settingsFile)
{
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QSettings settings(settingsFile, QSettings::IniFormat);

	settings.setIniCodec(codec);

	m_rpcHostAddress = settings.value("RPC_UI/IP", "127.0.0.1").toString();
	m_rpcHostPort = settings.value("RPC_UI/Port", DEFAULT_RPC_PORT).toInt();
}


void TabSpectrumWidgetController::onSettingsNodeChanged(const SettingsNode &)
{
}

void TabSpectrumWidgetController::onPropertyChanged(const Property & property)
{
}

void TabSpectrumWidgetController::onCleanSettings()
{
}
