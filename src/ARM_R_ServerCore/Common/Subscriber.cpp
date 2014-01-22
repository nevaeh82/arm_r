#include "Subscriber.h"

#include <QDebug>

Pw::Logger::ILogger* Subscriber::m_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(ARM_R_Server));

Subscriber::Subscriber(QObject* parent):
	QObject(parent)
{
    qRegisterMetaType<rpc_flakon_msg> ("rpc_flakon_msg");
    connect(this, SIGNAL(signalGetData(int,rpc_flakon_msg)), this, SLOT(_slotGetData(int,rpc_flakon_msg)));
    connect(this, SIGNAL(signalAddSubscription(int,IClient*)), this, SLOT(_slotAddSubscription(int,IClient*)));
}

Subscriber::~Subscriber()
{
}

void Subscriber::add_subscription(int type, IClient *client)
{
    emit signalAddSubscription(type, client);
}

void Subscriber::_slotAddSubscription(int type, IClient *client)
{
    m_logger->info(QString("ADD subsription type = %1").arg(type));
    QVector<IClient *> clients;
    if(!_map_subscribe.contains(type))
    {
        clients.push_back(client);
        _map_subscribe.insert(type, clients);
        return;
    }
    clients = _map_subscribe.value(type);
    if(clients.contains(client))
    {
        return;
    }

    clients.push_back(client);
    _map_subscribe.insert(type, clients);

    QTextStream(stdout) << "RPC clients count = " << clients.count() << endl;;
}

void Subscriber::remove_subscription(int type, IClient* client)
{
    _mux.lock();
    QVector<IClient *> clients = _map_subscribe.value(type);
    if(clients.contains(client))
    {
        int index = clients.indexOf(client);
        clients.remove(index);
    }
    _mux.unlock();
}

void Subscriber::remove_subscription(int type)
{
    _mux.lock();
    _map_subscribe.remove(type);
    _mux.unlock();
}

void Subscriber::remove_subscription(IClient *client)
{
    _mux.lock();
    QMap<int, QVector<IClient *> >::iterator it;
    QVector<IClient *> vec;
    IClient *cl;
    for(it = _map_subscribe.begin(); it != _map_subscribe.end(); ++it)
    {
        vec = it.value();
        for(int i = 0; i < vec.size(); i++)
        {
            cl = vec.at(i);
            if(cl == client)
            {
                vec.remove(i);
            }
        }
        if(vec.isEmpty())
        {
            //_map_subscribe.remove(it.key());
        }
        _map_subscribe.insert(it.key(), vec);
    }
    _mux.unlock();
}

QVector<IClient *> Subscriber::get_subscribers(int type)
{
    return _map_subscribe.value(type);
}


/// signal data ready in some type
void Subscriber::data_ready(int type, QSharedPointer<IMessage> msg_ptr)
{
    emit signalGetData(type, msg_ptr);
}

void Subscriber::_slotGetData(int type, rpc_flakon_msg msg_ptr)
{
    int type1 = 1;
    int id = 0;
    IMessage *f = (msg_ptr.data());
    QByteArray* dd = f->get(id, type1);

//    _mux.lock();
    QVector<IClient *> cls = _map_subscribe.value(type);
    for(int i = 0; i < cls.count(); i++)
    {
        if(!cls.at(i))
            return;
        int cl_id = cls.at(i)->get_id();
        int cl_type = cls.at(i)->get_type();
        if((cl_type != 2) || (cl_id == id))
        {
            cls.at(i)->send_data(msg_ptr);
        }
    }
//    _mux.unlock();
}
