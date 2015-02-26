#ifndef IPLAYERDATASOURCE_H
#define IPLAYERDATASOURCE_H

class IDataSourceListener;

#include "BaseSubject.h"

class IDataSource: public BaseSubject<IDataSourceListener>
{
public:
	virtual ~IDataSource(){}

	virtual void sendCommand(int) = 0;
};

#endif // IPLAYERDATASOURCE_H
