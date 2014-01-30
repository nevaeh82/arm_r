#ifndef ITABMANAGER_H
#define ITABMANAGER_H

#include <QString>

class IMessage;

class ITabManager
{
public:
    ~ITabManager(){};

    virtual QString getStationName(int id)              = 0;
	virtual void send_data(int pid, int type, IMessage *data)     = 0;
    virtual void set_tab(int id)                        = 0;
};

#endif // ITABMANAGER_H
