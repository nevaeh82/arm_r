#include "TabSpectrumWidgetController.h"
#include "UiDefines.h"

TabSpectrumWidgetController::TabSpectrumWidgetController(TabsProperty* prop, ICommonComponents* common_components, ICommonComponents *common_correlations, IDbManager* db_manager, ITabManager* tab_manager, QObject *parent) :
	QObject(parent)
{
	m_view = NULL;

	_threshold = -1;
	_common_correlations = common_correlations;
	_tab_manager = tab_manager;
	m_dbManager = db_manager;

	_map_correlation_widget = new QMap<int, IGraphicWidget *>;

	_common_components = common_components;
	_tab_property = prop;
	_id = _tab_property->get_id();
	m_stationName = _tab_property->get_name();

	QStringList headers;
	headers << tr("Name") << tr("Property");
	m_treeModel = new TreeModel(m_dbManager, headers, this);
	m_dbManager->registerReceiver(m_treeModel);

	m_treeDelegate = new TreeWidgetDelegate(this);

	_rpc_client1 = NULL;

	//activate();

	connect(this, SIGNAL(signalGetPointsFromRPCFlakon(QVector<QPointF>)), this, SLOT(_slot_get_points_from_rpc(QVector<QPointF>)));

	connect(this, SIGNAL(signalPanoramaState(bool)), this, SLOT(enablePanoramaSlot(bool)));


	///TODO: recheck
	//	for(int i = 0; i < _common_correlations->count(0); i++){
	//		ui->correlationsGroupWidget->insertCorrelationWidget((static_cast<CorrelationWidget *>(_common_correlations->get(i))));
	//	}
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

SpectrumWidget *TabSpectrumWidgetController::getSpectrumWidget()
{
	return m_view->getSpectrumWidget();
}

void TabSpectrumWidgetController::insertSpectrumWidget(SpectrumWidget *spectrumWidget)
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
	_rpc_client1 = new RPCClient(_tab_property, m_dbManager, this, _spectrumData, _controlPRM);
	QThread *thread_rpc_client = new QThread;

	connect(thread_rpc_client, SIGNAL(started()), _rpc_client1, SLOT(slotInit()));

	//    connect(this, SIGNAL(signalStartRPC()), _rpc_client1, SLOT(slotStart()));
	connect(_rpc_client1, SIGNAL(signalFinished()), thread_rpc_client, SLOT(quit()));
	connect(thread_rpc_client, SIGNAL(finished()), thread_rpc_client, SLOT(deleteLater()));

	connect(_rpc_client1, SIGNAL(signalFinished()), _rpc_client1, SLOT(deleteLater()));
	connect(this, SIGNAL(signalStopRPC()), _rpc_client1, SLOT(slotStop()));
	connect(this, SIGNAL(signalFinishRPC()), _rpc_client1, SLOT(slotFinish()));

	_rpc_client1->setParent(0);
	_rpc_client1->moveToThread(thread_rpc_client);
	thread_rpc_client->start();

	return 0;
}

TabsProperty* TabSpectrumWidgetController::get_tab_property()
{
	return _tab_property;
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

	for(int i = 0; i < _common_correlations->count(0); i++){
		m_view->insertCorrelationWidget((static_cast<CorrelationWidget *>(_common_correlations->get(i))));
	}

	SpectrumWidget* spectrumWidget = m_view->getSpectrumWidget();

	if (NULL == spectrumWidget) {
		return 0;
	}

	spectrumWidget->setTab(this);
	spectrumWidget->setId(_id);
	spectrumWidget->setSpectrumName(m_stationName);

	connect(m_view, SIGNAL(spectrumDoubleClickedSignal(int)), this, SLOT(spectrumDoubleClickedSlot(int)));

	/// add to common spectra
	_common_components->set(_id, spectrumWidget);

	/// TODO: update
	/*_controlPRM = new ControlPRM(0, this);
	_dock_controlPRM = new QDockWidget(tr("Управление ПРМ300В"), this);
	_dock_controlPRM->setAllowedAreas(Qt::LeftDockWidgetArea);
	_dock_controlPRM->setWidget(_controlPRM);

	connect(_dock_controlPRM, SIGNAL(visibilityChanged(bool)), this, SLOT(_slot_show_controlPRM(bool)));

	_dock_controlPRM->hide();
	_hboxlayout->insertWidget(0, _dock_controlPRM, Qt::AlignLeft);*/


	//    _controlPRM->slotShow();

	//    QGridLayout *l = new QGridLayout();
	//    l->addWidget(_spectrumWidget, 0, 0);
	//    this->setLayout(l);

	//    QHBoxLayout *hbox = new QHBoxLayout;
	//    hbox->addWidget(_spectrumWidget, Qt::AlignJustify);
	//    _hboxlayout->addLayout(hbox);
	//    _view_stacked_widget->addWidget(_spectrumWidget);

	//

	_spectrumData = new GraphicData(spectrumWidget, _common_correlations, _tab_manager, _id);

	connect(_spectrumData, SIGNAL(signalDataS(float*,float*)), spectrumWidget, SLOT(_slotSetFFTSetup(float*,float*)));
	connect(_spectrumData, SIGNAL(signalData(float*,float*)), spectrumWidget, SLOT(_slotSetFFT(float*,float*)));

	QThread *thread_spectrum_client = new QThread;

	connect(_spectrumData, SIGNAL(signalFinished()), thread_spectrum_client, SLOT(quit()));
	connect(thread_spectrum_client, SIGNAL(finished()), thread_spectrum_client, SLOT(deleteLater()));
	connect(_spectrumData, SIGNAL(signalFinished()), _spectrumData, SLOT(deleteLater()));
	_spectrumData->moveToThread(thread_spectrum_client);
	thread_spectrum_client->start();

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

	m_treeModel->setStationsList(QStringList(m_stationName));

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

	SettingsNode settingsNode = m_dbManager->getSettingsNode(m_stationName);

	foreach (Property property, settingsNode.properties) {
		if (DB_FREQUENCY_PROPERTY == property.name) {
			frequency = property.value.toDouble();
			break;
		}
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

	SettingsNode  settingsNode = m_dbManager->getSettingsNode(m_stationName);

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
	if (graphic != type) {
		return;
	}

	_rpc_client1->set_command(msg);
}


///getting points from rpc (flakon)
void TabSpectrumWidgetController::set_points_rpc(QVector<QPointF> points)
{
	emit signalGetPointsFromRPCFlakon(points);
}

void TabSpectrumWidgetController::set_thershold(double y)
{
	_threshold = y;
}

void TabSpectrumWidgetController::check_status()
{
	CommandMessage* msg = new CommandMessage(COMMAND_REQUEST_STATUS, QVariant());
	_tab_manager->send_data(_id, TypeCommand(graphic), msg);
}

void TabSpectrumWidgetController::set_panorama(bool state)
{
	emit signalPanoramaState(state);
}


/// in this thread set points from rpc
void TabSpectrumWidgetController::_slot_get_points_from_rpc(QVector<QPointF> points)
{
	_spectrumData->set_data(points, false);
}


void TabSpectrumWidgetController::_slot_show_controlPRM(bool state)
{
	if (NULL ==m_view) {
		return;
	}

	m_view->getSpectrumWidget()->set_coontrolPRM_state(state);
}

void TabSpectrumWidgetController::spectrumDoubleClickedSlot(int id)
{
	_tab_manager->set_tab(id);
}

void TabSpectrumWidgetController::enablePanoramaSlot(bool isEnabled)
{
	qDebug() << "slot Panorama = " << isEnabled;

	double panoramaStartValue = 300;
	double panoramaEndValue = 300;

	if (isEnabled) {

		SettingsNode settingsNode =  m_dbManager->getSettingsNode(m_stationName);

		foreach (Property property, settingsNode.properties) {
			if (DB_PANORAMA_START_PROPERTY == property.name) {
				panoramaStartValue = property.value.toDouble();
			}
			else if (DB_PANORAMA_END_PROPERTY == property.name) {
				panoramaEndValue = property.value.toDouble();
			}
		}

		_spectrumData->set_panorama(panoramaStartValue, panoramaEndValue);

	} else {
		_spectrumData->set_panorama_stop();
	}

}
