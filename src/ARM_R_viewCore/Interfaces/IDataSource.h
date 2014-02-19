#ifndef IPLAYERDATASOURCE_H
#define IPLAYERDATASOURCE_H

class IDataSourceListener;

class IDataSource
{
	public:
		virtual ~IDataSource(){}

		virtual void sendCommand(int) = 0;
};

#endif // IPLAYERDATASOURCE_H
