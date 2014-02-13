#ifndef ITABMANAGER_H
#define ITABMANAGER_H

#include <QString>
#include "ITabSpectrum.h"

class IMessage;

class ITabManager
{
public:
	~ITabManager(){}

    virtual QString getStationName(int id)              = 0;
	virtual void send_data(const QString &stationName, TypeCommand type, IMessage *data)     = 0;
    virtual void set_tab(int id)                        = 0;
};

#endif // ITABMANAGER_H
