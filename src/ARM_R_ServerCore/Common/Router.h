#ifndef ROUTER_H
#define ROUTER_H

#include <QObject>
#include <QThread>
#include <QMap>

#include "IRouter.h"
#include "IQueue.h"
#include "ISubscriber.h"
#include "../TCP/ITCPController.h"

#include "../Flakon/StationProperty.h"

class Router : public QObject, public IRouter
{
    Q_OBJECT
public:
    Router();
    ~Router();

public:
    virtual void set_queue(IQueue *queue);
    virtual void set_tcp_controller(ITCPController *tcp_controller);
    virtual void set_station_property(QMap<int, StationProperty *> *prop);
    virtual IQueue                  *get_queue();
    virtual ITCPController          *get_tcp_controller();

    virtual void set_subscriber(ISubscriber *scr);
    virtual ISubscriber* get_subscriber();
    virtual QMap<int, StationProperty* >* get_station_property();


private:
    IQueue                  *_queue;
    ITCPController          *_tcp_controller;
    ISubscriber*            _subscriber;

    QMap<int, StationProperty*>*    _map_station_property;
};

#endif // ROUTER_H
