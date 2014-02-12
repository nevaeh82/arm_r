#include "TabSpectrumWidgetController.h"

TabSpectrumWidgetController::TabSpectrumWidgetController(TabsProperty* prop, ICommonComponents* common_components, ICommonComponents *common_correlations, TreeModel* model, DBManager* db_manager, ITabManager* tab_manager, QObject *parent) :
	QObject(parent)
{
	m_view = NULL;

	_threshold = -1;
	_common_correlations = common_correlations;
	_tab_manager = tab_manager;
	_model = model;
	_db_manager = db_manager;

	_map_correlation_widget = new QMap<int, IGraphicWidget *>;

	_common_components = common_components;
	_tab_property = prop;
	_id = _tab_property->get_id();
	_name = _tab_property->get_name();

	_rpc_client1 = NULL;

	//activate();

	connect(this, SIGNAL(signalGetPointsFromRPCFlakon(QVector<QPointF>)), this, SLOT(_slot_get_points_from_rpc(QVector<QPointF>)));

	connect(this, SIGNAL(signalPanoramaState(bool)), this, SLOT(_slotPanoramaState(bool)));


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
	//	ui->correlationsGroupWidget->clearWidgetContainer();

	//	for(int i = 0; i < _common_correlations->count(0); i++){

	//		ui->correlationsGroupWidget->insertCorrelationWidget((static_cast<CorrelationWidget *>(_common_correlations->get(i))));
	//	}

	//	foreach (SpectrumWidget* spectrumWidget, m_widgetList) {
	//		ui->spectumWidgetsContainer->insertWidget(ui->spectumWidgetsContainer->count(), spectrumWidget);
	//	}
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
	QString settingsFile = QCoreApplication::applicationDirPath();
	settingsFile.append("/Tabs/RPC.ini");

	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QSettings m_settings(settingsFile, QSettings::IniFormat);

	m_settings.setIniCodec(codec);

	QString ipRpc = m_settings.value("RPC_UI/IP", "127.0.0.1").toString();
	quint16 portRpc = m_settings.value("RPC_UI/Port", 24500).toInt();

	_rpc_client1 = new RPCClient(_tab_property, _db_manager, this, _spectrumData, _controlPRM, this);
	_rpc_client1->start(ipRpc, portRpc);
	/*QThread *thread_rpc_client = new QThread;

	connect(thread_rpc_client, SIGNAL(started()), _rpc_client1, SLOT(slotInit()));

	//    connect(this, SIGNAL(signalStartRPC()), _rpc_client1, SLOT(slotStart()));
	connect(_rpc_client1, SIGNAL(signalFinished()), thread_rpc_client, SLOT(quit()));
	connect(thread_rpc_client, SIGNAL(finished()), thread_rpc_client, SLOT(deleteLater()));

	connect(_rpc_client1, SIGNAL(signalFinished()), _rpc_client1, SLOT(deleteLater()));
	connect(this, SIGNAL(signalStopRPC()), _rpc_client1, SLOT(slotStop()));
	connect(this, SIGNAL(signalFinishRPC()), _rpc_client1, SLOT(slotFinish()));

	//_rpc_client1->setParent(0);
	_rpc_client1->moveToThread(thread_rpc_client);
	thread_rpc_client->start();*/

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
	spectrumWidget->setSpectrumName(_name);

	connect(m_view, SIGNAL(spectrumDoubleClickedSignal(int)), this, SLOT(spectrumDoubleClickedSlot(int)));

	/// add to common spectra
	_common_components->set(_id, spectrumWidget);

	/// TODO: update
	/*_controlPRM = new ControlPRM(0, this);
	_dock_controlPRM = new QDockWidget(tr("Управление П� М300В"), this);
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

	if(_id == 0) {
		_model->fill_model(this->_id);
	}

	m_view->getTreeView()->setModel(_model);

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
	QMap<QString, QVariant> *data = _db_manager->get(0, _id);
	return data->value("value").toDouble();
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



void TabSpectrumWidgetController::set_selected_area(QMap<int, QVariant> data)
{
	QMap<QString, QVariant>* getting_data;
	QMap<int, QVariant>::iterator it;
	for(it = data.begin(); it != data.end(); ++it)
	{
		getting_data = _db_manager->get(it.key(), _id);

		getting_data->insert("value", it.value().toDouble());
		_db_manager->set(getting_data);
	}
}

void TabSpectrumWidgetController::set_command(TypeCommand type, IMessage *msg)
{
	if (graphic != type) {
		return;
	}

	_rpc_client1->setCommand(msg);
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

void TabSpectrumWidgetController::_slotPanoramaState(bool state)
{
	qDebug() << "slot Panorama = " << state;
	QMap<QString, QVariant>* getting_data;
	double start = 300;
	double end = 300;

	if (true == state) {
		getting_data = _db_manager->get(7, _id);
		start = getting_data->value("value").toDouble();
		getting_data = _db_manager->get(8, _id);
		end = getting_data->value("value").toDouble();
		_spectrumData->set_panorama(start, end);
	} else {
		_spectrumData->set_panorama_stop();
	}

}
