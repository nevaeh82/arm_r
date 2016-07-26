#ifndef ITABMANAGER_H
#define ITABMANAGER_H

#include <QString>

#include "Defines.h"

class IMessage;

class ITabManager
{
public:
	virtual ~ITabManager(){}

	virtual QString getStationName(const int id) = 0;
	virtual void setActiveTab(const int id) = 0;
	virtual void setResponseCommonFreq(quint32 freq)	= 0;
    virtual int getChannelCount() = 0;

};

#endif // ITABMANAGER_H
