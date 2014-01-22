#include "Router.h"

Router::Router(QObject* parent):
	QObject(parent)
{
    _map_station_property = NULL;
}

Router::~Router()
{
}

void Router::set_queue(IQueue *queue)
{
    _queue = queue;
    QThread *thread_queue = new QThread;

    connect(thread_queue, SIGNAL(started()), _queue, SLOT(exec()));
    connect(_queue, SIGNAL(finished()), thread_queue, SLOT(quit()));
    connect(_queue, SIGNAL(finished()), thread_queue, SLOT(deleteLater()));
    connect(thread_queue, SIGNAL(finished()), thread_queue, SLOT(deleteLater()));

    _queue->moveToThread(thread_queue);
    thread_queue->start();
}

void Router::set_tcp_controller(ITCPController *tcp_controller)
{
    _tcp_controller = tcp_controller;
}

void Router::set_station_property(QMap<int, StationProperty *> *prop)
{
    _map_station_property = prop;
}

IQueue *Router::get_queue()
{
    return _queue;
}

ITCPController *Router::get_tcp_controller()
{
    return _tcp_controller;
}

void Router::set_subscriber(ISubscriber *scr)
{
    _subscriber = scr;
}

ISubscriber* Router::get_subscriber()
{
    return _subscriber;
}

QMap<int, StationProperty *> *Router::get_station_property()
{
    return _map_station_property;
}
