#include "DBManager.h"

DBManager::DBManager(ITabManager *tab_manager)
{
    _tab_manager = tab_manager;
    _db_controller = new DBController();

    QMap<QString, QVariant>* data;
    int i = 0;
    int pid = 0;
    do
    {
        i = 0;
        do
        {
            data = _db_controller->get_data(i++, pid);
            if(data->count() == 0)
                break;
            _db_cache.set(data);
        }while(data->count());
        pid++;
    }while(i != 1);


    QThread *thread_db_controller = new QThread;

    connect(_db_controller, SIGNAL(signalFinished()), thread_db_controller, SLOT(quit()));
    connect(thread_db_controller, SIGNAL(finished()), thread_db_controller, SLOT(deleteLater()));

    connect(_db_controller, SIGNAL(signalFinished()), _db_controller, SLOT(deleteLater()));

    _db_controller->moveToThread(thread_db_controller);
    thread_db_controller->start();
}

void DBManager::set_model(ITreeModel *model)
{
    _model = model;
}

DBManager::~DBManager()
{
    delete _db_controller;
}

int DBManager::set(QMap<QString, QVariant>* data)
{
    _db_cache.set(data);
    _model->addChild(data, Qt::EditRole);
//    emit signalSetDataDBController(data);
    _db_controller->set_data(data);

    int id = data->value("id").toInt();
    int pid = data->value("pid").toInt();
	TypeCommand type = TypeCommand(unknown);
    CommandMessage *msg;
    switch(id)
    {
    case 0:
        msg = new CommandMessage(COMMAND_PRM_SET_FREQ, data->value("value"));
		type = TypeCommand(graphic);
        break;
    case 5:
        msg = new CommandMessage(COMMAND_FLAKON_SET_MAIN_STATION_COR, data->value("value"));
		type = TypeCommand(graphic);
        break;
    case 6:
        msg = new CommandMessage(COMMAND_FLAKON_SET_AVARAGE, data->value("value"));
		type = TypeCommand(graphic);
        break;
	case 7:
		msg = new CommandMessage(COMMAND_SET_PANORAMA_START_VALUE, data->value("value"));
		type = TypeCommand(panorama);
		break;
	case 8:
		msg = new CommandMessage(COMMAND_SET_PANORAMA_END_VALUE, data->value("value"));
		type = TypeCommand(panorama);
		break;

    default:
        break;
    }
	_tab_manager->send_data(pid, type, msg);


    return 0;
}

QMap<QString, QVariant>* DBManager::get(int id, int pid)
{
    return _db_cache.get(pid, id);//_db_controller->get_data(id, pid);
}

QMap<QString, QVariant>* DBManager::get(int pid)
{
    return NULL;//_db_controller->get_data(pid);
}
