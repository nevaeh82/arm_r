#include "TabSpectrum.h"
#include <QDebug>

TabSpectrum::TabSpectrum(TabsProperty* prop, ICommonComponents* common_components, ICommonComponents *common_correlations, TreeModel* model, DBManager* db_manager, ITabManager* tab_manager) :
    _control_widget(NULL),
    _current_stecked_widget(0)
{

    _view_stacked_widget = new QStackedWidget();
    _view_stacked_widget->setContentsMargins(0, 0, 0, 0);

    _pm_round_red = new QPixmap(":/images/signals/images/bullet_red.png");
    _pm_round_green = new QPixmap(":/images/signals/images/bullet_green.png");
    _indicator = new QLabel();
    _indicator->setFixedSize(16, 16);
    _indicator->setPixmap(_pm_round_red->scaled(16,16,Qt::KeepAspectRatio));
	connect(this, SIGNAL(signalChangeIndicator(int)), this, SLOT(m_slot_set_indicator(int)));

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

    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    _spectrumWidget = NULL;
    _rpc_client1 = NULL;
    _tree_view = NULL;
    _dock_controlPRM = NULL;

    _vboxlayout = new QVBoxLayout();
    _hboxlayout = new QHBoxLayout();

    start();

    this->setLayout(_hboxlayout);

	connect(this, SIGNAL(signalGetPointsFromRPCFlakon(QVector<QPointF>)), this, SLOT(m_slot_get_points_from_rpc(QVector<QPointF>)));

    _init();

//    for(int i = 0; i < _common_correlations->count(0); i++)
//    {
//        _vboxlayout->addWidget(static_cast<CorrelationWidget *>(_common_correlations->get(i)));
//    }
    _hboxlayout->insertLayout(2, _vboxlayout, Qt::AlignRight);

	connect(this, SIGNAL(signalPanoramaState(bool)), this, SLOT(m_slotPanoramaState(bool)));


}

TabSpectrum::~TabSpectrum()
{
    emit signalStopRPC();
//    delete _spectrumWidget;
    int error = closeRPC();

    if(_control_widget != NULL)
    {
        delete _control_widget;
    }
    delete _tree_view;
    delete _view_stacked_widget;

    if(_dock_controlPRM != NULL)
    {
        delete _controlPRM;
        delete _dock_controlPRM;
    }

    delete _vboxlayout;
    delete _hboxlayout;
}

int TabSpectrum::_init()
{
    int error = createView(this);
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
//    emit signalStartRPC();
    return 0;
}

int TabSpectrum::start()
{
//    view = new QWidget(this);
//    view->setMinimumSize(this->width(), this->height());
//    view->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

//    _hboxlayout->removeItem(_vboxlayout);
    CorrelationWidget *w;
    for(int i = 0; i < _common_correlations->count(0); i++)
    {
        w = static_cast<CorrelationWidget *>(_common_correlations->get(i));
        w->clear();
        _vboxlayout->addWidget(w);
    }

    switch(_current_stecked_widget)
    {
    case 0:
		m_slot_show_spectrum();
        break;
    case 1:
		m_slot_show_spectra();
        break;
    default:
		m_slot_show_spectrum();
        break;
    }

//    _hboxlayout->re 1, _vboxlayout, Qt::AlignRight);

//    emit signalStartRPC();

    return 0;
}

int TabSpectrum::stop()
{    
//    emit signalStopRPC();
    for(int i = 0; i < _common_correlations->count(0); i++)
    {
        _vboxlayout->removeWidget(static_cast<CorrelationWidget *>(_common_correlations->get(i)));
    }
//    _hboxlayout->removeItem(_vboxlayout);

    return 0;
}

void TabSpectrum::show()
{
    //_model->fill_model(this->_id);
    _spectrumWidget->setup();
//    emit signalStartRPC();
    this->show();
}

void TabSpectrum::hide()
{
//    emit signalStopRPC();
    this->hide();
}

int TabSpectrum::createRPC()
{
    _rpc_client1 = new RPCClient(_tab_property, _db_manager, this, _spectrumData, _controlPRM);
    QThread *thread_rpc_client = new QThread;

    connect(thread_rpc_client, SIGNAL(started()), _rpc_client1, SLOT(slotInit()));

//    connect(this, SIGNAL(signalStartRPC()), _rpc_client1, SLOT(slotStart()));
    connect(_rpc_client1, SIGNAL(signalFinished()), thread_rpc_client, SLOT(quit()));
    connect(thread_rpc_client, SIGNAL(finished()), thread_rpc_client, SLOT(deleteLater()));

    connect(_rpc_client1, SIGNAL(signalFinished()), _rpc_client1, SLOT(deleteLater()));
    connect(this, SIGNAL(signalStopRPC()), _rpc_client1, SLOT(slotStop()));
    connect(this, SIGNAL(signalFinishRPC()), _rpc_client1, SLOT(slotFinish()));



//    connect(_rpc_client1, SIGNAL(signalData2(QVector<QPointF>,bool)), _spectrumWidget, SLOT(slotSetFFT2(QVector<QPointF>,bool)));
//    connect(_rpc_client1, SIGNAL(signalSetParam(int,double,bool)), _spectrumWidget, SLOT(slotSetParam(int,double,bool)), Qt::QueuedConnection);

//    connect(_rpc_client1, SIGNAL(signalDataS(float*,float*)), _spectrumWidget, SLOT(_slotSetFFTSetup(float*,float*)), Qt::QueuedConnection);
//    connect(_rpc_client1, SIGNAL(signalData(float*,float*)), _spectrumWidget, SLOT(_slotSetFFT(float*,float*)), Qt::QueuedConnection);


    _rpc_client1->setParent(0);
    _rpc_client1->moveToThread(thread_rpc_client);
    thread_rpc_client->start();

    return 0;
}

int TabSpectrum::closeRPC()
{
    emit signalFinishRPC();
    return 0;
}

int TabSpectrum::createView(QWidget* view)
{
    _spectrumWidget = new GraphicWidget(0,0,_tab_property->get_name(), _id, this);//( _tab_property->get_ip_prm300(), _tab_property->get_ip_adc(), _tab_property->get_port_adc(), this, view);

	connect(this, SIGNAL(signalDoubleClicked(int,double,double)), this, SLOT(m_slot_double_clicked(int,double,double)));
    /// add to common spectra
    _common_components->set(_id, _spectrumWidget);


    _controlPRM = new ControlPRM(0, this);
    _dock_controlPRM = new QDockWidget(tr("Управление ПРМ300В"), this);
    _dock_controlPRM->setAllowedAreas(Qt::LeftDockWidgetArea);
    _dock_controlPRM->setWidget(_controlPRM);

	connect(_dock_controlPRM, SIGNAL(visibilityChanged(bool)), this, SLOT(m_slot_show_controlPRM(bool)));

    _dock_controlPRM->hide();
    _hboxlayout->insertWidget(0, _dock_controlPRM, Qt::AlignLeft);
//    _controlPRM->slotShow();

//    QGridLayout *l = new QGridLayout();
//    l->addWidget(_spectrumWidget, 0, 0);
//    this->setLayout(l);

//    QHBoxLayout *hbox = new QHBoxLayout;
//    hbox->addWidget(_spectrumWidget, Qt::AlignJustify);
//    _hboxlayout->addLayout(hbox);
//    _view_stacked_widget->addWidget(_spectrumWidget);

    _hboxlayout->insertWidget(1, _view_stacked_widget, Qt::AlignJustify);
//    _spectrumWidget->setLayout(this->layout());



//    for(int i = 0; i < 6; i++)
//    {
//        if(i == _id)
//        {
//            _map_correlation_widget->insert(i, NULL);
//            continue;
//        }
//        CorrelationWidget *cor = new CorrelationWidget(0,0, "", i);//( _tab_property->get_ip_prm300(), _tab_property->get_ip_adc(), _tab_property->get_port_adc(), this, view);
//        vboxlayout->addWidget(cor, Qt::AlignJustify);
//        _map_correlation_widget->insert(i, cor);
////        _hboxlayout->insertWidget(i+1, cor, Qt::AlignJustify);
//    }


    _spectrumData = new GraphicData(_spectrumWidget, _common_correlations, _tab_manager, _id);


    connect(_spectrumData, SIGNAL(signalDataS(float*,float*)), _spectrumWidget, SLOT(_slotSetFFTSetup(float*,float*)));
    connect(_spectrumData, SIGNAL(signalData(float*,float*)), _spectrumWidget, SLOT(_slotSetFFT(float*,float*)));

    //_common_components->set(_id, _spectrumWidget);

    QThread *thread_spectrum_client = new QThread;

//    connect(thread_spectrum_client, SIGNAL(started()), _spectrumWidget, SLOT(slotStart()));
    connect(_spectrumData, SIGNAL(signalFinished()), thread_spectrum_client, SLOT(quit()));
    connect(thread_spectrum_client, SIGNAL(finished()), thread_spectrum_client, SLOT(deleteLater()));

    connect(_spectrumData, SIGNAL(signalFinished()), _spectrumData, SLOT(deleteLater()));
//    connect(this, SIGNAL(signalFinishSpectrum()), _spectrumWidget, SLOT(slotStop()));

    _spectrumData->moveToThread(thread_spectrum_client);
    thread_spectrum_client->start();


//        _hboxlayout->addLayout(vboxlayout);
//    _hboxlayout->insertLayout(-1, vboxlayout);

    _current_stecked_widget = 0;
    _view_stacked_widget->setCurrentIndex(_current_stecked_widget);

    _init_spectrum();
    _init_spectra();

	m_slot_show_spectrum();

    return 0;
}

int TabSpectrum::createTree()
{
    _tree_view = new TreeView();
    _tree_view->setMinimumWidth(200);
    _tree_view->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);

    _tree_view->get_tree()->setMinimumWidth(200);

    _tree_view->get_tree()->header()->setResizeMode(QHeaderView::ResizeToContents);

    if(_id == 0)
    _model->fill_model(this->_id);

    _tree_view->get_tree()->setModel(_model);

    _control_widget = new ControlPanelWidgets(_tree_view->get_tree());
	connect(_control_widget->tb_spectra, SIGNAL(clicked()), this, SLOT(m_slot_show_spectra()));
	connect(_control_widget->tb_spectrum, SIGNAL(clicked()), this, SLOT(m_slot_show_spectrum()));

    ButtonShowPanel *pb = new ButtonShowPanel(_tree_view->get_tree());
    pb->setMaximumHeight(30);
    pb->setMaximumWidth(30);
    pb->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QGridLayout *gbox2 = new QGridLayout(_tree_view->get_tree());
    gbox2->setContentsMargins(0, 0, 0, 0);
    gbox2->setSpacing(0);
    gbox2->addWidget(pb, 0, 0, Qt::AlignRight | Qt::AlignTop);

    pb->setMouseTracking(true);

    _tree_view->get_tree()->setLayout(gbox2);

    connect(pb, SIGNAL(signalPanelShow()), _control_widget, SLOT(slotShow()));

    connect(_tree_view, SIGNAL(signalResize(int,int)), _control_widget, SLOT(slotSetSize(int,int)));

    _tree_view->get_tree()->header()->setResizeMode(QHeaderView::ResizeToContents);


//    QHBoxLayout *hbox = new QHBoxLayout();
//    hbox->addWidget(_tree_view, Qt::AlignRight);
//    _hboxlayout->addLayout(hbox, Qt::AlignRight);
    _hboxlayout->insertWidget(-1, _tree_view, Qt::AlignRight);
    return 0;
}

void TabSpectrum::set_indicator(int state)
{
    emit signalChangeIndicator(state);
}

QLabel* TabSpectrum::get_indicator()
{
    return _indicator;
}

void TabSpectrum::set_show_controlPRM(bool state)
{
    switch(state)
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

    }
}

void TabSpectrum::set_double_clicked(int id, double d1, double d2)
{
    emit signalDoubleClicked(id, d1, d2);
}

TabsProperty* TabSpectrum::get_tab_property()
{
    return _tab_property;
}


void TabSpectrum::set_selected_area(QMap<int, QVariant> data)
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

void TabSpectrum::set_command(TypeCommand type, IMessage *msg)
{
	switch(type)
	{
		case graphic:
			_rpc_client1->set_command(msg);
			break;
		case 2:
			break;
		default:
			break;
	}
}


///getting points from rpc (flakon)
void TabSpectrum::set_points_rpc(QVector<QPointF> points)
{
//    _spectrumWidget->_slotSetFFT(points, false);
//    _spectrumWidget->setFFT(points, false);
    emit signalGetPointsFromRPCFlakon(points);
}

void TabSpectrum::set_thershold(double y)
{
    _threshold = y;
}

void TabSpectrum::check_status()
{
    CommandMessage* msg = new CommandMessage(COMMAND_REQUEST_STATUS, QVariant());
	_tab_manager->send_data(_id, TypeCommand(graphic), msg);
}

void TabSpectrum::set_panorama(bool state)
{
	emit signalPanoramaState(state);
//	qDebug() << "PANORAMA = " << state;
}


/// in this thread set points from rpc
void TabSpectrum::m_slot_get_points_from_rpc(QVector<QPointF> points)
{
//	if()
    _spectrumData->set_data(points, false);
}

void TabSpectrum::m_slot_set_indicator(int state)
{
    switch(state)
    {
    case 1:
        _indicator->setPixmap(_pm_round_green->scaled(16,16,Qt::KeepAspectRatio));
        break;
    case 0:
        _indicator->setPixmap(_pm_round_red->scaled(16,16,Qt::KeepAspectRatio));
        break;
    default:
        _indicator->setPixmap(_pm_round_red->scaled(16,16,Qt::KeepAspectRatio));
        break;
    }
}

void TabSpectrum::_init_spectrum()
{
    _com_spectrum_widget = new QWidget();
    _gl_spectrum = new QGridLayout();

//    _spectrumWidget = _tab_property->get_graphic_widget();//_map_spectra->value(_id_spectra);//new GraphicWidget(this);


//    connect(_spectrumWidget, SIGNAL(selected(double, double, double, double)), this, SLOT(slotSelectedArea(double, double, double,double)));
//    connect(_spectrumWidget, SIGNAL(signalChoosedThreshold(double)), this, SLOT(slotSetThreshold(double)));
//    connect(_spectrumWidget, SIGNAL(signalAddSelToLists(int)), this, SLOT(slotAddToDBList(int)));
    _gl_spectrum->addWidget(_spectrumWidget, 0, 0/*, Qt::AlignVCenter*/);
    //_gl_spectrum->addWidget(_chb_setThreshold, 1, 0, Qt::AlignLeft | Qt::AlignTop);

    _com_spectrum_widget->setLayout(_gl_spectrum);
    _view_stacked_widget->addWidget(_com_spectrum_widget);
    _current_stecked_widget = 0;
    _view_stacked_widget->setCurrentIndex(_current_stecked_widget);
}

void TabSpectrum::_init_spectra()
{
    _com_spectra_widget = new QWidget();
    _gl_spectra = new QGridLayout();

    int i = 0;
    int j = 0;
//    bool state = false;
    QMap<int, IGraphicWidget *>* map_spectra = _common_components->get_components();
    QMap<int, IGraphicWidget *>::iterator it;
    for (it = (*map_spectra).begin(); it != (*map_spectra).end(); ++it)
    {
//         connect(it.value(), SIGNAL(selected(double, double)), this, SLOT(slotSelectedArea(double, double)));
        _gl_spectra->addWidget(dynamic_cast<QWidget *>(it.value()), i++, j, Qt::AlignLeft | Qt::AlignTop);

//         if(state == false)
//         {
//            j++;
//            state = true;
//         }
//         else
//         {
//             j = 0;
//             i++;
//             state = false;
//         }
    }

    _com_spectra_widget->setLayout(_gl_spectra);
    _view_stacked_widget->addWidget(_com_spectra_widget);
    _current_stecked_widget = 0;
    _view_stacked_widget->setCurrentIndex(_current_stecked_widget);

}

void TabSpectrum::m_slot_show_spectra()
{
    _dock_controlPRM->hide();
    IGraphicWidget *gr = _common_components->get(_id);

    _gl_spectra->removeWidget(dynamic_cast<QWidget *>(gr));

    int i = 0;
    int j = 0;
//    bool state = false;
    QMap<int, IGraphicWidget *>* map_spectra = _common_components->get_components();
    QMap<int, IGraphicWidget *>::iterator it;
    for (it = (*map_spectra).begin(); it != (*map_spectra).end(); ++it)
    {
        _gl_spectra->addWidget(dynamic_cast<QWidget *>(it.value()), i++, j);

//         if(state == false)
//         {
//            j++;
//            state = true;
//         }
//         else
//         {
//             j = 0;
//             i++;
//             state = false;
//         }
    }
//    _view->setModel(_model_spectra);
    /// set for model type of view
//    int id_view = 2;

//    _models_controller->reset_model(0, id_view);
//    _models_controller->setupData();
//    _view->setModel(_models_controller->get_model());
    _current_stecked_widget = 1;
    _view_stacked_widget->setCurrentIndex(_current_stecked_widget);


}

void TabSpectrum::m_slot_show_spectrum()
{
    int id_view = 1;
    IGraphicWidget *gr = _common_components->get(_id);
    if(gr == NULL)
        return;
    _gl_spectrum->removeWidget(dynamic_cast<QWidget *>(gr));
    _gl_spectrum->addWidget(dynamic_cast<QWidget *>(gr), 0, 0/*, Qt::AlignVCenter*/);

//    _models_controller->reset_model(_id_spectra, id_view);
//    _models_controller->setupData();
//    _view->setModel(_models_controller->get_model());
    _current_stecked_widget = 0;
    _view_stacked_widget->setCurrentIndex(_current_stecked_widget);
}

void TabSpectrum::m_slot_show_controlPRM(bool state)
{
    _spectrumWidget->set_coontrolPRM_state(state);
}

void TabSpectrum::m_slot_double_clicked(int id, double d1, double d2)
{
//    if(id != _id)
//    {
//    this->stop();
       _tab_manager->set_tab(id);
//    }
//    else if(_current_stecked_widget != 0)
//    {
		m_slot_show_spectrum();
		//    }
}

void TabSpectrum::m_slotPanoramaState(bool state)
{
	qDebug() << "slot Panorama = " << state;
	QMap<QString, QVariant>* getting_data;
	double start = 300;
	double end = 300;
	switch(state)
	{
		case true:
			getting_data = _db_manager->get(7, _id);
			start = getting_data->value("value").toDouble();
			getting_data = _db_manager->get(8, _id);
			end = getting_data->value("value").toDouble();
			_spectrumData->set_panorama(start, end);
			break;
		case false:
			_spectrumData->set_panorama_stop();
			break;
		default:
			break;
	}
}
