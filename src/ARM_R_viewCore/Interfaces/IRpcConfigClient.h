#ifndef IRPCCONFIGCLIENT_H
#define IRPCCONFIGCLIENT_H

class QString;

class IRpcConfigClient
{

public:
	virtual ~IRpcConfigClient(){}

	virtual void requestGetStationList(const QString& filename)			= 0;
	virtual void requestGetAtlantConfiguration(const QString& filename)	= 0;
};

#endif // IRPCCONFIGCLIENT_H
